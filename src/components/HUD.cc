#include <components/HUD.hh>
#include <components/animator.hh>
#include <core/utils.hh>
#include <graphics/window.hh>
#include <iostream>
#include <sstream>

using namespace pro2;

HUD::HUD() : value_(0) {}

HUD::HUD(const Anim::Animation& anim)
    : icon_(new Anim::Animator(anim)), value_(0), spins_left_(0), pos_relative_to_camera_({0, 0}) {}

HUD::HUD(const Anim::Animation& anim, Pt pos_relative_to_camera)
    : icon_(new Anim::Animator(anim)),
      value_(0),
      spins_left_(0),
      pos_relative_to_camera_(pos_relative_to_camera) {}

HUD::HUD(const Anim::Animation& anim, int value)
    : icon_(new Anim::Animator(anim)),
      value_(value),
      spins_left_(0),
      pos_relative_to_camera_({0, 0}) {}

void HUD::updateValue(int value) {
    value_ = value;
}

int HUD::getValue() const {
    return value_ + spins_left_;
}

void HUD::plusOne() {
    spins_left_++;
}

void HUD::plusN(int n) {
    // for (int i = 0; i < n; i++) {
    //     plusOne();
    // }
    spins_left_ += n;
}

Frame HUD::updateIcon() {
    int orig_spins = spins_left_;
    if (spins_left_ > 0 && spins_left_ <= 5) {
        if (icon_->animationEnded()) {
            icon_->changeAnimationReset("idle");
            spins_left_--;
            value_++;

            // std::cout << "SPINS " << std::endl;
            // std::cout << spins_left_ << ' ' << value_ << std::endl;
        }
        return icon_->nextFrame();
    } else if (spins_left_ > 5 && spins_left_ <= 50) {
        if (icon_->animationEnded()) {
            icon_->changeAnimationReset("idle");
            value_ += 2;
            spins_left_ -= 2;
        }
        icon_->nextFrame();
        return icon_->nextFrame();
    } else if (spins_left_ > 50) {
        if (icon_->animationEnded()) {
            icon_->changeAnimationReset("idle");
            value_ += 5;
            spins_left_ -= 5;
        }
        icon_->nextFrame();
        icon_->nextFrame();
        return icon_->nextFrame();
    }

    return icon_->currentFrame();
}

void HUD::paint(pro2::Window& window) {
    Pt top_left = window.topleft();

    Frame frameToRender = updateIcon();

    paint_sprite(window, pos_relative_to_camera_ + top_left + Pt{1, 1}, frameToRender, false);
    Pt pos = {static_cast<int>(pos_relative_to_camera_.x + frameToRender[0].size() + 10),
              pos_relative_to_camera_.y + 2};

    Pt extra_offset = Pt{22 - static_cast<int>(frameToRender[0].size()), 0};

    paint_number(window, pos + top_left + extra_offset, value_);
    // std::cout << "SUCCESSFUL PAITN OF THE HUD " << std::endl;
}

void HUD::changeRelativePosition(pro2::Pt new_pos) {
    pos_relative_to_camera_ = new_pos;
}
