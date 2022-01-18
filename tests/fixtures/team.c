#include <stdlib.h>

#include <uiligi/object.h>

#include "team.h"

static void set_name(UlgObject* object, UlgValue value) {
    Team* team = (void *)object;
    team->name = ulg_value_to_str(value);
}

static UlgValue get_name(const UlgObject* object) {
    const Team* team = (void *)object;
    return ulg_value_from_str(team->name);
}

const UlgClass* team_type(UlgClassFactory* factory) {
    UlgClass* class_ = ulg_class_declare(
        factory,
        "Team",
        sizeof(Team),
        ulg_object_type,
        0
    );

    ulg_class_add_property(class_, "name", get_name, set_name);

    return class_;
}
