#include <stdbool.h>
#include <stdint.h>
#include "game_types.h"

/**
 * Number of resource types
 * 
 * The number of resource types in the game.
 */
#define GAME_RESOURCE_TYPES 10

/**
 * Resource type

 * C: Common
 * A: Alternative
 * P: Primary
 * S: Secondary
 */
enum ResourceType : uint8_t {
    RES_FRONTIER_C1,
    RES_FRONTIER_C2,
    RES_FRONTIER_A1,
    RES_FRONTIER_A2,
    RES_EXOTIC_P1,
    RES_EXOTIC_P2,
    RES_EXOTIC_S1,
    RES_EXOTIC_S2,
    RES_RESTRICTED_P, 
    RES_RESTRICTED_S, 
};

/**
 * Resource type rarity
 * 
 * The rarity of the resource type.
 * Will affect the chance of the resource being found in the asteroid.
 */
enum ResourceRarity : uint8_t {
    FRONTIER,
    EXOTIC,
    RESTRICTED
};

/**
 * Resource
 * 
 * A resource in the game.
 */
typedef struct {
    enum ResourceType type;
    enum ResourceRarity rarity;
} Resource;

/**
 * Resource deposit
 * 
 * A resource deposit on an asteroid.
 * 
 * Represents a density quantity of a resoure on an asteroid.
 */
typedef uint8_t ResourceDeposit;

/**
 * Get a resource by type
 * 
 * @param type The type of the resource to get
 * @return The resource
 */
const Resource *resource(enum ResourceType type);

/**
 * Get all resources
 * 
 * @return A list of all resources
 */
const Resource *resources();