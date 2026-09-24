#include <assets/animation_data.hh>
#include <components/animator.hh>
#include <components/boxCollider.hh>
#include <core/utils.hh>
#include <enemies/koopaShell.hh>
#include <graphics/window.hh>
#include <iostream>
#include "world/platform.hh"

using namespace std;
using namespace pro2;
using namespace Anim;

KoopaShell::KoopaShell(Pt pos) {
    pos_ = pos;
    last_pos_ = pos;
    animator_ = new Animator(Animation(AnimData::koopa_shell_idle));
    animator_->addAnimation("stop", Animation(AnimData::koopa_shell_idle, false));
    topCollider_ = new BoxCollider();
    looking_left_ = true;
    stop_ = false;
}

KoopaShell::KoopaShell(Pt pos, Animation anim) {
    pos_ = pos;
    last_pos_ = pos;
    animator_ = new Animator(anim);
    topCollider_ = new BoxCollider();
    looking_left_ = true;
    stop_ = false;
}

void KoopaShell::update(const std::vector<Platform *>& platforms) {
    last_pos_ = pos_;
    // cout << "update" << endl;
    // Apply acceleration and speed
    update_horizontal_movement();
    apply_physics();

    // Check position
    grounded_ = false;
    const int height = static_cast<int>(animator_->currentFrame().size());
    for (const Platform *platform : platforms) {
        if (platform->has_crossed_floor_downwards({last_pos_.x, last_pos_.y + height / 2 - 2},
                                                  {pos_.x, pos_.y + height / 2 - 2})) {
            grounded_ = true;
            pos_.y = (platform->top() - height / 2 + 1);
        }
    }

    const Frame curr = animator_->currentFrame();
    const int   width = static_cast<int>(curr[0].size());

    for (const Platform *platform : platforms) {
        if ((platform->has_crossed_platform(last_pos_ - Pt{width / 2, 0},
                                            pos_ - Pt{width / 2, 0}) &&
             looking_left_) ||
            (platform->has_crossed_platform(last_pos_ + Pt{width / 2, 0},
                                            pos_ + Pt{width / 2, 0}) &&
             !looking_left_)) {
            // cout << "corrseed" << endl;
            changeDirection();
            // cout << "collided looking left: " << looking_left_ << " speed: " << speed_.x << endl;
        }
    }

    update_collider();
    // cout << "update end" << endl;
}

void KoopaShell::changeDirection() {
    looking_left_ = !looking_left_;
}

void KoopaShell::update_collider() {
    // cout << pos_.x << ' ' << pos_.y << endl;
    // cout << animator_->currentFrame().size() << endl;
    // cout << animator_->currentFrame()[0].size() << endl;

    topCollider_->update(pos_, animator_->currentFrame());
}

void KoopaShell::update_horizontal_movement() {
    if (stop_) {
        return;
    }
    int direction = (looking_left_) ? -velocity_ : velocity_;
    if (grounded_) {
        speed_.x = direction;
    }
}

void KoopaShell::apply_physics() {
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

    if (!stop_) {
        pos_.x += speed_.x;
    }
    pos_.y += speed_.y;
}

void KoopaShell::paint(pro2::Window& window, bool debug) {
    // cout << "to paint in koopashell" << endl;
    paint_sprite_center(window, pos_, animator_->nextFrame(), looking_left_);
    // cout << "painted" << endl;
    if (debug) {
        topCollider_->printDebugLines(window);
    }
}

BoxCollider *KoopaShell::getCollider() const {
    return topCollider_;
}

void KoopaShell::stop() {
    // cout << "stoped " << &(*this) << endl;

    stop_ = !stop_;
    if (stop_) {
        animator_->changeAnimationReset("stop");

    } else {
        animator_->changeAnimationReset("idle");
    }
}

bool KoopaShell::hasStoped() const {
    return stop_;
}
