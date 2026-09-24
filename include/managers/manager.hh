/**
 * @file manager.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-06-15
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef MANAGER_HH
#define MANAGER_HH

class ResourceManager;
class EntityManager;
class BlockManager;
class HUD;
class WigglerManager;
class PlatformManager;
class Kamek;
class FlagManager;

namespace pro2 {
class Window;
}

class Mario;

/**
 * @brief General Manager for the game. A manager of managers.
 *
 */
class Manager {
 private:
    FlagManager     *fm_;
    PlatformManager *pm_;
    ResourceManager *rm_;
    EntityManager   *gm_;
    BlockManager    *bm_;
    WigglerManager  *wg_;

    Kamek *kamek_;
    Kamek *kamek2_;
    Kamek *kamek3_;
    Kamek *kamek4_;

    HUD *hud_coin_;
    HUD *hud_time_;
    HUD *hud_mushroom_;

 public:
    Manager();

    /**
     * @brief Updates all managers, returns 1 only if the game has ended (mario has collected the
     * END_STAR)
     *
     * @param mario
     * @param luigi
     * @param window
     * @return int
     */
    int update(Mario *mario, Mario *luigi, pro2::Window& window);

    /**
     * @brief Paints everything in every manager. (most of the game).
     *
     * @param window
     * @param debug
     * @param end
     */
    void paint(pro2::Window& window, bool debug, bool end);
};

#endif