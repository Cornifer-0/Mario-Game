#include "managers/resourceManager.hh"
#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include "components/boxCollider.hh"
#include "core/finder.hh"
#include "graphics/window.hh"

using namespace pro2;
using namespace std;

ResourceManager::ResourceManager() {
    collected_ = 0;
    collectablesMap_ = judit;

    collectablesMap_ = {
        {{286, 164}, SMALL_COIN},  {{308, 164}, SMALL_COIN},  {{330, 164}, SMALL_COIN},
        {{352, 164}, SMALL_COIN},  {{374, 164}, SMALL_COIN},  {{396, 164}, SMALL_COIN},

        {{564, 220}, SMALL_COIN},  {{564, 200}, SMALL_COIN},  {{564, 180}, SMALL_COIN},
        {{586, 220}, SMALL_COIN},  {{586, 200}, SMALL_COIN},  {{586, 180}, SMALL_COIN},
        {{608, 220}, SMALL_COIN},  {{608, 200}, SMALL_COIN},  {{608, 180}, SMALL_COIN},
        {{630, 220}, SMALL_COIN},  {{630, 200}, SMALL_COIN},  {{630, 180}, SMALL_COIN},
        {{652, 220}, SMALL_COIN},  {{652, 200}, SMALL_COIN},  {{652, 180}, SMALL_COIN},
        {{674, 220}, SMALL_COIN},  {{674, 200}, SMALL_COIN},  {{674, 180}, SMALL_COIN},
        {{696, 220}, SMALL_COIN},  {{696, 200}, SMALL_COIN},  {{696, 180}, SMALL_COIN},
        {{718, 220}, SMALL_COIN},  {{718, 200}, SMALL_COIN},  {{718, 180}, SMALL_COIN},

        {{1010, 30}, COIN},        {{2472, 80}, COIN},

        {{1700, 220}, SMALL_COIN}, {{1700, 200}, SMALL_COIN}, {{1700, 180}, SMALL_COIN},
        {{1722, 220}, SMALL_COIN}, {{1722, 200}, SMALL_COIN}, {{1722, 180}, SMALL_COIN},
        {{1744, 220}, SMALL_COIN}, {{1744, 200}, SMALL_COIN}, {{1744, 180}, SMALL_COIN},
        {{1766, 220}, SMALL_COIN}, {{1766, 200}, SMALL_COIN}, {{1766, 180}, SMALL_COIN},
        {{1788, 220}, SMALL_COIN}, {{1788, 200}, SMALL_COIN}, {{1788, 180}, SMALL_COIN},
        {{1810, 220}, SMALL_COIN}, {{1810, 200}, SMALL_COIN}, {{1810, 180}, SMALL_COIN},
        {{1832, 220}, SMALL_COIN}, {{1832, 200}, SMALL_COIN}, {{1832, 180}, SMALL_COIN},
        {{1854, 220}, SMALL_COIN}, {{1854, 200}, SMALL_COIN}, {{1854, 180}, SMALL_COIN},

        {{3568, 180}, COIN},

        {{3200, 220}, SMALL_COIN}, {{3200, 200}, SMALL_COIN}, {{3200, 180}, SMALL_COIN},
        {{3222, 220}, SMALL_COIN}, {{3222, 200}, SMALL_COIN}, {{3222, 180}, SMALL_COIN},
        {{3244, 220}, SMALL_COIN}, {{3244, 200}, SMALL_COIN}, {{3244, 180}, SMALL_COIN},
        {{3266, 220}, SMALL_COIN}, {{3266, 200}, SMALL_COIN}, {{3266, 180}, SMALL_COIN},
        {{3288, 220}, SMALL_COIN}, {{3288, 200}, SMALL_COIN}, {{3288, 180}, SMALL_COIN},
        {{3310, 220}, SMALL_COIN}, {{3310, 200}, SMALL_COIN}, {{3310, 180}, SMALL_COIN},
        {{3332, 220}, SMALL_COIN}, {{3332, 200}, SMALL_COIN}, {{3332, 180}, SMALL_COIN},
        {{3354, 220}, SMALL_COIN}, {{3354, 200}, SMALL_COIN}, {{3354, 180}, SMALL_COIN},

        {{6700, 220}, SMALL_COIN}, {{6722, 220}, SMALL_COIN}, {{6744, 220}, SMALL_COIN},
        {{6766, 220}, SMALL_COIN}, {{6766, 200}, SMALL_COIN}, {{6788, 220}, SMALL_COIN},
        {{6788, 200}, SMALL_COIN}, {{6810, 220}, SMALL_COIN}, {{6810, 200}, SMALL_COIN},
        {{6832, 220}, SMALL_COIN}, {{6832, 200}, SMALL_COIN}, {{6832, 180}, SMALL_COIN},
        {{6854, 220}, SMALL_COIN}, {{6854, 200}, SMALL_COIN}, {{6854, 180}, SMALL_COIN},

        {{7150, 150}, END_STAR},
    };

    respawnCollectables();
}

ResourceManager::ResourceManager(CollectablesMap cm) {
    collected_ = 0;
    collectablesMap_ = cm;
    respawnCollectables();
}

void ResourceManager::respawnCollectables() {
    collectables_.clear();
    f_Collectable_ = new Finder<Collectable>();

    for (auto& [point, ct] : collectablesMap_) {
        collectables_.push_back(Collectable{point, ct});
    }

    for (auto& coll : collectables_) {
        // cout << coll.get_rect().left << ' ' << coll.get_rect().top << ' ' <<
        // coll.get_rect().right
        //  << endl;
        f_Collectable_->add(&coll);
    }
}

void ResourceManager::paint(pro2::Window& window, bool debug) {
    set<Collectable *> setToPaint = f_Collectable_->query(window.camera_rect());
    auto               it = setToPaint.begin();
    while (it != setToPaint.end()) {
        (*it)->paint(window, debug);
        it++;
    }
}

int ResourceManager::update(BoxCollider *marioCollider) {
    auto it = collectables_.begin();

    while (it != collectables_.end()) {
        if (marioCollider->collidingWith(it->getCollider()) && !it->isDestroyed()) {
            if (it->getType() == SMALL_COIN) {
                collected_++;
            } else if (it->getType() == COIN) {
                collected_ += 10;
            } else if (it->getType() == END_STAR) {
                return 1;
            }
            f_Collectable_->remove(&(*it));
            it->Destroy();
        } else {
            ++it;
        }
    }
    return 0;
}

int ResourceManager::getCollected() const {
    return collected_;
}
