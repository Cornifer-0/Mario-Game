/**
 * @file boxCollider.hh
 * @brief
 * @version 0.1
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef BOXCOLLIDER_HH
#define BOXCOLLIDER_HH

#include <vector>
#include "core/geometry.hh"

typedef std::vector<std::vector<int>> Frame;

// Forward declarations to reduce dependencies
namespace pro2 {
class Window;
}  // namespace pro2

/**
 * @brief
 *
 */
class BoxCollider {
 private:
    pro2::Rect rect_;
    pro2::Pt   topleft_, bottomright_;
    int        padding_;

 public:
    /**
     * @brief Constructs an empty boxCollider. Not safe, should only be used temporarlly before
     * updating it's attributes.
     */
    BoxCollider();

    /**
     * @brief Construct a new boxCollider with the Pt's topleft and bottomright.
     *
     * @param top_left
     * @param bottom_right
     */
    BoxCollider(pro2::Pt top_left, pro2::Pt bottom_right);

    /**
     * @brief Construct a new boxCollider with the Pt's topleft and bottomright. The boxCollider is
     * augmented in all sides by padding.
     *
     * @param top_left
     * @param bottom_right
     * @param padding
     */
    BoxCollider(pro2::Pt top_left, pro2::Pt bottom_right, int padding);

    /**
     * @brief Construct a new boxCollider using a rect
     *
     */
    BoxCollider(pro2::Rect);

    /**
     * @brief Construct a new boxCollider using rect and padding
     *
     * @param padding
     */
    BoxCollider(pro2::Rect, int padding);

    /**
     * @brief Construct a new box Collider object
     *
     * @param left
     * @param top
     * @param right
     * @param bottom
     */
    BoxCollider(int left, int top, int right, int bottom);

    /**
     * @brief Construct a new box Collider object
     *
     * @param left
     * @param top
     * @param right
     * @param bottom
     * @param padding
     */
    BoxCollider(int left, int top, int right, int bottom, int padding);

    /**
     * @brief It updates the position of the boxCollider using the position of the objecte and a
     * Frame(sprite), so it can tell the width and height it must have.
     *
     * @param pos
     * @param frame
     */
    void update(pro2::Pt pos, const Frame& frame);

    /**
     * @brief Set the Padding, boxCollider should be updated for the effects of this change to take
     * place.
     *
     * @param padding
     */
    void setPadding(int padding);

    /**
     * @brief It changes the top_left and bottom_right attributes of the boxCollider.
     *
     * @param top_left
     * @param bottom_right
     */
    void move(pro2::Pt top_left, pro2::Pt bottom_right);

    /**
     * @brief Prints debugging square in the screen of color green by default. This square marks the
     * borders of the boxCollider.
     *
     * @param window
     */
    void printDebugLines(pro2::Window& window, int color = 0x00FF00) const;

    /**
     * @brief Returns true if and only if the boxCollider collides with other.
     *
     * @param other
     * @return true
     * @return false
     */
    bool collidingWith(const BoxCollider *other) const;
    bool collidingWith(const pro2::Rect other) const;

    /**
     * @brief Get the Rect object
     *
     * @return pro2::Rect
     */
    pro2::Rect getRect() const;
};

#endif