/**
 * @file entity.hh
 * @brief
 * @version 0.1
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef GOOMBA_HH
#define GOOMBA_HH

#include <set>
#include <vector>
#include "core/geometry.hh"

// Forward declarations
namespace Anim {
class Animator;
class Animation;
}  // namespace Anim

namespace pro2 {
class Window;
}  // namespace pro2

class BoxCollider;
class Platform;
class Block;

// TODO: More types of goomba
enum EntityType {
    GOOMBA,
    ANGRY,
    // SAD,
    // CAVE,
    // SPIKED,
    // WINGED,
    // MARIOFAN,
    // STAR,
    MUSHROOM,

    KOOPA,
};

/**
 * @brief Class of the Enemy Goomba
 *
 */
class Entity {
 private:
    pro2::Pt pos_, last_pos_;
    pro2::Pt speed_ = {0, 0};
    pro2::Pt accel_ = {0, 0};
    int      accel_time_ = 0;

    int velocity_ = 1;

    Anim::Animator *animator_;

    BoxCollider *bodyCollider_;
    BoxCollider *headCollider_;
    BoxCollider *visibilityCollider_;

    EntityType type_;

    bool grounded_;
    bool looking_left_;
    bool dead;

    bool mario_in_sight_ = false;

    /**
     * @brief Applies normal physics to the goomba, just as the mario
     *
     */
    void apply_physics();

    /**
     * @brief Updates the horintal movement of the goomba, making it walk.
     *
     */
    void update_horizontal_movement();

    /**
     * @brief Updates the collider of the goomba.
     *
     */
    void update_collider();

 public:
    /**
     * @brief Default Constructor of goomba, not safe.
     */
    Entity();

    /**
     * @brief Construct a Goomba at a position type NORMAL is assumed
     *
     */
    Entity(pro2::Pt);

    /**
     * @brief Construct a new Goomba object at a position with a custom type .
     *
     * @param tp
     */
    Entity(pro2::Pt, EntityType tp);

    /**
     * @brief Returns a pointer to the headCollider attribute of the goomba.
     *
     * @return boxCollider*
     */
    BoxCollider *getHeadCollider() const;

    /**
     * @brief Returns true if looking left.
     *
     * @return true
     * @return false
     */
    bool looking_left() const;

    /**
     * @brief Get the Type object
     *
     * @return goombaType
     */
    EntityType getType() const;

    /**
     * @brief Returns a pointer to the bodyCollider attribute of the goomba.
     *
     * @return boxCollider*
     */
    BoxCollider *getBodyCollider() const;

    /**
     * @brief Get the Visibility Collider object
     *
     * @return boxCollider*
     */
    BoxCollider *getVisibilityCollider() const;

    /**
     * @brief Spawns the death particle of the goomba at it's position
     */
    void die();

    /**
     * @brief Returns the position of the goomba.
     *
     * @return pro2::Pt
     */
    pro2::Pt pos() const;

    /**
     * @brief Checks for collisions with platforms, makes the goomba turn if it has collided with a
     * platform horizontally, sets y to the platform top if it has fallen inside a platform.
     *
     * @param platforms
     */
    void check_collisions(const std::vector<Platform *>  platforms,
                          const std::set<class Block *>& blocks);

    /**
     * @brief Updates the goomba, calling  update_horizontal_movement and
     * update_collider. Aswell as updating last_pos_
     */
    void update();

    /**
     * @brief Makes the goomba change direction by flipping the sign of the speed.x and setting
     * looking_left_ to !looking_left_
     */
    void changeDirection();

    /**
     * @brief Set the mario in sight object
     *
     * @param b
     */
    void set_mario_in_sight(bool b);

    /**
     * @brief Paints the goomba on the window, if debug is set to true, it calls for the debug of
     * the boxCollider.
     *
     * @param window
     * @param debug
     */
    void paint(pro2::Window& window, bool debug);

    pro2::Rect get_rect() const;
};

#endif