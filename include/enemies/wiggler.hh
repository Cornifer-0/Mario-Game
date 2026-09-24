/**
 * @file wiggler.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-06-15
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef WIGGLER_HH
#define WIGGLER_HH

#include <vector>
#include "core/geometry.hh"

class Platform;
class BoxCollider;
class Mario;

namespace pro2 {
class Window;
}

namespace Anim {
class Animator;
}

/**
 * @brief Class for the Wiggler enemy *** In a linked list style ***
 *       Interesting recursive functions for updating, painting and managing the Wiggler.
 */
class Wiggler {
 private:
    struct bodyPart {
        pro2::Pt  pos, last_pos;
        bodyPart *next = nullptr;
        bodyPart *prev = nullptr;

        BoxCollider    *boxCollider_;
        Anim::Animator *bodyAnimator_;
        Anim::Animator *legAnimator_;

        int upOffset = 0;

        bool grounded_ = false;
        bool looking_left_ = false;
        int  accel_time_ = 0;

        pro2::Pt accel_ = {0, 0};
        pro2::Pt speed_ = {0, 0};

        bodyPart() {}

        bodyPart(pro2::Pt _pos) : pos(_pos) {}
    };

    bodyPart *head_;
    bodyPart *tail_;
    bool      angry_ = false;
    bool      dead_ = false;

    int   amplitude_ = 5;
    float frequency_ = 30.0;

    int velocity_ = 1;

    void updateCollider();
    void spawnEnd();
    void update_physics_rec(bodyPart *bp);
    void update_horizontal_movement_rec(bodyPart *bp);
    void mario_update(Mario                  *mario,
                      Mario                  *luigi,
                      std::vector<Wiggler *>& toRemove,
                      std::vector<Wiggler *>& toAdd);
    void mario_update_rec(Mario                  *mario,
                          bodyPart               *bp,
                          std::vector<Wiggler *>& toRemove,
                          std::vector<Wiggler *>& toAdd);
    void check_collisions_rec(const std::vector<Platform *>& platforms, bodyPart *bp);
    void paint_rec(pro2::Window& window, bool debug, bodyPart *bp);
    void turnAngry();
    void die();
    void die_rec(bodyPart *bp);

 public:
    /**
     * @brief Construct a new Wiggler object
     *
     */
    Wiggler();

    /**
     * @brief Construct a new Wiggler object with a position and a length (number of body parts)
     *
     * @param pos
     * @param length
     */
    Wiggler(pro2::Pt pos, int length);

    /**
     * @brief Construct a new Wiggler object
     *
     * @param bp
     */
    Wiggler(bodyPart *bp);

    /**
     * @brief Updates the wiggler, using the recursive functions
     *
     * @param mario
     * @param luigi
     * @param platforms
     * @param toRemove
     * @param toAdd
     */
    void update(Mario                         *mario,
                Mario                         *luigi,
                const std::vector<Platform *>& platforms,
                std::vector<Wiggler *>&        toRemove,
                std::vector<Wiggler *>&        toAdd);

    /**
     * @brief
     *
     * @param window
     * @param debug
     */
    void paint(pro2::Window& window, bool debug);

 private:
    void update_horizontal_movement();
    void changeDirection(bodyPart *bp);
    void update_physics();
    void check_collisions(const std::vector<Platform *>& platforms);
};

#endif