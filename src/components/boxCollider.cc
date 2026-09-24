#include "components/boxCollider.hh"
#include <core/utils.hh>
#include <iostream>
#include "graphics/window.hh"

using namespace pro2;
using namespace std;

// Not Safe
BoxCollider::BoxCollider() {
    topleft_ = {0, 0};
    bottomright_ = {0, 0};
    padding_ = 0;
    // cout << "DANGER: boxCollider uninitialized" << endl;
}

BoxCollider::BoxCollider(Pt top_left, Pt bottom_right, int padding) {
    rect_ = Rect{top_left.x, top_left.y, bottom_right.x, bottom_right.y};
    move(top_left, bottom_right);
    padding_ = padding;
}

BoxCollider::BoxCollider(Pt top_left, Pt bottom_right) {
    rect_ = Rect{top_left.x, top_left.y, bottom_right.x, bottom_right.y};
    move(top_left, bottom_right);
    padding_ = 0;
}

BoxCollider::BoxCollider(Rect rect) {
    rect_ = rect;
}

BoxCollider::BoxCollider(pro2::Rect rect, int padding) {
    padding_ = padding;
    rect_ = rect;
}

BoxCollider::BoxCollider(int left, int top, int right, int bottom) {
    BoxCollider(left, top, right, bottom, 0);
}

BoxCollider::BoxCollider(int left, int top, int right, int bottom, int padding) {
    rect_ = Rect{left, top, right, bottom};
    padding_ = padding;
}

void BoxCollider::printDebugLines(pro2::Window& window, int color) const {
    paint_vline(window, topleft_.x - padding_, topleft_.y - padding_, bottomright_.y + padding_,
                color);
    paint_vline(window, bottomright_.x + padding_, topleft_.y - padding_, bottomright_.y + padding_,
                color);

    paint_hline(window, topleft_.x - padding_, bottomright_.x + padding_, topleft_.y - padding_,
                color);
    paint_hline(window, topleft_.x - padding_, bottomright_.x + padding_, bottomright_.y + padding_,
                color);
}

bool BoxCollider::collidingWith(const BoxCollider *other) const {
    return !((topleft_.x - padding_ > other->bottomright_.x + other->padding_) ||
             (bottomright_.x + padding_ < other->topleft_.x - other->padding_) ||
             (topleft_.y - padding_ > other->bottomright_.y + other->padding_) ||
             (bottomright_.y + padding_ < other->topleft_.y - other->padding_));
}

bool BoxCollider::collidingWith(const pro2::Rect other) const {
    return !((topleft_.x - padding_ > other.right) || (bottomright_.x + padding_ < other.left) ||
             (topleft_.y - padding_ > other.bottom) || (bottomright_.y + padding_ < other.top));
}

void BoxCollider::move(Pt top_left, Pt bottom_right) {
    topleft_ = top_left;
    bottomright_ = bottom_right;
    rect_ = Rect{top_left.x, top_left.y, bottom_right.x, bottom_right.y};
}

void BoxCollider::update(Pt pos, const Frame& frame) {
    const int width = static_cast<int>(frame[0].size());
    const int height = static_cast<int>(frame.size());

    const Pt top_left = {pos.x - width / 2 - padding_, pos.y - height / 2 - padding_};

    const int bottom_right_x =
        (width % 2 == 0) ? pos.x + width / 2 - 1 + padding_ : pos.x + width / 2 + padding_;
    const int bottom_right_y =
        (height % 2 == 0) ? pos.y + height / 2 + padding_ - 1 : pos.y + height / 2 + padding_;

    const Pt bottom_right = {bottom_right_x, bottom_right_y};

    // cout << "SIZE : " << width << ' ' << height << endl;
    // cout << "POS : " << pos.x << ' ' << pos.y << endl;
    // cout << "TOPLEFT : " << top_left.x << ' ' << top_left.y << endl;
    // cout << "BOTTOMRIGHT : " << bottom_right.x << ' ' << bottom_right.y << endl;

    move(top_left, bottom_right);
}

void BoxCollider::setPadding(int padding) {
    padding_ = padding;
    // topleft_ = topleft_ - Pt{padding, padding};
    // bottomright_ = bottomright_ + Pt{padding, padding};
}

pro2::Rect BoxCollider::getRect() const {
    return rect_ + padding_;
}
