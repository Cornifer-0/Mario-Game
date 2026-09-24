#include "managers/manager.hh"
#include <set>
#include "assets/animation_data.hh"
#include "components/HUD.hh"
#include "components/particleSystem.hh"
#include "enemies/kamek.hh"
#include "graphics/window.hh"
#include "managers/blockManager.hh"
#include "managers/entityManager.hh"
#include "managers/flagManager.hh"
#include "managers/koopaShellManager.hh"
#include "managers/platformManager.hh"
#include "managers/resourceManager.hh"
#include "managers/wigglerManager.hh"
#include "mario.hh"
#include "world/platform.hh"

using namespace std;
using namespace pro2;

Manager::Manager() {
    fm_ = new FlagManager();
    wg_ = new WigglerManager();
    rm_ = new ResourceManager();
    hud_coin_ = new HUD(Anim::Animation(AnimData::wonder_coin, false), {2, 2});
    hud_time_ = new HUD(Anim::Animation(AnimData::clock, false, 16), {2, 32});
    hud_mushroom_ = new HUD(Anim::Animation(AnimData::mushroom, false, 16), {2, 56});

    gm_ = new EntityManager();
    bm_ = new BlockManager();
    pm_ = new PlatformManager();

    kamek_ = new Kamek({4000, 200});
    kamek2_ = new Kamek({5200, 100});
    kamek3_ = new Kamek({5400, 100});
    kamek4_ = new Kamek({5600, 50});
}

int Manager::update(Mario *mario, Mario *luigi, pro2::Window& window) {
    // cout << "start update" << endl;

    pm_->update(window);
    const set<Platform *>&    s_platforms = pm_->getFinderPlatforms();
    const vector<Platform *>& platforms = pm_->getAllPlatforms();

    kamek_->update(window, mario, luigi, platforms, bm_->get_blocks(), gm_);
    kamek2_->update(window, mario, luigi, platforms, bm_->get_blocks(), gm_);
    kamek3_->update(window, mario, luigi, platforms, bm_->get_blocks(), gm_);
    kamek4_->update(window, mario, luigi, platforms, bm_->get_blocks(), gm_);

    wg_->update(mario, luigi, platforms);
    // myWiggler->update(mario_, platforms_);
    mario->update(window, s_platforms);
    luigi->update(window, s_platforms);
    gm_->update(mario, hud_mushroom_);
    gm_->update(luigi, hud_mushroom_);
    gm_->check_collisions(platforms, bm_->get_blocks_f(window));
    bm_->update(mario, luigi, gm_);
    fm_->update(mario, luigi);

    if (rm_->update(mario->getCollider()) == 1 || rm_->update(luigi->getCollider()) == 1) {
        hud_coin_->changeRelativePosition(Pt{window.width() / 2, window.height() / 2} +
                                          Pt{-22, -48});
        hud_time_->changeRelativePosition(Pt{window.width() / 2, window.height() / 2} +
                                          Pt{-22, -16});

        hud_mushroom_->changeRelativePosition(Pt{window.width() / 2, window.height() / 2} +
                                              Pt{-22, 12});

        int coins_collected = hud_coin_->getValue();
        int time_finish = hud_time_->getValue();
        int mushrooms_collected = hud_mushroom_->getValue();

        hud_coin_->updateValue(0);
        hud_coin_->plusN(coins_collected);
        hud_time_->updateValue(0);
        hud_time_->plusN(time_finish);
        hud_mushroom_->updateValue(0);
        hud_mushroom_->plusN(mushrooms_collected);
        return 1;
    }

    if (rm_->getCollected() > hud_coin_->getValue()) {
        hud_coin_->plusN(rm_->getCollected() - hud_coin_->getValue());
    }

    if (window.frame_count() % 48 == 0) {
        hud_time_->plusOne();
    }

    KoopaShellManager::getInstance().update(platforms, mario, luigi, gm_);
    // cout << "end update" << endl;

    return 0;
}

void Manager::paint(pro2::Window& window, bool debug, bool end) {
    if (end) {
        // cout << "painting" << endl;
        paint_sprite_center(window, window.camera_center() - Pt{0, 72}, AnimData::super_mario_pro2,
                            false);

        hud_coin_->paint(window);
        hud_time_->paint(window);
        hud_mushroom_->paint(window);
        return;
    }
    fm_->paint(window, debug);
    pm_->paint(window, debug);
    set<Platform *>    s_platforms = pm_->getFinderPlatforms();
    vector<Platform *> platforms = pm_->getAllPlatforms();

    wg_->paint(window, debug);

    ParticleSystemManager::getInstance().paint(window);
    KoopaShellManager::getInstance().paint(window, debug);

    rm_->paint(window, debug);

    gm_->paint(window, debug);

    auto it = s_platforms.begin();
    while (it != s_platforms.end()) {
        (*it)->paint(window, debug);
        it++;
    }

    bm_->paint(window, debug);

    kamek_->paint(window, debug);
    kamek2_->paint(window, debug);
    kamek3_->paint(window, debug);
    kamek4_->paint(window, debug);

    hud_coin_->paint(window);
    hud_time_->paint(window);
    hud_mushroom_->paint(window);
}