/**
 * @file kamek.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-06-15
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef KAMEK_HH
#define KAMEK_HH

#include <vector>
#include "core/geometry.hh"

namespace pro2 {
class Window;
}

namespace Anim {
class Animator;

}
class BoxCollider;
class Platform;
class Block;
class Mario;
class EntityManager;

enum KamekState {
    IDLE = 0,
    MOVE,
    MOVING,
    SUMMON,
    SUMMONING,

    // TODO

    //  CASTING,
    //  TELEPORT_IN,
    //  TELEPORT_OUT,
    //  STUNNED,
};

/**
 * @brief Class for Kamek enemy
 *
 */
class Kamek {
 private:
    pro2::Pt pos_, last_pos_, offset_pos_;
    pro2::Pt speed_;

    bool looking_left_;
    bool dead_ = false;

    int health_ = 3;
    int velocity_ = 1;

    KamekState state_ = IDLE;

    int state_timer_ = 0;

    pro2::Pt move_target_;
    pro2::Pt teleport_target_;

    const int IDLE_TIME = 50;
    const int SUMMONING_TIME = 40;

    BoxCollider    *boxCollider_;
    Anim::Animator *animator_;

    std::vector<pro2::Pt> minions_spawns_;

 public:
    /**
     * @brief Construct a new Kamek object
     *
     */
    Kamek();

    /**
     * @brief Construct a new Kamek object with position
     *
     * @param pos
     */
    Kamek(pro2::Pt pos);

    /**
     * @brief Paints Kamek on the window
     *
     * @param window
     * @param debug
     */
    void paint(pro2::Window& window, bool debug);

    /**
     * @brief Updates Kamek
     *
     * @param window
     * @param mario
     * @param platforms
     * @param blocks
     * @param gm_
     */
    void update(pro2::Window&                  window,
                Mario                         *mario,
                Mario                         *luigi,
                const std::vector<Platform *>& platforms,
                const std::vector<Block *>     blocks,
                EntityManager                 *gm_);

 private:
    /**
     * @brief Initializes kamek animations, state and collider
     *
     */
    void init();

    /**
     * @brief Changes the state of the Kamek, based on previous state and randomness.
     *
     */
    void changeState();

    /**
     * @brief Updates in the IDLE states, floats...
     *
     * @param window
     * @param mario
     * @param platforms
     * @param blocks
     */
    void update_IDLE(pro2::Window&                  window,
                     Mario                         *mario,
                     const std::vector<Platform *>& platforms,
                     const std::vector<Block *>     blocks);

    /**
     * @brief Updates in the MOVE state, chooses a position to move to.
     *
     * @param window
     * @param mario
     * @param platforms
     * @param blocks
     */

    /**
     * @brief
     *
     * @param window
     * @param mario
     * @param platforms
     * @param blocks
     */
    void update_MOVE(pro2::Window&                  window,
                     Mario                         *mario,
                     const std::vector<Platform *>& platforms,
                     const std::vector<Block *>     blocks);

    /**
     * @brief Updates in the MOVING state, moves towards move_target_
     *
     * @param window
     * @param mario
     * @param platforms
     * @param blocks
     */
    void update_MOVING(pro2::Window&                  window,
                       Mario                         *mario,
                       const std::vector<Platform *>& platforms,
                       const std::vector<Block *>     blocks);

    /**
     * @brief Updates in the SUMMON state, chooses random spots to spawn Goombas, and spawns
     * particles and animations
     *
     * @param window
     * @param mario
     * @param platforms
     * @param blocks
     * @param gm_
     * @param chooseOspawn
     */
    void update_SUMMON(pro2::Window&                  window,
                       Mario                         *mario,
                       const std::vector<Platform *>& platforms,
                       const std::vector<Block *>     blocks,
                       EntityManager                 *gm_,
                       bool                           chooseOspawn);

    /**
     * @brief Updates in the SUMMONING, summons goombas at all Pt in the vector goombas_spawns_
     *
     * @param window
     * @param mario
     * @param platforms
     * @param blocks
     * @param gm_
     */
    void update_SUMMONING(pro2::Window&                  window,
                          Mario                         *mario,
                          const std::vector<Platform *>& platforms,
                          const std::vector<Block *>     blocks,
                          EntityManager                 *gm_);

    /**
     * @brief Returns a random Pt on screen, atleast [innerMargin] away from the borders
     *
     * @param window
     * @param innerMargin
     * @return pro2::Pt
     */
    pro2::Pt randomPointOnScreen(pro2::Window& window, int innerMargin);
};

#endif