/**
 * @file platformManager.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-06-15
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef PLATFORM_MANAGER_HH
#define PLATFORM_MANAGER_HH

#include <set>
#include <vector>

class Platform;

namespace pro2 {
class Window;
}

template <typename T>
class Finder;

/**
 * @brief A manager for platforms.
 *
 */
class PlatformManager {
 private:
    std::vector<Platform *> platforms_;
    std::set<Platform *>    s_platforms_;
    Finder<Platform>       *f_Platforms_;

 public:
    /**
     * @brief Construct a new Platform Manager object
     *
     */
    PlatformManager();

    const std::vector<Platform *>& getAllPlatforms() const;
    const std::set<Platform *>&    getFinderPlatforms() const;

    /**
     * @brief Updates platforms in s_platforms_
     *
     * @param window
     */
    void update(pro2::Window& window);

    /**
     * @brief Paints all platforms in s_platforms_
     *
     * @param window
     * @param debug
     */
    void paint(pro2::Window& window, bool debug);
};

#endif