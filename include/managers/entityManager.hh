/**
 * @file goombasManager.hh
 * @brief
 * @version 0.1
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef GOOMBASMANAGER_HH
#define GOOMBASMANAGER_HH

#include <enemies/entity.hh>
#include <set>
#include <vector>

typedef std::vector<std::pair<pro2::Pt, EntityType>> EntityMap;

// Forward declaration of boxCollider class
class Mario;

namespace pro2 {
class Window;
}

class Block;
class HUD;

/**
 * @brief Class GoombasManager manages the goombas in the scene. Basically a container for
 * goombas that makes it easy to, spawn and kill goombas aswell as updating them.
 */
class EntityManager {
 private:
    EntityMap           entityMap_;
    std::vector<Entity> entities_;

    int killed_;

 public:
    /**
     * @brief Construct a new GoombasManager with the defaultGoombaMap.
     *
     */
    EntityManager();

    /**
     * @brief Construct a new GoombasManager with a custom map.
     *
     * @param gm
     */
    EntityManager(EntityMap gm);

    /**
     * @brief Get the killed_ attribute. (How many goombas have been killed)
     *
     * @return int
     */
    int getKilled() const;

    /**
     * @brief Updates the goomba, takes into account a Mairo object
     * If The Goomba collides with mario collider, it dies and makes the mario jump.
     *
     * @param mario
     * @param platforms
     */
    void update(Mario *mario, HUD *mushroom_hud);

    /**
     * @brief Kills if in the collider.
     *
     * @param boxCollider
     */
    void killIfInThisCollider(BoxCollider *boxCollider);

    /**
     * @brief Updates the goomba, takes into account he platforms
     * If it collides
     * with a platform it turns, and if it falls through a platform it's pos.y is set to the
     * platform top().
     *
     * @param mario
     * @param platforms
     */
    void check_collisions(const std::vector<Platform *>& platforms,
                          const std::set<Block *>&       blocks);

    /**
     * @brief Respawn all goombas using the goombasMap_, makes current goombas disapear
     */
    void respawnEntities();

    /**
     * @brief Calls paint to all goombas in the container goombas_.
     *
     * @param window
     * @param debug
     */
    void paint(pro2::Window& window, bool debug);

    /**
     * @brief Spawns an entity of type at a pos.
     *
     * @param pos
     * @param type
     */
    void spawnEntity(pro2::Pt pos, EntityType type);

    //  private:
    //     const EntityMap defaultGoombaMap = {
    //         {{75, 225}, GOOMBA},  {{125, 225}, GOOMBA}, {{175, 225}, GOOMBA}, {{225, 225},
    //         GOOMBA},
    //         {{275, 225}, GOOMBA}, {{325, 225}, GOOMBA}, {{375, 225}, GOOMBA}, {{425, 225},
    //         GOOMBA},

    //         {{75, 165}, ANGRY},   {{125, 165}, ANGRY},  {{375, 165}, GOOMBA}, {{425, 165},
    //         GOOMBA},

    //         {{75, 100}, KOOPA},   {{425, 100}, KOOPA}

    //     };

    //     const EntityMap entrega2 = {
    //         {{75, 225}, ANGRY},   {{125, 225}, GOOMBA}, {{175, 225}, GOOMBA},
    //         {{225, 225}, GOOMBA}, {{100, 225}, KOOPA},

    //     };

    //     const EntityMap juditMap = {
    //         // {{790, 250}, GOOMBA},
    //         //                           {{820, 250}, KOOPA},
    //         //                           {{1020, 250}, GOOMBA},
    //         //                           {{4500, 250}, ANGRY},
    //         //                           {{5944, 250}, ANGRY}

    //     };
};

#endif