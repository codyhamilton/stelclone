#include <stdint.h>
#include "game_types.h"

/**
 * Resource type rarity
 * 
 * The rarity of the resource type.
 * Will affect the chance of the resource being found in the asteroid.
 */
enum ResourceTypeRarity {
    FRONTIER,
    EXOTIC,
    RESTRICTED
};

typedef struct {
    enum ResourceType type;
    enum ResourceTypeRarity rarity;
} ResourceTypeDef;

typedef struct {
    const ResourceTypeDef *items;
    const uint8_t count;
} ResourceTypeDefList;

const ResourceTypeDef *resource_type(enum ResourceType type);

const ResourceTypeDefList *resource_types();