#include "enemies/wiggler.hh"
#include <algorithm>
#include <cmath>
#include "assets/animation_data.hh"
#include "components/animator.hh"
#include "components/boxCollider.hh"
#include "components/particleSystem.hh"
#include "core/utils.hh"
#include "graphics/window.hh"
#include "mario.hh"
#include "world/platform.hh"

using namespace std;
using namespace pro2;
using namespace Anim;

Wiggler::Wiggler() {}

Wiggler::Wiggler(pro2::Pt pos, int length) {
    head_ = new bodyPart(pos);
    tail_ = new bodyPart(pos);
    head_->prev = tail_;
    tail_->next = head_;

    head_->bodyAnimator_ = new Animator(Animation(AnimData::wiggler_head));
    head_->bodyAnimator_->addAnimation("angry", Animation(AnimData::wiggler_head_angry));
    head_->legAnimator_ = new Animator(Animation({AnimData::wiggler_legs[0]}, true, 10));
    head_->legAnimator_->addAnimation("step", Animation({AnimData::wiggler_legs[1]}, true, 10));

    head_->boxCollider_ = new BoxCollider();

    amplitude_ = rand() % 9 + 5;                                // random number between 5 and 15
    frequency_ =
        static_cast<float>(rand()) / RAND_MAX * 40.0f + 10.0f;  // random float between 10 and 50

    for (int i = 0; i < length; i++) {
        spawnEnd();
    }
}

Wiggler::Wiggler(bodyPart *bp) {
    // cout << "starting to create new one" << endl;
    head_ = bp;
    head_->next = nullptr;

    head_->bodyAnimator_ = new Animator(Animation(AnimData::wiggler_head));
    head_->bodyAnimator_->addAnimation("angry", Animation(AnimData::wiggler_head_angry));
    head_->legAnimator_ = new Animator(Animation({AnimData::wiggler_legs[0]}, true, 10));
    head_->legAnimator_->addAnimation("step", Animation({AnimData::wiggler_legs[1]}, true, 10));

    head_->boxCollider_ = new BoxCollider();

    bodyPart *it = head_;
    int       legnth = 0;
    while (it->prev) {
        it = it->prev;
        legnth++;
    }
    tail_ = it;
    // cout << "end of creation with length of " << legnth << endl;
}

void Wiggler::spawnEnd() {
    // cout << 'a' << endl;
    bodyPart *newBodyPart = new bodyPart(tail_->pos + Pt{10, 0});
    tail_->pos = tail_->pos + Pt{10, 10};

    int frameCount = static_cast<int>(AnimData::wiggler_body_ball.size());
    int frameIndex = rand() % frameCount;
    newBodyPart->bodyAnimator_ =
        new Animator(Animation({AnimData::wiggler_body_ball[frameIndex]}, true, 1));
    newBodyPart->bodyAnimator_->addAnimation(
        "angry", Animation({AnimData::wiggler_body_ball_angry[frameIndex]}, true, 1));
    newBodyPart->legAnimator_ = new Animator(Animation({AnimData::wiggler_legs[0]}, true, 10));
    newBodyPart->legAnimator_->addAnimation("step",
                                            Animation({AnimData::wiggler_legs[1]}, true, 10));

    newBodyPart->boxCollider_ = new BoxCollider();
    newBodyPart->boxCollider_->update(newBodyPart->pos, newBodyPart->bodyAnimator_->currentFrame());

    newBodyPart->looking_left_ = tail_->next->looking_left_;
    newBodyPart->upOffset = tail_->next->upOffset + 10;

    newBodyPart->prev = tail_;
    newBodyPart->next = tail_->next;

    tail_->next->prev = newBodyPart;
    tail_->next = newBodyPart;
    // cout << 'b' << endl;
}

void Wiggler::update(Mario                         *mario,
                     Mario                         *luigi,
                     const std::vector<Platform *>& platforms,
                     vector<Wiggler *>&             toRemove,
                     vector<Wiggler *>&             toAdd) {
    // cout << "start update" << endl;
    if (dead_) {
        return;
    }
    update_horizontal_movement();
    update_physics();
    check_collisions(platforms);
    mario_update(mario, luigi, toRemove, toAdd);
    // cout << "end update" << endl;
}

void Wiggler::paint(pro2::Window& window, bool debug) {
    if (dead_ || !head_->boxCollider_->collidingWith(window.camera_rect() + 100)) {
        return;
    }
    // cout << "start paint" << endl;
    paint_rec(window, debug, tail_->next);
    // cout << "end paint" << endl;
}

void Wiggler::paint_rec(pro2::Window& window, bool debug, bodyPart *bp) {
    if (!bp) {
        return;
    }

    // CALC sinusoidal offset
    Pt offset = Pt{0, static_cast<int>(std::abs(amplitude_ * std::sin(bp->pos.x / frequency_)))};
    if (head_->prev == tail_) {
        offset = {0, -10};
    }
    // cout << offset.y << endl;
    // cout << "painting one" << endl;
    if (bp == head_) {
        paint_sprite_center(window, bp->pos - offset - Pt{0, 10}, bp->bodyAnimator_->nextFrame(),
                            !bp->looking_left_);

    } else {
        paint_sprite_center(window, bp->pos - offset, bp->bodyAnimator_->currentFrame(),
                            bp->looking_left_);

        // if the bp->pos.x /40.0 == 0 => offset = 0 => leg is touching ground, therefore should
        // step
        // cout << offset.y << endl;

        if (offset.y <= 1) {
            bp->legAnimator_->changeAnimation("step");

            int dice = rand() % 20;
            if (dice == 5) {
                ParticleSystemManager::getInstance().spawn(
                    ParticleSystem(bp->pos + Pt{0, 11}, Animation(AnimData::dust_trail, false)));
            }

        } else {
            bp->legAnimator_->changeAnimation("idle");
        }

        paint_sprite_center(window, bp->pos + Pt{0, 7} - offset, bp->legAnimator_->nextFrame(),
                            !bp->looking_left_);
    }
    // if (angry_ && (rand() % 5) == 2) {
    //     ParticleSystemManager::getInstance().spawn(
    //         ParticleSystem(bp->pos + Pt{rand() % 30 - 10, rand() % 30 - 10},
    //                        Animation(AnimData::dust_trail, false)));
    // }

    if (debug) {
        if (bp == head_ || bp == tail_) {
            bp->boxCollider_->printDebugLines(window, 0xff0000ff);
        } else {
            bp->boxCollider_->printDebugLines(window);
        }
    }

    paint_rec(window, debug, bp->next);
}

void Wiggler::update_physics() {
    // cout << "start phyciscs update" << endl;
    update_physics_rec(head_);
    // cout << "end physics update" << endl;
}

void Wiggler::update_physics_rec(bodyPart *bp) {
    if (bp == tail_ || !bp) {
        return;
    }

    bp->last_pos = bp->pos;

    if (bp->grounded_) {
        bp->speed_.y = 0;
        bp->accel_.y = 0;
    }

    // Always falling to check if we aren't grounded
    // If we are, we will return to the same spot

    const int gravity = 2;  // gravity = 1 pixel / frame_time^2
    bp->speed_.y += gravity;

    if (bp->accel_time_ > 0) {
        bp->speed_.y += bp->accel_.y;
        bp->accel_time_--;
    }

    bp->speed_.x += bp->accel_.x;
    bp->pos.x += bp->speed_.x;
    bp->pos.y += bp->speed_.y;

    bp->boxCollider_->update(bp->pos, bp->bodyAnimator_->currentFrame());

    update_physics_rec(bp->prev);
}

void Wiggler::update_horizontal_movement() {
    // update the head

    int direction = (head_->looking_left_) ? -velocity_ : velocity_;

    if (head_->grounded_) {
        // cout << "move" << endl;
        head_->speed_.x = direction;
    }
    update_horizontal_movement_rec(head_->prev);
}

void Wiggler::update_horizontal_movement_rec(bodyPart *bp) {
    if (bp == tail_ || !bp) {
        return;
    }

    bp->upOffset += 1;
    bodyPart *nextPart = bp->next;
    const int spacing = (nextPart == head_) ? 25 : 20;
    if (abs(nextPart->pos.x - bp->pos.x) >= spacing) {
        int direction = (nextPart->pos.x >= bp->pos.x) ? velocity_ : -velocity_;
        bp->speed_.x = direction;

        bp->looking_left_ = (direction > 0) ? false : true;
    }

    update_horizontal_movement_rec(bp->prev);
}

void Wiggler::check_collisions(const std::vector<Platform *>& platforms) {
    // cout << "start collisions" << endl;

    check_collisions_rec(platforms, head_);
    // cout << "end collisions" << endl;
}

void Wiggler::check_collisions_rec(const std::vector<Platform *>& platforms, bodyPart *bp) {
    if (bp == tail_ || !bp) {
        return;
    }
    // cout << "colilsions check " << endl;
    const Frame curr = bp->bodyAnimator_->currentFrame();
    const int   height = static_cast<int>(curr.size());
    const int   width = static_cast<int>(curr[0].size());
    for (const Platform *platform : platforms) {
        if (platform->has_crossed_floor_downwards({bp->last_pos.x, bp->last_pos.y + height / 2 - 2},
                                                  {bp->pos.x, bp->pos.y + height / 2 - 2})) {
            bp->grounded_ = true;
            bp->pos.y = platform->top() - height / 2 + 1;
        }
    }
    // cout << "colilsions check1  " << endl;

    for (const Platform *platform : platforms) {
        if ((platform->has_crossed_platform(bp->last_pos - Pt{width / 2, 0},
                                            bp->pos - Pt{width / 2, 0}) &&
             bp->looking_left_) ||
            (platform->has_crossed_platform(bp->last_pos + Pt{width / 2, 0},
                                            bp->pos + Pt{width / 2, 0}) &&
             !bp->looking_left_)) {
            changeDirection(bp);
            // cout << "collided looking left: " << looking_left_ << " speed: " << speed_.x <<
            // endl;
        }
    }

    // cout << "colilsions check2 " << endl;

    check_collisions_rec(platforms, bp->prev);
}

void Wiggler::changeDirection(bodyPart *bp) {
    bp->looking_left_ = (bp->looking_left_) ? false : true;
    bp->speed_.x = -bp->speed_.x;
}

void Wiggler::mario_update(Mario             *mario,
                           Mario             *luigi,
                           vector<Wiggler *>& toRemove,
                           vector<Wiggler *>& toAdd) {
    // cout << "start update" << endl;

    if (angry_ && mario->getCollider()->collidingWith(head_->boxCollider_) &&
        !mario->is_falling()) {
        mario->die();
        return;
    }

    if (angry_ && luigi->getCollider()->collidingWith(head_->boxCollider_) &&
        !luigi->is_falling()) {
        luigi->die();
        return;
    }

    if (mario->is_falling() && !mario->is_grounded()) {
        mario_update_rec(mario, head_, toRemove, toAdd);
    }

    if (luigi->is_falling() && !luigi->is_grounded()) {
        mario_update_rec(luigi, head_, toRemove, toAdd);
    }
    // cout << "end update" << endl;
}

void Wiggler::mario_update_rec(Mario             *mario,
                               bodyPart          *bp,
                               vector<Wiggler *>& toRemove,
                               vector<Wiggler *>& toAdd) {
    if (!bp || bp == tail_) {
        // cout << 'f' << endl;

        return;
    }
    // cout << 'i' << endl;

    if (angry_ && mario->getCollider()->collidingWith(bp->boxCollider_) && !mario->is_falling()) {
        mario->die();
        return;
    }

    if (mario->getCollider()->collidingWith(bp->boxCollider_) && mario->is_falling()) {
        // if (bp == &tail_) {
        //     cout << "is tail" << endl;
        // } else if (bp == tail_.next) {
        //     cout << "is tail's next" << endl;
        // } else if (bp == tail_.prev) {
        //     cout << "tail's prev" << endl;
        // }

        bodyPart *splitHead = bp->prev;

        if (bp == head_) {
            // Remove this Wiggler  from the vector
            // auto it = find(wiggler_vector.begin(), wiggler_vector.end(), this);
            // if (it != wiggler_vector.end()) {
            //     wiggler_vector.erase(it);
            // }
            die();
            toRemove.push_back(this);
            // dead_ = true;
            mario->jump();
            return;
        }

        if (splitHead) {
            splitHead->next = nullptr;
            bp->prev = nullptr;
            // delete bp;
        }

        // bp->prev = nullptr;
        tail_ = bp;

        // spawn bluff particle
        ParticleSystemManager::getInstance().spawn(
            ParticleSystem(bp->pos, Animation(AnimData::death_1_dust, false)));

        if (splitHead && splitHead->prev) {
            auto it = new Wiggler(splitHead);
            it->changeDirection(splitHead);
            if (angry_) {
                // if the wiggler you divde is already angry the child you create should remain
                // angry.
                it->turnAngry();
            }
            toAdd.push_back(it);
        }

        turnAngry();
        mario->jump();
        // delete bp;
    } else {
        // cout << 'a' << endl;

        mario_update_rec(mario, bp->prev, toRemove, toAdd);
    }
}

void Wiggler::turnAngry() {
    angry_ = true;
    velocity_ = 2;

    auto it = head_;
    while (it) {
        it->bodyAnimator_->changeAnimation("angry");
        it = it->prev;
    }
}

void Wiggler::die() {
    die_rec(head_);
}

void Wiggler::die_rec(bodyPart *bp) {
    if (!bp) {
        return;
    }

    if (bp == tail_) {
        delete bp;
        return;
    }
    ParticleSystemManager::getInstance().spawn(
        ParticleSystem(bp->pos, Animation(AnimData::death_1_dust, false)));
    die_rec(bp->prev);
    delete bp;
}
