#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <uiligi/object.h>

#include "utils/hashmap.h"

struct _UlgContext {
    struct hashmap* name_index;
    struct hashmap* id_index;
};

struct _UlgClass {
    uint64_t id;
    const char* name;
    size_t data_size;
    struct hashmap* properties;
    const UlgClass* parent;
    size_t vtable_size; // This is here for checks only, could be removed.
    char vtable[];
};

typedef struct {
    const UlgClass* class_;
} _ClassItem;

typedef struct _UlgClassFactory {
    UlgContext* context;
    uint64_t class_id;
} UlgClassFactory;


static uint64_t _hash_class_name(const void* item, uint64_t seed0, uint64_t seed1);
static int _compare_class_name(const void* a, const void* b, void* udata);
static void _free_class_item(void* item);
static uint64_t _hash_class_id(const void* item, uint64_t seed0, uint64_t seed1);
static int _compare_class_id(const void* a, const void* b, void* udata);

UlgContext* ulg_context_new() {
    UlgContext* context = malloc(sizeof(UlgContext));
    // Class data is owned by the name index.
    context->name_index = hashmap_new(
        sizeof(_ClassItem),
        0, 0, 0,
        &_hash_class_name,
        &_compare_class_name,
        _free_class_item,
        NULL
    );

    context->id_index = hashmap_new(
        sizeof(_ClassItem),
        0, 0, 0,
        &_hash_class_id,
        &_compare_class_id,
        NULL,
        0
    );

    ulg_class_register(context, ulg_object_type);

    return context;
}

void ulg_context_free(UlgContext* context) {
    hashmap_free(context->id_index);
    hashmap_free(context->name_index);
    free(context);
}

void ulg_class_register(UlgContext* context, UlgClassDefinition definition) {
    const UlgClass* new_class = definition(
        &(UlgClassFactory) {
            .class_id = (uint64_t)definition, // To pass the id down to ulg_class_declare.
            .context = context
        }
    );

    void* replaced_item = NULL;
    replaced_item = hashmap_set(
        context->name_index,
        &(_ClassItem) { .class_ = new_class }
    );
    assert(!replaced_item);

    replaced_item = hashmap_set(
        context->id_index,
        &(_ClassItem) { .class_ = new_class }
    );
    assert(!replaced_item);
}

const UlgClass* ulg_class_get(UlgContext* context, UlgClassDefinition definition) {
    uint64_t class_id = (size_t)definition; // TODO : will be anoying when wrapping to other languages.
    _ClassItem* item = hashmap_get(
        context->id_index,
        &(_ClassItem) {
            .class_ = &(UlgClass) { .id = class_id }
        }
    );

    if(!item) return NULL;

    return item->class_;
}

const UlgClass* ulg_class_get_by_name(UlgContext* context, const char* name) {
    _ClassItem* item = hashmap_get(
        context->name_index,
        &(_ClassItem) {
            .class_ = &(UlgClass) { .name = name }
        }
    );
    if(!item) return NULL;
    return item->class_;
}

static UlgClass* _class_new(
    const char* name,
    size_t data_size,
    const UlgClass* parent,
    size_t vtable_size
);

UlgClass* ulg_class_declare(
    UlgClassFactory* factory,
    const char* name,
    size_t data_size,
    UlgClassDefinition parent_definition,
    size_t vtable_size
) {
    const UlgClass* parent = NULL;
    // We juste use an obfuscated type to prevent direct calls to ulg_class_declare
    // but allow them only in UlgClassDefinition callbacks.
    UlgContext* context = factory->context;
    if(parent_definition) {
        parent = ulg_class_get(context, parent_definition);
    }
    UlgClass* class_ =  _class_new(name, data_size, parent, vtable_size);
    class_->id = factory->class_id;
    return class_;
}

void* ulg_class_edit_vtable(UlgClass* class_) {
    return class_->vtable;
}

typedef struct {
    const char* name;
    UlgGetter getter;
    UlgSetter setter;
} _UlgProperty;

void ulg_class_add_property(UlgClass* class_, const char* name, UlgGetter getter, UlgSetter setter) {
    hashmap_set(
        class_->properties,
        &(_UlgProperty){
            .name = name,
            .getter = getter,
            .setter = setter
        }
    );
}

const UlgClass* ulg_object_type(UlgClassFactory* factory) {
    UlgClass* class_ = ulg_class_declare(
        factory,
        "Object",
        0,
        NULL,
        sizeof(UlgObjectVT)
    );

    memset(ulg_class_edit_vtable(class_), 0, sizeof(UlgObjectVT));

    return class_;
}

typedef struct _UlgObjectHeader {
    const UlgClass* class_;
    char data[];
} UlgObjectHeader;

static UlgObjectHeader* _get_header(UlgObject* object);
static const UlgObjectHeader* _get_header_const(const UlgObject* object);

UlgObject* ulg_object_new(const UlgClass* class_) {
    UlgObjectHeader* header = malloc(sizeof(UlgObjectHeader) + class_->data_size);
    header->class_ = class_;
    // Recursively initialize hierarchy, parent-first.
    // Return a pointer to the data, we offset the pointer to get the header each time.
    UlgObject* object = (UlgObject*)(header + 1);
    const UlgObjectVT* vtable = ulg_object_vtable(object);
    if(vtable->init) {
        vtable->init(object);
    }
    return object;
}

void ulg_object_free(UlgObject* object) {
    UlgObjectHeader* header = _get_header(object);
    const UlgObjectVT* vtable = ulg_object_vtable(object);
    if(vtable->cleanup) {
        vtable->cleanup(object);
    }

    free(header);
}

const void* ulg_object_vtable(const UlgObject* object) {
    const UlgObjectHeader* header = _get_header_const(object);
    return header->class_->vtable;
}

static const _UlgProperty* _get_property(const UlgClass* class_, const char* name);

UlgValue ulg_object_get(const UlgObject* object, const char* property_name) {
    const UlgObjectHeader* header = _get_header_const(object);
    const UlgClass* class_ = header->class_;
    const _UlgProperty* property = _get_property(class_, property_name);
    return property->getter(object);
}

void ulg_object_set(UlgObject* object, const char* property_name, UlgValue value) {
    UlgObjectHeader* header = _get_header(object);
    const UlgClass* class_ = header->class_;
    const _UlgProperty* property = _get_property(class_, property_name);
    property->setter(object, value);
}

static uint64_t _hash_class_name(const void* item, uint64_t seed0, uint64_t seed1) {
    const _ClassItem* index_item = item;
    const char* name = index_item->class_->name;
    return hashmap_sip(name, strlen(name), seed0, seed1);
}

static int _compare_class_name(const void* a, const void* b, void* udata) {
    (void)udata;
    const _ClassItem* item_a = a;
    const _ClassItem* item_b = b;
    return strcmp(item_a->class_->name, item_b->class_->name);
}

static void _free_class_item(void* item) {
    _ClassItem* class_item = item;
    UlgClass* class_ = (UlgClass*)class_item->class_;
    hashmap_free(class_->properties);
    free(class_);
}

static uint64_t _hash_class_id(const void* item, uint64_t seed0, uint64_t seed1) {
    (void)seed0;
    (void)seed1;
    const _ClassItem* index_item = item;
    return index_item->class_->id;
}

static int _compare_class_id(const void* a, const void* b, void* udata) {
    (void)udata;
    const _ClassItem* item_a = a;
    const _ClassItem* item_b = b;
    return item_a->class_->id - item_b->class_->id;
}

static const _UlgProperty* _get_property(const UlgClass* class_, const char* name) {
    _UlgProperty* result = hashmap_get(
        class_->properties,
        &(_UlgProperty){ .name = name }
    );
    assert(result); // TODO: Handle error.
    return result;
}

static uint64_t _property_hash(const void* item, uint64_t seed0, uint64_t seed1) {
    const _UlgProperty* property = item;
    const char* name = property->name;
    return hashmap_sip(name, strlen(name), seed0, seed1);
}

static int _property_compare(const void* a, const void* b, void* udata) {
    (void)udata;
    const _UlgProperty *property_a = a;
    const _UlgProperty *property_b = b;
    return strcmp(property_a->name, property_b->name);
}

static bool _copy_parent_properties(const void* item, void* udata) {
    struct hashmap* child_properties = udata;
    _UlgProperty copy;
    memcpy(&copy, item, sizeof(_UlgProperty));
    hashmap_set(child_properties, &copy);
    return true;
}

static UlgClass* _class_new(
    const char* name,
    size_t data_size,
    const UlgClass* parent,
    size_t vtable_size
) {
    // vtable size can not be less than the parent one.
    if(vtable_size == 0 && parent) {
        vtable_size = parent->vtable_size;
    }
    else {
        assert(!parent || parent->vtable_size <= vtable_size);
    }

    UlgClass* class_ = memcpy(
        malloc(sizeof(UlgClass) + vtable_size),
        &(UlgClass) {
            .name = name,
            .data_size = data_size,
            .parent = parent,
            .vtable_size = vtable_size
        },
        sizeof(UlgClass)
    );

    class_->properties = hashmap_new(
        sizeof(_UlgProperty),
        0, 0, 0,
        &_property_hash,
        &_property_compare,
        NULL,
        NULL
    );

    if(parent) {
        memcpy(class_->vtable, parent->vtable, parent->vtable_size);
        hashmap_scan(parent->properties, _copy_parent_properties, class_->properties);
    }

    return class_;
}

static UlgObjectHeader* _get_header(UlgObject* object) {
    return (UlgObjectHeader*)object - 1;
}

static const UlgObjectHeader* _get_header_const(const UlgObject* object) {
    return (const UlgObjectHeader*)object - 1;
}
