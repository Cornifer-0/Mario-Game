#include "managers/entityManager.hh"
#include <algorithm>
#include <iostream>
#include <utility>
#include "components/HUD.hh"
#include "components/boxCollider.hh"
#include "core/finder.hh"
#include "graphics/window.hh"
#include "managers/koopaShellManager.hh"
#include "mario.hh"

using namespace pro2;
using namespace std;

// GoombasManager::GoombasManager() {
//     GoombasManager(defaultGoombaMap);
// }

EntityManager::EntityManager() {
    killed_ = 0;
    entityMap_ = {
        {{1010, 160}, GOOMBA}, {{1010, 150}, GOOMBA}, {{1010, 140}, GOOMBA},
        {{1010, 130}, GOOMBA}, {{1100, 160}, GOOMBA}, {{900, 250}, KOOPA},
        {{900, 1100}, GOOMBA}, {{1300, 220}, GOOMBA}, {{790, 1050}, ANGRY},

    };
    respawnEntities();
}

void EntityManager::respawnEntities() {
    entities_ = {};
    // cout << "HERE" << endl;
    for (auto& [point, type] : entityMap_) {
        entities_.push_back(Entity{point, type});
    }
    // cout << "there" << endl;

    // goombas_ = {};
}

void EntityManager::paint(pro2::Window& window, bool debug) {
    // cout << "ented once" << endl;
    for (Entity& g : entities_) {
        g.paint(window, debug);
    }
    // cout << "out once" << endl;
}

void EntityManager::update(Mario *mario, HUD *hud_mushrooms) {
    auto it = entities_.begin();

    if (mario->is_dead()) {
        while (it != entities_.end()) {
            it->update();
            it++;
        }
        return;
    }

    while (it != entities_.end()) {
        auto head = it->getHeadCollider();
        auto body = it->getBodyCollider();
        auto visibility = it->getVisibilityCollider();

        auto marioCollider = mario->getCollider();

        if (marioCollider->collidingWith(head)) {
            // spawn koopa shell if it was a koopa type goomba
            if (it->getType() == KOOPA) {
                KoopaShellManager::getInstance().spawn(it->pos() + Pt{0, 10});
                mario->jump();
                it = entities_.erase(it);
                killed_++;
            } else if (it->getType() == MUSHROOM) {
                hud_mushrooms->plusOne();
                it->die();
                it = entities_.erase(it);
            } else {
                it->die();
                mario->jump();
                it = entities_.erase(it);
                killed_++;
            }
        } else if (marioCollider->collidingWith(body)) {
            if (it->getType() == MUSHROOM) {
                // cout << "collected" << endl;
                hud_mushrooms->plusOne();
                it->die();
                it = entities_.erase(it);
            } else {
                mario->die();
                it->update();
                it++;
            }

        } else {
            if (marioCollider->collidingWith(visibility) && it->getType() == ANGRY) {
                if (mario->pos().x <= it->pos().x && !it->looking_left()) {
                    it->changeDirection();
                }

                if (mario->pos().x > it->pos().x && it->looking_left()) {
                    it->changeDirection();
                }

                it->set_mario_in_sight(true);
            } else {
                it->set_mario_in_sight(false);
            }

            it->update();
            ++it;
        }
    }
}

void EntityManager::check_collisions(const std::vector<Platform *>& platforms,
                                     const std::set<Block *>&       blocks) {
    auto it = entities_.begin();

    while (it != entities_.end()) {
        it->check_collisions(platforms, blocks);
        ++it;
    }
}

void EntityManager::killIfInThisCollider(BoxCollider *boxCollider) {
    auto it = entities_.begin();

    while (it != entities_.end()) {
        if (it->getBodyCollider()->collidingWith(boxCollider) &&
            (it->getType() == GOOMBA || it->getType() == ANGRY)) {
            it->die();

            // spawn koopa shell if it was a koopa type goomba
            if (it->getType() == KOOPA) {
                KoopaShellManager::getInstance().spawn(it->pos() + Pt{0, 10});
            }

            it = entities_.erase(it);
        } else {
            ++it;
        }
    }
}

void EntityManager::spawnEntity(pro2::Pt pos, EntityType type) {
    Entity *new_goomba = new Entity(pos, type);
    entities_.push_back(*new_goomba);
}

int EntityManager::getKilled() const {
    return killed_;
}
