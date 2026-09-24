/**
 * @file platform.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-05-25
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef PLATFORM_HH
#define PLATFORM_HH

#include <core/geometry.hh>
#include <vector>

class BoxCollider;

namespace pro2 {
class Window;
}

namespace Anim {
class Animator;
}

class Platform {
 private:
    int left_, right_, top_, bottom_;

    BoxCollider *boxCollider_;

    Anim::Animator *animator_;

    // static const std::vector<std::vector<int>> platform_texture_;
    // static const std::vector<std::vector<int>> platform_texture_border_;
    // static const std::vector<std::vector<int>> platform_texture_under_;
    // static const std::vector<std::vector<int>> platform_texture_under_border_;
    static const std::vector<std::vector<int>> platform_clear_mid_1_;
    // static const std::vector<std::vector<int>> platform_clear_mid_2_;
    // static const std::vector<std::vector<int>> platform_clear_mid_3_;
    static const std::vector<std::vector<int>> platform_clear_top;
    // static const std::vector<std::vector<int>> platform_clear_top_2_;
    static const std::vector<std::vector<int>> platform_clear_side;
    static const std::vector<std::vector<int>> platform_clear_border;

 public:
    Platform();

    Platform(const Platform& other);

    Platform(int left, int right, int top, int bottom);

    void paint(pro2::Window& window, bool debug) const;
    bool has_crossed_floor_downwards(pro2::Pt plast, pro2::Pt pcurr) const;
    bool has_crossed_platform(pro2::Pt plast, pro2::Pt pcurr) const;
    bool has_crossed_block_sideways(pro2::Pt plast, pro2::Pt pcurr) const;

    bool is_pt_inside(pro2::Pt pt) const;

    BoxCollider *getCollider() const;

    int top() const {
        return top_;
    }

    pro2::Rect get_rect() const {
        return {left_, top_, right_, bottom_};
    }
};

#endif