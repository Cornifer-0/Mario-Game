/**
 * @file collectable.hh
 * @brief
 * @version 0.1
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef COLLECTABLE_HH
#define COLLECTABLE_HH

#include <core/geometry.hh>

// Forward declarations

class BoxCollider;

namespace Anim {
class Animator;
}

namespace pro2 {
class Window;
}

enum COLLECTABLE_TYPE {
    SMALL_COIN,
    COIN,
    END_STAR,
    //  PIG, //TODO
};

/**
 * @brief Class for a collectable, an object that sits at a spot in the world and is waiting to be
 * picked up by the player.
 */
class Collectable {
 private:
    pro2::Pt         pos_, last_pos_;
    pro2::Pt         speed_ = {0, 0};
    pro2::Pt         accel_ = {0, 0};
    COLLECTABLE_TYPE type_;

    BoxCollider *boxCollider_;

    Anim::Animator *animator_;

    bool collected_;

 public:
    /**
     * @brief Construct a new Collectable object at a position pos. It assumes COIN
     *
     * @param pos
     */
    Collectable(pro2::Pt pos);

    /**
     * @brief Construct a new Collectable object at a position pos. With a type ct.
     *
     * @param pos
     * @param ct
     */
    Collectable(pro2::Pt pos, COLLECTABLE_TYPE ct);

    /**
     * @brief Get the boxCollider object
     *
     * @return boxCollider*
     */
    BoxCollider *getCollider() const;

    /**
     * @brief Get the rect object
     *
     * @return pro2::Rect
     */
    pro2::Rect get_rect();

    /**
     * @brief Get the Type object
     *
     * @return COLLECTABLE_TYPE
     */
    COLLECTABLE_TYPE getType() const;

    /**
     * @brief Paints the collectable on the window, if debug it also paints the boxCollider's border
     * by calling the paint method on the boxCollider class.
     *
     * @param window
     * @param debug
     */
    void paint(pro2::Window& window, bool debug);

    /**
     * @brief Spawns a collected animation at pos_. Does not actually destroy itself.
     */
    void Destroy();
    bool isDestroyed() const;

 private:
    /**
     * @brief Calculates an offset, based on the frame_count, useful to simulate sinusoidal
     * movements of the coins.
     *
     * @param window
     * @return pro2::Pt
     */
    pro2::Pt calcPositionOffset(const pro2::Window& window) const;
};

#endif