#include <assert.h>
#include <stdlib.h>

#include <yaml.h>

#include "uiligi/template.h"

#include "uiligi/context.h"

struct UlgTemplate {
    UlgContext* context;
    yaml_document_t document;
};

UlgTemplate* ulg_template_from_string(UlgContext* context, const unsigned char* source) {
    (void)source;
    (void)context;
    return malloc(sizeof(UlgTemplate));
    /*
    template->context = context;

    yaml_parser_t parser;

    assert(yaml_parser_initialize(&parser));
    size_t length = strlen((const char*)source);
    yaml_parser_set_input_string(&parser, source, length);

    if (!yaml_parser_load(&parser, &template->document)) {
        // TODO : Handle error.
        free(template);
        return NULL;
    }

    yaml_parser_delete(&parser);
    return template;
}

void ulg_template_destroy(UlgTemplate* template) {
    yaml_document_delete(&template->document);
    free(template);
}

void ulg_template_instanciate(UlgTemplate* template) {
    yaml_document_t* document = &template->document;
    yaml_node_t* root_node = yaml_document_get_root_node(document);
    if(root_node->type != YAML_MAPPING_NODE) {
        // TODO : Handle error
        return;
    }

    yaml_node_pair_t* end = root_node->data.mapping.pairs.end;
    yaml_node_pair_t* start = root_node->data.mapping.pairs.start;

    if(end - start != 1) {
        // TODO : Handle error
        return;
    }

    yaml_node_t* key_node = yaml_document_get_node(document, start->key);
    yaml_node_t* value_node = yaml_document_get_node(document, start->value);

    if(key_node->type != YAML_SCALAR_NODE) {
        // TODO : Handle error.
        return;
    }

    const unsigned char* builder_name = key_node->data.scalar.value;
    UlgContext* context = template->context;
    */
}