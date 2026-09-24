/**
 * @file koopaShellManager.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-05-25
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef KOOPASHELLMANAGER_HH
#define KOOPASHELLMANAGER_HH

#include <core/geometry.hh>
#include <vector>

// Forward definition
class KoopaShell;
class Platform;
class Mario;
class EntityManager;

namespace pro2 {
class Window;
}

class KoopaShellManager {
 private:
    std::vector<KoopaShell *> koopaShells_;

 public:
    /**
     * @brief Returns the only instance of a koopaShellManager.
     *
     * @return KoopaShellManager&
     */
    static KoopaShellManager& getInstance() {
        static KoopaShellManager instance;
        return instance;
    }

    /**
     * @brief Spawns a koopshell at a position.
     *
     */
    void spawn(const pro2::Pt);

    /**
     * @brief Updates the koopShell aswell as the objects around it, like the mario, luigi and the
     * goombas.
     *
     * @param platforms
     * @param mario
     * @param luigi
     * @param gm
     */
    void update(const std::vector<Platform *>& platforms,
                Mario                         *mario,
                Mario                         *luigi,
                EntityManager                 *gm);

    /**
     * @brief Paints all koopaShells.
     *
     * @param window
     * @param debug
     */
    void paint(pro2::Window& window, bool debug);
};

#endif