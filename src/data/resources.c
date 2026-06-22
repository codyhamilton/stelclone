#include <stddef.h>
#include "resources.h"
#include "game_types.h"

static const Resource resources_list[] = {
    { RES_FRONTIER_C1, FRONTIER },
    { RES_FRONTIER_C2, FRONTIER },
    { RES_FRONTIER_A1, FRONTIER },
    { RES_FRONTIER_A2, FRONTIER },
    { RES_EXOTIC_P1, EXOTIC },
    { RES_EXOTIC_P2, EXOTIC },
    { RES_EXOTIC_S1, EXOTIC },
    { RES_EXOTIC_S2, EXOTIC },
    { RES_RESTRICTED_P, RESTRICTED },
    { RES_RESTRICTED_S, RESTRICTED },
};

const Resource *resources() {
    return resources_list;
}

const Resource *resource(enum ResourceType type) {
    for(int i = 0; i < GAME_RESOURCE_TYPES; i++) {
        if(resources_list[i].type == type) {
            return &resources_list[i];
        }
    }
    return NULL;
}