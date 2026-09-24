/**
 * @file flagManager.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-06-15
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef FLAGMANAGER_HH
#define FLAGMANAGER_HH

#include <vector>
#include "core/geometry.hh"

namespace pro2 {
class Window;
};

class Mario;
class BoxCollider;

namespace Anim {
class Animator;
};

/**
 * @brief Class for managing flags in the game. These flags, are used to set checkpoints.
 *
 */
class FlagManager {
 private:
    enum FlagState {
        BOWSER_FLAG,
        MARIO_FLAG,
        LUIGI_FLAG,
    };

    struct Flag {
        pro2::Pt  pos_;
        FlagState state_;

        Anim::Animator *animator_;
        BoxCollider    *boxCollider_;

        Flag(pro2::Pt pos) : pos_(pos), state_(BOWSER_FLAG) {}
    };

    std::vector<Flag> flags_;

 public:
    /**
     * @brief Construct a new Flag Manager object
     *
     */
    FlagManager();
    /**
     * @brief Updates the flags in the game, they change state if touched by a player.
     *
     * @param mario
     * @param luigi
     */
    void update(Mario *mario, Mario *luigi);

    /**
     * @brief Paints all flags, with animation.
     *
     * @param window
     * @param debug
     */
    void paint(pro2::Window& window, bool debug);
};

#endif