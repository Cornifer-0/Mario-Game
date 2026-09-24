/**
 * @file block.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-05-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef BLOCK_HH
#define BLOCK_HH

#include <vector>
#include "core/geometry.hh"

class BoxCollider;

namespace Anim {
class Animator;
}

namespace pro2 {
class Window;
}

enum BlockType { BRICK, QUESTION, COLLECTED };

class Block {
 private:
    pro2::Pt  topleft_;
    BlockType type_;

    BoxCollider *boxCollider_;
    int          frames_up_ = 0;
    bool         up_ = false;

    Anim::Animator *animator_;

    const int PIXELS_UP = 5;
    const int UP_TIME = 5;

 public:
    /**
     * @brief Construct a new Block object
     *
     */
    Block();

    /**
     * @brief Construct a new Block object at with topleft being the topleft position of the block
     * and being of type type.
     *
     * @param topleft
     * @param type
     */
    Block(pro2::Pt topleft, BlockType type);

    /**
     * @brief Get the rect object
     *
     * @return pro2::Rect
     */
    pro2::Rect get_rect() const;

    /**
     * @brief Returns true only if when you draw a line from plast to pcurr it crosses the collider
     * of the block downwards.
     *
     * @param plast
     * @param pcurr
     * @return true
     * @return false
     */
    bool has_crossed_block_downwards(pro2::Pt plast, pro2::Pt pcurr) const;

    /**
     * @brief Returns true only if when you draw a line from plast to pcurr it crosses the collider
     * of the block upwards.
     *
     * @param plast
     * @param pcurr
     * @return true
     * @return false
     */
    bool has_crossed_block_upwards(pro2::Pt plast, pro2::Pt pcurr) const;

    /**
     * @brief Returns true only if when you draw a line from plast to pcurr it crosses the collider
     * of the block sideways.
     *
     * @param plast
     * @param pcurr
     * @return true
     * @return false
     */
    bool has_crossed_block_sideways(pro2::Pt plast, pro2::Pt pcurr) const;

    /**
     * @brief Paints the block on window.
     *
     * @param window
     * @param debug
     */
    void paint(pro2::Window& window, bool debug);

    /**
     * @brief Changes the type of Block to a new type.
     *
     * @param type
     */
    void changeType(BlockType type);

    /**
     * @brief Get the Collider object
     *
     * @return boxCollider*
     */
    BoxCollider *getCollider() const;

    /**
     * @brief plays a little animation of the block going up and down. Using the helper functions
     * isUp() and getDow().
     *
     */
    void headBumped();
    bool isUp() const;
    void getDown();

    /**
     * @brief Get the Type object
     *
     * @return BlockType
     */
    BlockType getType() const;
};

#endif