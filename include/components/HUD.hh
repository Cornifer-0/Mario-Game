/**
 * @file HUD.hh
 * @brief
 * @version 0.1
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef HUD_HH
#define HUD_HH

#include <vector>
#include "core/geometry.hh"

typedef std::vector<std::vector<int>> Frame;

// Forward declarations
namespace Anim {
class Animator;
class Animation;
}  // namespace Anim

namespace pro2 {
class Window;
}  // namespace pro2

class HUD {
 private:
    Anim::Animator *icon_;
    int             value_;
    pro2::Pt        pos_relative_to_camera_;
    int             spins_left_;

 public:
    /**
     * @brief An Empty HUD, not used.
     *
     */
    HUD();

    /**
     * @brief Construct a new HUD object with an animation for the object to be collected, this
     * represents the icon_.
     *
     * @param anim
     */
    HUD(const Anim::Animation& anim);

    HUD(const Anim::Animation& anim, pro2::Pt pos_relative_to_camera);

    /**
     * @brief Construct a new HUD object with an animation for the object to be collected, this
     * represents the icon_. value_ is initialized at value.
     *
     * @param anim
     * @param value
     */
    HUD(const Anim::Animation& anim, int value);

    /**
     * @brief Updates the value_ attribute.
     *
     * @param value
     */
    void updateValue(int value);

    /**
     * @brief Adds one to the value_ attribute.
     *
     */
    void plusOne();

    /**
     * @brief Adds n to the value_ attribute.
     *
     * @param n
     */
    void plusN(int n);

    /**
     * @brief Returns the value_ attribute.
     *
     * @return int
     */
    int getValue() const;

    void changeRelativePosition(pro2::Pt new_pos);

    /**
     * @brief Paints the HUD on the screen. Fixed position relative to the camera.
     *
     * @param window
     */
    void paint(pro2::Window& window);

 private:
    /**
     * @brief Returns the Frame that must be rendered on screen as the HUD icon. Usually an
     * animation.
     *
     * @return Frame
     */
    Frame updateIcon();
};

#endif
