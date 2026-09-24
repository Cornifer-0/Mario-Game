#include "managers/wigglerManager.hh"
#include <algorithm>
#include <iostream>
#include "enemies/wiggler.hh"

using namespace std;
using namespace pro2;

WigglerManager::WigglerManager() {
    wigglers_ = {new Wiggler({2500, 250}, 16), new Wiggler({6010, 250}, 8),
                 new Wiggler({6500, 250}, 12), new Wiggler({6800, 250}, 4)};
    newWigglers_ = {};
    toRemoveWigglers_ = {};
}

void WigglerManager::update(Mario *mario, Mario *luigi, const std::vector<Platform *>& platforms) {
    for (auto wigg : wigglers_) {
        wigg->update(mario, luigi, platforms, toRemoveWigglers_, newWigglers_);
    }

    auto itRemovep = toRemoveWigglers_.begin();
    while (itRemovep != toRemoveWigglers_.end()) {
        auto it = find(wigglers_.begin(), wigglers_.end(), *itRemovep);
        if (it != wigglers_.end()) {
            delete *it;
            wigglers_.erase(it);
        }
        itRemovep = toRemoveWigglers_.erase(itRemovep);
    }

    auto toAddp = newWigglers_.begin();
    while (toAddp != newWigglers_.end()) {
        wigglers_.push_back(*toAddp);
        toAddp = newWigglers_.erase(toAddp);
    }
}

void WigglerManager::paint(Window& window, bool debug) {
    for (auto wigg : wigglers_) {
        wigg->paint(window, debug);
    }
}