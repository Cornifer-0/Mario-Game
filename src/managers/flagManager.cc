#include "managers/flagManager.hh"
#include "assets/animation_data.hh"
#include "components/animator.hh"
#include "components/boxCollider.hh"
#include "components/particleSystem.hh"
#include "core/utils.hh"
#include "graphics/window.hh"
#include "mario.hh"

using namespace pro2;
using namespace std;
using namespace Anim;

FlagManager::FlagManager() {
    flags_ = {Flag({1900, 235}), Flag({3400, 235}), Flag({4800, 235})};

    for (auto& flag : flags_) {
        flag.animator_ = new Animator(Animation(AnimData::bowser_flag));
        flag.animator_->addAnimation("mario", Animation(AnimData::mario_flag));
        flag.animator_->addAnimation("luigi", Animation(AnimData::luigi_flag));
        flag.boxCollider_ = new BoxCollider();
        flag.boxCollider_->update(flag.pos_, flag.animator_->currentFrame());
    }
}

void FlagManager::update(Mario *mario, Mario *luigi) {
    for (auto& flag : flags_) {
        if (flag.boxCollider_->collidingWith(mario->getCollider())) {
            // colliding with mairo

            if (flag.state_ != MARIO_FLAG) {
                flag.state_ = MARIO_FLAG;
                flag.animator_->changeAnimation("mario");
                ParticleSystemManager::getInstance().spawn(
                    ParticleSystem(flag.pos_, Animation(AnimData::blue_shine_2_, false, 2)));

                mario->changeResapawn(flag.pos_);
            }

        } else if (flag.boxCollider_->collidingWith(luigi->getCollider())) {
            if (flag.state_ != LUIGI_FLAG) {
                flag.state_ = LUIGI_FLAG;
                flag.animator_->changeAnimation("luigi");
                ParticleSystemManager::getInstance().spawn(
                    ParticleSystem(flag.pos_, Animation(AnimData::blue_shine_2_, false, 2)));

                luigi->changeResapawn(flag.pos_);
            }
        }
    }
}

void FlagManager::paint(pro2::Window& window, bool debug) {
    for (auto flag : flags_) {
        if (flag.boxCollider_->collidingWith(window.camera_rect())) {
            // cout << 'a' << endl;
            // cout << flag.pos_.x << ' ' << flag.pos_.y << endl;

            paint_sprite_center(window, flag.pos_, flag.animator_->nextFrame(), false);

            if (debug) {
                flag.boxCollider_->printDebugLines(window);
            }
        }
    }
}