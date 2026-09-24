#include "core/game.hh"
#include <iostream>
#include "assets/animation_data.hh"
#include "components/HUD.hh"
#include "components/boxCollider.hh"
#include "components/particleSystem.hh"
#include "core/finder.hh"
#include "core/geometry.hh"
#include "core/utils.hh"
#include "enemies/kamek.hh"
#include "enemies/wiggler.hh"
#include "graphics/window.hh"
#include "managers/blockManager.hh"
#include "managers/entityManager.hh"
#include "managers/koopaShellManager.hh"
#include "managers/manager.hh"
#include "managers/resourceManager.hh"
#include "managers/wigglerManager.hh"
#include "mario.hh"
#include "world/platform.hh"

using namespace pro2;
using namespace std;

Game::Game(int width, int height) : finished_(false), paused_(false), debug_(false) {
    manager_ = new Manager();
    mario_ = new Mario("mario", {100, 100}, defaultPlayerOneControls, true);
    luigi_ = new Mario("luigi", {100, 100}, defaultPlayerTwoControls, false);
}

void Game::process_keys(pro2::Window& window) {
    if (window.is_key_down(Keys::Escape)) {
        finished_ = true;
        return;
    }

    if (end_sequence_) {
        return;
    }

    if (window.was_key_pressed(Keys::E)) {
        debug_ = !debug_;
        // if (debug_) {
        //     std::cout << "DEBUGGING TOOLS ACTIVATED..." << std::endl;
        // } else {
        //     std::cout << "DEBUGGING TOOLS DESACTIVATED..." << std::endl;
        // }
        // std::cout << "RESOURCE COLLECTED -> " << rm_->getCollected() << std::endl;
    }

    // if (window.was_key_pressed(Keys::R)) {
    //     rm_->respawnCollectables();
    //     gm_->respawnGoombas();
    // }
    if (window.was_key_pressed(Keys::P)) {
        paused_ = !paused_;
    }

    if (window.was_key_pressed(Keys::K)) {
        mario_->die();
        if (twoPlayers) {
            luigi_->die();
        }
    }

    if (window.was_key_pressed(Keys::U)) {
        mario_->respawn();
        if (twoPlayers) {
            luigi_->respawn();
        }
    }

    if (window.was_key_pressed(Keys::U)) {
        mario_->respawn();
        if (twoPlayers) {
            luigi_->respawn();
        }
    }

    if (window.was_key_pressed('2')) {
        if (twoPlayers) {
            luigi_->die();
            ParticleSystemManager::getInstance().spawn(
                ParticleSystem(luigi_->pos(), Anim::Animation(AnimData::blue_shine_2_, false, 2)));
            twoPlayers = false;
            // cout << "luigi dead" << endl;
        } else {
            // cout << 'a' << endl;
            luigi_->changeResapawn(mario_->pos());
            luigi_->respawn();
            luigi_->setInvulnarabilityFrames(10);
            twoPlayers = true;
            // cout << 'b' << endl;
        }
    }

    // if (window.was_key_pressed(Keys::H)) {
    //     for (Goomba *goomba : goombas_) {
    //         goomba->die();
    //     }
    // }
}

Game::~Game() {
    // cout << "Retornant la memoria" << endl;
    delete mario_;
    delete luigi_;
    delete manager_;
}

void Game::update_objects(pro2::Window& window) {
    // if (twoPlayers) {
    //     cout << "2 player mode" << endl;
    // } else {
    //     cout << "1 player mode" << endl;
    // }
    if (twoPlayers && !luigi_->getCollider()->collidingWith(window.camera_rect() + 10) &&
        !luigi_->invulnarable()) {
        // luigi_->die();
        twoPlayers = false;
    }

    if (manager_->update(mario_, luigi_, window) == 1) {
        end_sequence_ = true;
    }
}

void Game::update_camera(pro2::Window& window) {
    const Pt pos = mario_->pos();
    const Pt cam = window.camera_center();

    const int left = cam.x - window.width() / 4;
    const int right = cam.x + window.width() / 4 - 100;
    const int top = cam.y - window.height() / 4 - 50;
    const int bottom = cam.y + window.height() / 4 + 50;

    int dx = 0, dy = 0;
    if (pos.x > right) {
        dx = pos.x - right;
    } else if (pos.x < left) {
        dx = pos.x - left;
    }
    // if (pos.y < top) {
    //     dy = pos.y - top;
    // } else if (pos.y > bottom) {
    //     dy = pos.y - bottom;
    // }

    window.move_camera({dx, dy});
}

void Game::update(pro2::Window& window) {
    process_keys(window);

    if (paused_ || end_sequence_) {
        return;
    }
    update_objects(window);
    update_camera(window);
    // hud_.updateValue(rm_.getCollected());
    // std::cout << "last time : " << window.get_last_time_() << std::endl;
    // std::cout << "delta time: " << window.getDeltaTime() << std::endl;
}

void Game::paint(pro2::Window& window) {
    if (end_sequence_) {
        paint_end_circle(window, end_sequence_time_, mario_->pos());
        end_sequence_time_ -= 2;
        // cout << end_sequence_time_ << endl;
        if (end_sequence_time_ <= -50) {
            // cout << "game finished" << endl;
            manager_->paint(window, debug_, true);
        }
        return;
    }
    window.clear(sky_blue);
    paint_background_sprite(window, AnimData::rolling_hills, 0.5f, AnimData::background_sky);
    // for (const Platform *p : platforms_) {
    //     p->paint(window, debug_);
    // }
    mario_->paint(window, debug_);
    if (twoPlayers) {
        luigi_->paint(window, debug_);
    }
    // std::cout << "how many  " << std::endl;
    // rm_.paint(window, debug_);
    manager_->paint(window, debug_, false);
    // std::cout << "How many" << std::endl;
    // collectables_[0].paint(window);
    // collectables_[1].paint(window);
}