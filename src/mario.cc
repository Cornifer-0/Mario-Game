#include "mario.hh"
#include <assets/animation_data.hh>
#include <components/particleSystem.hh>
#include <core/utils.hh>
#include <iostream>
#include <random>
#include <vector>
#include "components/animator.hh"
#include "components/boxCollider.hh"
#include "graphics/window.hh"
#include "world/platform.hh"

using namespace std;
using namespace pro2;

// clang-format off
// const vector<vector<int>> Mario::mario_sprite_normal_ = {
//     {_, _, _, r, r, r, r, r, _, _, _, _},
//     {_, _, r, r, r, r, r, r, r, r, r, _},
//     {_, _, h,s h, h, s, s, h, s, _, _, _},
//     {_, h, s, h, s, s, s, h, s, s, s, _},
//     {_, h, s, h, h, s, s, s, h, s, s, s},
//     {_, h, h, s, s, s, s, h, h, h, h, _},
//     {_, _, _, s, s, s, s, s, s, s, _, _},
//     {_, _, r, r, b, r, r, r, _, _, _, _},
//     {_, r, r, r, b, r, r, b, r, r, r, _},
//     {r, r, r, r, b, b, b, b, r, r, r, r},
//     {g, g, r, b, y, b, b, y, b, r, g, g},
//     {g, g, g, b, b, b, b, b, b, g, g, g},
//     {g, g, b, b, b, b, b, b, b, b, g, g},
//     {_, _, b, b, b, _, _, b, b, b, _, _},
//     {_, w, w, w, _, _, _, _, w, w, w, _},
//     {w, w, w, w, _, _, _, _, w, w, w, w},
// };
// clang-format on

Mario::Mario(pro2::Pt pos, pro2::ControlsSetUp controls, bool mainCharacter) {
    Mario("mario", pos, controls, mainCharacter);
}

Mario::Mario(std::string name, pro2::Pt pos, ControlsSetUp controls, bool mainCharacter)
    : pos_(pos),
      spawn_(pos),
      last_pos_(pos),
      animator_(new Anim::Animator),
      boxCollider_(new BoxCollider),
      mainCharacter_(mainCharacter) {
    // cout << "start" << endl;
    if (name == "mario") {
        animator_->addAnimation("idle", Anim::Animation(AnimData::mario_idle, false, 100));
        animator_->addAnimation("run", Anim::Animation(AnimData::mario_run));
        animator_->addAnimation("jump", Anim::Animation(AnimData::mario_jump, true, 3));
        animator_->addAnimation("spin_jump", Anim::Animation(AnimData::mario_spin_jump));
        animator_->addAnimation("run_fast", Anim::Animation(AnimData::mario_run_fast));
        animator_->addAnimation("dance", Anim::Animation(AnimData::mario_dance));
        animator_->addAnimation("crouch", Anim::Animation(AnimData::mario_crouch, false, 4));
        animator_->addAnimation("crouch_walk", Anim::Animation(AnimData::mario_crouch_walk));
        animator_->addAnimation("death", Anim::Animation(AnimData::mario_death));
    } else if (name == "luigi") {
        animator_->addAnimation("idle", Anim::Animation(AnimData::luigi_idle, false, 100));
        animator_->addAnimation("run", Anim::Animation(AnimData::luigi_run));
        animator_->addAnimation("jump", Anim::Animation(AnimData::mario_jump, true, 3));
        animator_->addAnimation("spin_jump", Anim::Animation(AnimData::luigi_spin_jump));
        animator_->addAnimation("run_fast", Anim::Animation(AnimData::luigi_run_fast));
        animator_->addAnimation("dance", Anim::Animation(AnimData::luigi_dance));
        animator_->addAnimation("crouch", Anim::Animation(AnimData::luigi_crouch, false, 4));
        animator_->addAnimation("crouch_walk", Anim::Animation(AnimData::luigi_crouch_walk));
        animator_->addAnimation("death", Anim::Animation(AnimData::luigi_death));
    }

    controls_ = controls;
    // for the box collider
    const Frame animFrame = animator_->currentFrame();
    boxCollider_->update(pos_, animFrame);
}

void Mario::paint(pro2::Window& window, bool debug) {
    // Frame    animFrame = animator_->nextFrame();
    // const Pt top_left = {pos_.x - static_cast<int>(animFrame[0].size() / 2),
    //                      pos_.y - static_cast<int>(animFrame.size() / 2)};
    // paint_sprite(window, top_left, animFrame, looking_left_);

    paint_sprite_center(window, pos_, animator_->nextFrame(), looking_left_);

    if (debug) {
        boxCollider_->printDebugLines(window);
        window.set_pixel(pos_, red);
    }
}

void Mario::apply_physics_() {
    if (grounded_) {
        speed_.y = 0;
        accel_.y = 0;
    }

    // Always falling to check if we aren't grounded
    // If we are, we will return to the same spot

    const int gravity = 1;  // gravity = 1 pixel / frame_time^2
    speed_.y += gravity;

    if (accel_time_ > 0) {
        speed_.y += accel_.y;
        accel_time_--;
    }

    pos_.x += speed_.x;
    pos_.y += speed_.y;
}

void Mario::jump() {
    if (dead_) {
        return;
    }
    animator_->changeAnimationReset("spin_jump");

    speed_.y = 0;
    accel_.y = -8;
    grounded_ = false;
    accel_time_ = 2;
}

void Mario::update(pro2::Window& window, const set<Platform *>& platforms) {
    if (dead_) {
        if (!mainCharacter_) {
            return;
        }
        if (respawn_timer_ >= FRAMES_TO_RESPAWN) {
            respawn();
            respawn_timer_ = 0;
        } else {
            respawn_timer_++;
        }

        return;
    }

    if (invulnarability_frames_ > 0) {
        invulnarability_frames_--;
    }
    // cout << "mario updating " << endl;
    last_pos_ = pos_;
    if (window.is_key_down(controls_.jump_) && grounded_) {
        jump();
    }

    // Velocitat horitzontal
    speed_.x = 0;
    if (window.is_key_down(controls_.left_) || window.is_key_down(controls_.right_)) {
        int  direction = window.is_key_down(controls_.left_) ? -1 : 1;
        bool isCrouching = window.is_key_down(controls_.crouch_);
        bool isRunningFast = window.is_key_down(controls_.sprint_);

        if (isCrouching) {
            speed_.x = direction;
            if (grounded_) {
                animator_->changeAnimation("crouch_walk");
            }
        } else if (isRunningFast) {
            speed_.x = direction * 4;
            if (grounded_) {
                animator_->changeAnimation("run_fast");
                if (window.frame_count() % (rand() % 10 + 3) == 0) {
                    Frame f = animator_->currentFrame();
                    int   width = f[0].size();
                    int   height = f.size();
                    Pt    posToSpawnTrail = pos_ + Pt{0, height / 2 - 2};
                    if (looking_left_) {
                        posToSpawnTrail = posToSpawnTrail - Pt{width / 2, 0};
                    } else {
                        posToSpawnTrail = posToSpawnTrail + Pt{width / 2, 0};
                    }
                    ParticleSystemManager::getInstance().spawn(ParticleSystem(
                        posToSpawnTrail,
                        Anim::Animation(AnimData::dust_trail, false, (rand() % 7) + 2)));
                }
            }
        } else {
            speed_.x = direction * 2;
            if (grounded_) {
                animator_->changeAnimation("run");
            }
        }
    } else if (grounded_) {
        if (window.is_key_down(controls_.crouch_)) {
            animator_->changeAnimation("crouch");
        } else if (window.is_key_down(controls_.dance_)) {
            animator_->changeAnimation("dance");
            // cout << "Time to dance !!!" << endl;
        } else if (animator_->getAnimation() != "dance") {
            if (!(animator_->getAnimation() == "idle")) {
                animator_->changeAnimationReset("idle");
            }
        }
    }

    if (speed_.x != 0) {
        looking_left_ = speed_.x < 0;
    }

    // Apply acceleration and speed
    apply_physics_();

    if (pos_.y >= 500) {
        die();
    }

    // Check position
    was_grounded_ = grounded_;
    set_grounded(false);

    const int height = static_cast<int>(animator_->currentFrame().size());
    const int width = static_cast<int>(animator_->currentFrame()[0].size());

    for (const Platform *platform : platforms) {
        if (platform->has_crossed_floor_downwards({last_pos_.x, last_pos_.y + height / 2 - 2},
                                                  {pos_.x, pos_.y + height / 2 - 2})) {
            set_grounded(true);
            set_y(platform->top() - height / 2 + 1);
        }

        if (platform->has_crossed_block_sideways({last_pos_.x - width / 2 - 2, last_pos_.y},
                                                 {pos_.x - width / 2 - 2, pos_.y}) ||
            platform->has_crossed_block_sideways({last_pos_.x + width / 2 + 2, last_pos_.y},
                                                 {pos_.x + width / 2 + 2, pos_.y})) {
            // cout << "mario side blocked " << endl;

            if (platform->get_rect().left < pos_.x && looking_left_) {
                pos_.x = platform->get_rect().right + width / 2;

            } else if (platform->get_rect().right > pos_.x && !looking_left_) {
                pos_.x = platform->get_rect().left - width / 2;
            }
            // set_x_accel(0);
        }
    }

    if (grounded_ && !was_grounded_ && !window.is_key_down(controls_.crouch_)) {
        // so just hit the ground
        Frame f = animator_->currentFrame();
        int   width = f[0].size();
        int   height = f.size();
        Pt    posToSpawnTrail = pos_ + Pt{-(width / 2), height / 2 - 2};
        ParticleSystemManager::getInstance().spawn(
            ParticleSystem(posToSpawnTrail, Anim::Animation(AnimData::dust_trail, false, 5)));
        posToSpawnTrail = pos_ + Pt{width / 2, height / 2 - 2};
        ParticleSystemManager::getInstance().spawn(
            ParticleSystem(posToSpawnTrail, Anim::Animation(AnimData::dust_trail, false, 5)));
    }

    // cout << "updating collider..." << endl;
    updateCollider();
}

void Mario::updateCollider() {
    // update the boxCollider
    const Frame animFrame = animator_->currentFrame();
    // cout << animFrame.size() << ' ' << animFrame[0].size() << endl;
    boxCollider_->update(pos_, animFrame);
}

BoxCollider *Mario::getCollider() const {
    return boxCollider_;
}

void Mario::toggle_grounded() {
    set_grounded(!grounded_);
}

void Mario::set_grounded(bool grounded) {
    grounded_ = grounded;
    if (grounded_) {
        speed_.y = 0;
    }
}

bool Mario::is_grounded() const {
    return grounded_;
}

void Mario::set_y(int y) {
    pos_.y = y;
}

void Mario::set_x(int x) {
    pos_.x = x;
}

bool Mario::lookingLeft() const {
    return looking_left_;
}

pro2::Pt Mario::pos() const {
    return pos_;
}

pro2::Pt Mario::last_pos() const {
    return last_pos_;
}

bool Mario::is_dead() const {
    return dead_;
}

void Mario::die() {
    dead_ = true;
    animator_->changeAnimation("death");
}

bool Mario::invulnarable() const {
    return invulnarability_frames_ > 0;
}

void Mario::setInvulnarabilityFrames(int k) {
    invulnarability_frames_ = k;
}

void Mario::set_y_accel(int k) {
    accel_.y = k;
    speed_.y = k;
}

void Mario::set_x_accel(int k) {
    accel_.x = k;
    speed_.y = k;
}

bool Mario::is_falling() const {
    return last_pos_.y < pos_.y;
}

void Mario::changeResapawn(pro2::Pt new_spawn) {
    spawn_ = new_spawn;
}

void Mario::respawn() {
    animator_->changeAnimation("idle");
    pos_ = last_pos_ = spawn_;
    speed_ = accel_ = {0, 0};
    dead_ = false;

    ParticleSystemManager::getInstance().spawn(
        ParticleSystem(pos_, Anim::Animation(AnimData::blue_shine_2_, false, 2)));
}

pro2::Pt Mario::respawn_point() const {
    return spawn_;
}
