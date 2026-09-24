/**
 * @file particleSystem.hh
 * @brief
 * @version 0.1
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef PARTICLESYSTEM_HH
#define PARTICLESYSTEM_HH

#include <vector>
#include "components/animator.hh"
#include "core/geometry.hh"

// Forward declarations

namespace Anim {
class Animator;
class Animation;
}  // namespace Anim

namespace pro2 {
class Window;
}

/**
 * @brief ParticleSystem creates a particle system that spawns at a poition and when the animation
 * has ended destroys itself. Useful for dust, stars , and small VFX effects.
 *
 */
class ParticleSystem {
 private:
    pro2::Pt        pos_;
    Anim::Animator *animator_;
    bool            dead_;

 public:
    /**
     * @brief Construct a ParticleSystem at a position with an animation.
     */
    ParticleSystem(pro2::Pt, const Anim::Animation&);

    /**
     * @brief Returns true only if the animation in the animator has ended.
     *
     * @return true
     * @return false
     */
    bool hasEnded() const;

    /**
     * @brief Sets dead to true.
     *
     */
    void die();

    /**
     * @brief Paints the particle system on the window
     *
     * @param window
     */
    void paint(pro2::Window& window);
};

/**
 * @brief ParticleSystemManager is a class that manages particle systems and helps spawn them on the
 * world.
 */
/**
 * @class ParticleSystemManager
 * @brief Manages a collection of particle systems and provides functionality
 *        to spawn and render them.
 *
 * This class follows the Singleton design pattern to ensure that only one
 * instance of the ParticleSystemManager exists throughout the application.
 * It provides methods to spawn new particle systems and render them onto
 * a given window.
 */

class ParticleSystemManager {
 private:
    std::vector<ParticleSystem> particles_;

 public:
    /**
     * @brief Retrieves the singleton instance of the ParticleSystemManager.
     *
     * This method ensures that only one instance of the ParticleSystemManager
     * exists and provides global access to it.
     *
     * @return ParticleSystemManager& Reference to the singleton instance.
     */
    static ParticleSystemManager& getInstance() {
        static ParticleSystemManager instance;
        return instance;
    }

    /**
     * @brief Spawns a new particle system.
     * @param ps The particle system to be added.
     */

    void spawn(const ParticleSystem& ps);

    /**
     * @brief Renders all managed particle systems onto the specified window.
     * @param window Reference to the window where the particle systems will be rendered.
     */
    void paint(pro2::Window& window);
};

#endif