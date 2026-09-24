#include "enemies/kamek.hh"
#include <cmath>
#include <random>
#include "assets/animation_data.hh"
#include "components/animator.hh"
#include "components/boxCollider.hh"
#include "components/particleSystem.hh"
#include "core/utils.hh"
#include "graphics/window.hh"
#include "managers/entityManager.hh"
#include "mario.hh"

using namespace std;
using namespace pro2;
using namespace Anim;

Kamek::Kamek() {
    pos_ = offset_pos_ = {0, 0};
    init();
}

Kamek::Kamek(pro2::Pt pos) {
    pos_ = offset_pos_ = pos;
    init();
}

void Kamek::init() {
    animator_ = new Animator(Animation(AnimData::kamek_idle));
    animator_->addAnimation("moving", AnimData::kamek_fly);
    animator_->addAnimation("shouting", AnimData::kamek_fly_shout);

    // Add more animations for fly, spawn ... teleport

    boxCollider_ = new BoxCollider();
    boxCollider_->update(pos_, animator_->currentFrame());
    state_ = IDLE;
    state_timer_ = IDLE_TIME;
}

void Kamek::paint(pro2::Window& window, bool debug) {
    if (dead_) {
        return;
    }
    paint_sprite_center(window, offset_pos_, animator_->nextFrame(), looking_left_);

    if (debug) {
        boxCollider_->printDebugLines(window);
        window.set_pixel(move_target_, 0xFF0000);
    }
}

void Kamek::update(pro2::Window&                  window,
                   Mario                         *mario,
                   Mario                         *luigi,
                   const std::vector<Platform *>& platforms,
                   const std::vector<Block *>     blocks,
                   EntityManager                 *gm_) {
    // for each state call update fucntion associated with that state.

    if (dead_ || !(boxCollider_->collidingWith(window.camera_rect()))) {
        return;
    }

    if (boxCollider_->collidingWith(mario->getCollider())) {
        if (mario->is_falling()) {
            dead_ = true;
            ParticleSystemManager::getInstance().spawn(
                ParticleSystem(pos_, Anim::Animation(AnimData::spawn_effect, false)));
            mario->jump();
        } else {
            mario->die();
        }
    }

    if (boxCollider_->collidingWith(luigi->getCollider())) {
        if (luigi->is_falling()) {
            dead_ = true;
            ParticleSystemManager::getInstance().spawn(
                ParticleSystem(pos_, Anim::Animation(AnimData::spawn_effect, false)));
            luigi->jump();
        } else {
            luigi->die();
        }
    }

    if (state_ == IDLE) {
        update_IDLE(window, mario, platforms, blocks);

    } else if (state_ == MOVE) {
        update_MOVE(window, mario, platforms, blocks);

    } else if (state_ == MOVING) {
        update_MOVING(window, mario, platforms, blocks);

    } else if (state_ == SUMMON) {
        update_SUMMON(window, mario, platforms, blocks, gm_, false);
    } else if (state_ == SUMMONING) {
        update_SUMMONING(window, mario, platforms, blocks, gm_);
    }
    //  else if (state_ == SUMMONING) {
    // } else if (state_ == CASTING) {
    // } else if (state_ == TELEPORT_IN) {
    // } else if (state_ == TELEPORT_OUT) {
    // } else if (state_ == STUNNED) {
    // } else if (state_ == COOLDOWN) {
    // }

    state_timer_--;
}

void Kamek::changeState() {
    // int dice = rand();
    // state_ = IDLE;
    // TODO: more complex way to change state

    if (state_ == IDLE) {
        int dice = rand() % 3;

        if (dice == 0) {
            state_ = MOVE;
        } else {
            state_ = SUMMON;
        }

    } else {
        state_ = IDLE;
        state_timer_ = IDLE_TIME;
    }
}

void Kamek::update_IDLE(pro2::Window&                  window,
                        Mario                         *mario,
                        const std::vector<Platform *>& platforms,
                        const std::vector<Block *>     blocks) {
    // floating...
    double offset = 10.0 * std::sin(window.frame_count() / 20.0f);
    offset_pos_ = pos_ + Pt{0, static_cast<int>(offset)};
    boxCollider_->update(offset_pos_, animator_->currentFrame());

    // make kamek look to mario.
    looking_left_ = (mario->pos().x > pos_.x) ? true : false;

    if (state_timer_ <= 0) {
        changeState();
    }
}

void Kamek::update_MOVE(pro2::Window&                  window,
                        Mario                         *mario,
                        const std::vector<Platform *>& platforms,
                        const std::vector<Block *>     blocks) {
    move_target_ = randomPointOnScreen(window, 40);
    state_ = MOVING;
}

void Kamek::update_MOVING(pro2::Window&                  window,
                          Mario                         *mario,
                          const std::vector<Platform *>& platforms,
                          const std::vector<Block *>     blocks) {
    animator_->changeAnimation("moving");

    const int margin = 5;  // if withing [margin] pixels, it's good enough

    if (abs(pos_.x - move_target_.x) > margin) {
        if (pos_.x <= move_target_.x) {
            pos_.x += velocity_;
        } else if (pos_.x >= move_target_.x) {
            pos_.x -= velocity_;
        }
    } else {
        animator_->changeAnimation("idle");
    }

    if (abs(pos_.y - move_target_.y) > margin) {
        if (pos_.y <= move_target_.y) {
            pos_.y += velocity_;
        } else if (pos_.y >= move_target_.y) {
            pos_.y -= velocity_;
        }
    }

    looking_left_ = (pos_.x < move_target_.x) ? true : false;

    double offset = 10.0 * std::sin(window.frame_count() / 20.0f);
    offset_pos_ = pos_ + Pt{0, static_cast<int>(offset)};
    boxCollider_->update(offset_pos_, animator_->currentFrame());

    if (abs(pos_.y - move_target_.y) <= margin && abs(pos_.x - move_target_.x) <= margin) {
        changeState();
        animator_->changeAnimation("idle");
    }
}

void Kamek::update_SUMMON(pro2::Window&                  window,
                          Mario                         *mario,
                          const std::vector<Platform *>& platforms,
                          const std::vector<Block *>     blocks,
                          EntityManager                 *gm_,
                          bool                           chooseOspawn) {
    if (!chooseOspawn) {
        const int numberOfSpawns = rand() % 3 + 2;

        for (int i = 0; i < numberOfSpawns; i++) {
            Pt new_pos = randomPointOnScreen(window, 100);
            ParticleSystemManager::getInstance().spawn(
                ParticleSystem(new_pos, Anim::Animation(AnimData::spawn_effect_2_, false, 8)));
            minions_spawns_.push_back(new_pos);
            // gm_->spawnGoomba(new_pos, NORMAL);
        }

        state_ = SUMMONING;
        state_timer_ = SUMMONING_TIME;
    } else {
        for (Pt point : minions_spawns_) {
            int dice = rand() % 5;
            if (dice == 0) {
                gm_->spawnEntity(point, KOOPA);
            } else {
                gm_->spawnEntity(point, GOOMBA);
            }
        }
        minions_spawns_.clear();
    }
}

void Kamek::update_SUMMONING(pro2::Window&                  window,
                             Mario                         *mario,
                             const std::vector<Platform *>& platforms,
                             const std::vector<Block *>     blocks,
                             EntityManager                 *gm_) {
    animator_->changeAnimation("shouting");

    if (state_timer_ <= 0) {
        update_SUMMON(window, mario, platforms, blocks, gm_, true);
        changeState();
        animator_->changeAnimation("idle");
    }
}

Pt Kamek::randomPointOnScreen(pro2::Window& window, int innerMargin) {
    int dice1 = rand();
    int dice2 = rand();

    int cam_left = window.camera_rect().left + innerMargin;
    int cam_top = window.camera_rect().top + innerMargin;
    int cam_right = window.camera_rect().right - innerMargin;
    int cam_bottom = window.camera_rect().bottom - innerMargin;

    int x = cam_left + rand() % (cam_right - cam_left);
    int y = cam_top + rand() % (cam_bottom - cam_top);

    return {x, y};
}
