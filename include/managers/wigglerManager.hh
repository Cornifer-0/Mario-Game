/**
 * @file wigglerManager.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-06-15
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef WIGGLERMANAGER_HH
#define WIGGLERMANAGER_HH

#include <vector>
#include "core/geometry.hh"

class Wiggler;
class Platform;
class Mario;

namespace pro2 {
class Window;
}

/**
 * @brief Class for managing wigglers.
 *
 */
class WigglerManager {
 private:
    std::vector<Wiggler *> wigglers_;
    std::vector<Wiggler *> newWigglers_;
    std::vector<Wiggler *> toRemoveWigglers_;

 public:
    /**
     * @brief Construct a new Wiggler Manager object
     *
     */
    WigglerManager();

    /**
     * @brief Updates all wigglers in wigglers_, fills newWigglers_ with new wigglers to add, and
     * fills toRemoveWigglers_ with wigglers that have been killed.
     *
     * Also moves all wigglers from newWigglers to wigglers_ and emptys newWigglers. As well as
     * removing all wigglers in wigglers_ that are in toRemoveWigglers_, then empties
     * toRemoveWigglers_.
     *
     * @param mario
     * @param luigi
     * @param platforms
     */
    void update(Mario *mario, Mario *luigi, const std::vector<Platform *>& platforms);

    /**
     * @brief Paints all wigglers in wigglers_
     *
     * @param window
     * @param debug
     */
    void paint(pro2::Window& window, bool debug);
};

#endif