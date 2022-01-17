#include <stdlib.h>
#include <string.h>

#include <uiligi/class_registry.h>

#include "class_impl.h"
#include "utils/hashmap.h"

struct _UlgClassRegistry {
    struct hashmap* name_index;
    struct hashmap* id_index;
};

typedef struct {
    const UlgClass* class_;
} IndexItem;

typedef struct _UlgClassFactory {
    UlgClassRegistry* registry;
    uint64_t class_id;
} UlgClassFactory;

static uint64_t _hash_name(const void* item, uint64_t seed0, uint64_t seed1);
static int _compare_name(const void* a, const void* b, void* udata);
static void _free_item(void* item);
static uint64_t _hash_id(const void* item, uint64_t seed0, uint64_t seed1);
static int _compare_id(const void* a, const void* b, void* udata);


UlgClassRegistry* ulg_class_registry_new() {
    UlgClassRegistry* registry = malloc(sizeof(UlgClassRegistry));
    // Class data is owned by the name index.
    registry->name_index = hashmap_new(
        sizeof(UlgClass),
        0, 0, 0,
        &_hash_name,
        &_compare_name,
        _free_item,
        NULL
    );

    registry->id_index = hashmap_new(
        sizeof(UlgClass*),
        0, 0, 0,
        &_hash_id,
        &_compare_id,
        NULL,
        0
    );

    return registry;
}

void ulg_class_registry_free(UlgClassRegistry* registry) {
    hashmap_free(registry->id_index);
    hashmap_free(registry->name_index);
}

const UlgClass* ulg_class_get(UlgClassRegistry* registry, UlgClassDefinition definition) {
    uint64_t class_id = (size_t)definition; // TODO : will be anoying when wrapping to other languages.
    IndexItem* existing_class = hashmap_get(
        registry->id_index,
        &(IndexItem) {
            .class_ = &(UlgClass) { .id = class_id }
        }
    );

    if(existing_class) {
        return existing_class->class_;
    }

    const UlgClass* new_class = definition(
        &(UlgClassFactory) {
            .class_id = (uint64_t)definition,
            .registry = registry
        }
    );

    hashmap_set(
        registry->name_index,
        &(IndexItem) { .class_ = new_class }
    );

    hashmap_set(
        registry->id_index,
        &(IndexItem) { .class_ = new_class }
    );

    return new_class;
}

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
    UlgClassRegistry* registry = factory->registry;
    if(parent_definition) {
        parent = ulg_class_get(registry, parent_definition);
    }
    UlgClass* class_ =  _ulg_class_new(name, data_size, parent, vtable_size);
    class_->id = factory->class_id;
    return class_;
}

static uint64_t _hash_name(const void* item, uint64_t seed0, uint64_t seed1) {
    const IndexItem* index_item = item;
    const char* name = index_item->class_->name;
    return hashmap_sip(name, strlen(name), seed0, seed1);
}

static int _compare_name(const void* a, const void* b, void* udata) {
    (void)udata;
    const IndexItem* item_a = a;
    const IndexItem* item_b = b;
    return strcmp(item_a->class_->name, item_b->class_->name);
}

static void _free_item(void* item) {
    const IndexItem* index_item = item;
    _ulg_class_free((UlgClass*)index_item->class_);
}

static uint64_t _hash_id(const void* item, uint64_t seed0, uint64_t seed1) {
    (void)seed0;
    (void)seed1;
    const IndexItem* index_item = item;
    return index_item->class_->id;
}

static int _compare_id(const void* a, const void* b, void* udata) {
    (void)udata;
    const IndexItem* item_a = a;
    const IndexItem* item_b = b;
    return item_a->class_->id - item_b->class_->id;
}
