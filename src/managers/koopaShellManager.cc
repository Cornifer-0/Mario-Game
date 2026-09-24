#include "managers/koopaShellManager.hh"
#include <iostream>
#include "components/boxCollider.hh"
#include "enemies/koopaShell.hh"
#include "graphics/window.hh"
#include "managers/entityManager.hh"
#include "mario.hh"

using namespace pro2;
using namespace std;

void KoopaShellManager::spawn(const pro2::Pt pos) {
    KoopaShell *ks = new KoopaShell(pos);
    koopaShells_.push_back(ks);
    // cout << "spawned" << endl;
}

void KoopaShellManager::paint(pro2::Window& window, bool debug) {
    for (KoopaShell *ks : koopaShells_) {
        // cout << "To paint" << endl;
        ks->paint(window, debug);
        // cout << "painted" << endl;
    }
}

void KoopaShellManager::update(const std::vector<Platform *>& platforms,
                               Mario                         *mario,
                               Mario                         *luigi,
                               EntityManager                 *gm) {
    for (KoopaShell *ks : koopaShells_) {
        // cout << "To paint" << endl;
        ks->update(platforms);
        if ((mario->getCollider())->collidingWith(ks->getCollider()) && !mario->invulnarable()) {
            if (mario->is_grounded() && !ks->hasStoped()) {
                mario->die();
            } else if (!mario->is_grounded()) {
                mario->setInvulnarabilityFrames(5);
                mario->jump();
                ks->stop();
            }
        }
        if ((luigi->getCollider())->collidingWith(ks->getCollider()) && !luigi->invulnarable()) {
            if (luigi->is_grounded() && !ks->hasStoped()) {
                luigi->die();
            } else if (!luigi->is_grounded()) {
                luigi->setInvulnarabilityFrames(5);
                luigi->jump();
                ks->stop();
            }
        }

        gm->killIfInThisCollider(ks->getCollider());
    }
}