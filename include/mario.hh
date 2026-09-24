/**
 * @file mario.hh
 * @brief
 * @version 0.1
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef MARIO_HH
#define MARIO_HH

#include <core/geometry.hh>
#include <core/utils.hh>
#include <set>
#include <vector>

class Platform;

namespace pro2 {
class Window;
}

namespace Anim {
class Animator;
}

class BoxCollider;

class Mario {
 private:
    pro2::Pt pos_, last_pos_;
    pro2::Pt speed_ = {0, 0};
    pro2::Pt accel_ = {0, 0};
    pro2::Pt spawn_ = {0, 0};
    int      accel_time_ = 0;

    Anim::Animator *animator_;

    BoxCollider *boxCollider_;

    pro2::ControlsSetUp controls_;

    bool mainCharacter_;
    bool grounded_ = false;
    bool was_grounded_ = false;
    bool looking_left_ = false;

    const int FRAMES_TO_RESPAWN = 50;
    int       respawn_timer_ = 0;

    int invulnarability_frames_ = 0;

    bool dead_ = false;

    void apply_physics_();
    void updateCollider();

 public:
    Mario(std::string name, pro2::Pt pos, pro2::ControlsSetUp controls, bool mainCharacter);

    Mario(pro2::Pt pos, pro2::ControlsSetUp controls, bool mainCharacter);

    void paint(pro2::Window& window, bool debug);

    pro2::Pt pos() const;
    pro2::Pt last_pos() const;
    pro2::Pt respawn_point() const;

    void set_y(int y);
    void set_x(int x);

    bool is_grounded() const;
    bool is_falling() const;
    bool lookingLeft() const;
    bool is_dead() const;

    void set_grounded(bool grounded);

    void set_y_accel(int);
    void set_x_accel(int);

    void toggle_grounded();

    void changeResapawn(pro2::Pt new_spawn);

    void jump();
    void respawn();
    void die();

    bool invulnarable() const;
    void setInvulnarabilityFrames(int k);

    void update(pro2::Window& window, const std::set<Platform *>& platforms);

    BoxCollider *getCollider() const;

    //  private:
    //     static const std::vector<std::vector<int>> mario_sprite_normal_;
};

#endif