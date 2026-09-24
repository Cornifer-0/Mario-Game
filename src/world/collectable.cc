#include "world/collectable.hh"
#include <cmath>  // For mathematical constants and functions
#include "assets/animation_data.hh"
#include "components/animator.hh"
#include "components/boxCollider.hh"
#include "components/particleSystem.hh"
#include "core/utils.hh"

#ifndef PI
#define PI 3.14159265358979323846
#endif

using namespace std;
using namespace pro2;

Collectable::Collectable(pro2::Pt pos) {
    Collectable(pos, SMALL_COIN);
}

Collectable::Collectable(pro2::Pt pos, COLLECTABLE_TYPE ct) : boxCollider_(new BoxCollider) {
    type_ = ct;
    pos_ = last_pos_ = pos;
    if (type_ == SMALL_COIN) {
        animator_ = new Anim::Animator(Anim::Animation(AnimData::wonder_small_coin));
        animator_->addAnimation("collected", Anim::Animation(AnimData::blue_shine_1_, false));
    } else if (type_ == COIN) {
        animator_ = new Anim::Animator(Anim::Animation(AnimData::wonder_coin));
        animator_->addAnimation("collected", Anim::Animation(AnimData::blue_shine_1_, false));
    } else if (type_ == END_STAR) {
        animator_ = new Anim::Animator(Anim::Animation(AnimData::end_star));
        animator_->addAnimation("collected", Anim::Animation(AnimData::blue_shine_1_, false));
    }

    const Frame animFrame = animator_->currentFrame();
    if (type_ != END_STAR) {
        boxCollider_->setPadding(2);
    } else {
        boxCollider_->setPadding(-5);
    }
    boxCollider_->update(pos_, animFrame);
}

void Collectable::paint(pro2::Window& window, bool debug) {
    // cout << "PAINTING sprite at " << position_.x << ' ' << position_.y << endl;
    // Frame animFrame = animator_->nextFrame();
    Pt offset = calcPositionOffset(window);

    // cout << "COIN AT: "<< abs(static_cast<int>(position_.x - animFrame.size()) )<< ' ' <<
    // abs(position_.y - animFrame[0].size()) << endl;
    // const Pt top_left = {(static_cast<int>(position_.x - animFrame[0].size() / 2)),
    //                      (static_cast<int>(position_.y - animFrame.size() / 2))};
    // paint_sprite(window, top_left + offset, animFrame, false);

    paint_sprite_center(window, pos_ + offset, animator_->nextFrame(), false);

    boxCollider_->update(pos_ + offset, animator_->currentFrame());

    if (debug) {
        boxCollider_->printDebugLines(window);
        window.set_pixel(pos_, red);
    }
}

BoxCollider *Collectable::getCollider() const {
    return boxCollider_;
}

void Collectable::Destroy() {
    collected_ = true;
    animator_->spawnParticleOfAnimation(pos_ + Pt{0, 8}, "collected");
}

COLLECTABLE_TYPE Collectable::getType() const {
    return type_;
}

Pt Collectable::calcPositionOffset(const pro2::Window& window) const {
    int duration = 100;
    int amplitude = 5;

    // cout << "FRAME COUNT " << window.frame_count() << endl;
    // cout << (window.frame_count() % (2 * duration)) << endl;
    // cout << sin((window.frame_count() % (2 * duration)) - duration) << endl;

    int extraY = amplitude * sin(2.0 * PI * (double(window.frame_count() % duration) / duration));

    // int extraX = amplitude * cos(2.0 * PI * (double(window.frame_count() % duration) /
    // duration));

    return {0, extraY};
}

pro2::Rect Collectable::get_rect() {
    return boxCollider_->getRect();
}

bool Collectable::isDestroyed() const {
    return collected_;
}
