/**
 * @file koopa.hh
 * @brief
 * @version 0.1
 * @date 2025-05-14
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef KOOPASHELL_HH
#define KOOPASHELL_HH

#include <core/geometry.hh>
#include <vector>

class BoxCollider;

class Platform;

namespace pro2 {
class Window;
}

namespace Anim {
class Animator;
class Animation;
}  // namespace Anim

class KoopaShell {
 private:
    pro2::Pt pos_, last_pos_;
    pro2::Pt speed_ = {0, 0};
    pro2::Pt accel_ = {0, 0};
    int      accel_time_ = 0;
    bool     grounded_;
    bool     looking_left_;
    bool     stop_;

    int velocity_ = 4;

    BoxCollider *bottomCollider_;
    BoxCollider *topCollider_;

    Anim::Animator *animator_;

    void apply_physics();
    void update_collider();
    void update_horizontal_movement();

 public:
    /**
     * @brief Construct a new Koopa Shell object
     *
     */
    KoopaShell(pro2::Pt);

    /**
     * @brief Construct a new Koopa Shell object
     *
     */
    KoopaShell(pro2::Pt, Anim::Animation);

    /**
     * @brief Get the Collider object
     *
     * @return boxCollider*
     */
    BoxCollider *getCollider() const;

    /**
     * @brief Changes the sign of the velocity of the koopashell.
     */
    void changeDirection();

    /**
     * @brief Makes the koopashell stop, if it has already stoped it starts moving again.
     *
     */
    void stop();

    /**
     * @brief Returns true if the shell is stopped.
     *
     * @return true
     * @return false
     */
    bool hasStoped() const;

    /**
     * @brief Updates the koopshell with all the platforms
     *
     * @param platforms
     */
    void update(const std::vector<Platform *>& platforms);

    /**
     * @brief Paints the koopaShells on the window
     *
     * @param window
     * @param debug
     */
    void paint(pro2::Window& window, bool debug);
};

#endif