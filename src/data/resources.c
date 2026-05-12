#include <stddef.h>
#include "resources.h"
#include "game_types.h"

static const ResourceTypeDef resource_types_def[] = {
    { FR_C1, FRONTIER },
    { FR_C2, FRONTIER },
    { FR_A1, FRONTIER },
    { FR_A2, FRONTIER },
    { EX_P1, EXOTIC },
    { EX_P2, EXOTIC },
    { EX_S1, EXOTIC },
    { EX_S2, EXOTIC },
    { RE_P, RESTRICTED },
    { RE_S, RESTRICTED },
};

static const ResourceTypeDefList resource_types_list = {
    .items = resource_types_def,
    .count = sizeof(resource_types_def) / sizeof(resource_types_def[0])
};

const ResourceTypeDefList *resource_types() {
    return &resource_types_list;
}

const ResourceTypeDef *resource_type(enum ResourceType type) {
    for(int i = 0; i < resource_types_list.count; i++) {
        if(resource_types_list.items[i].type == type) {
            return &resource_types_list.items[i];
        }
    }
    return NULL;
}