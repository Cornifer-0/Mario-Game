#include "components/animator.hh"
#include <components/particleSystem.hh>

#include <iostream>
using namespace std;

namespace Anim {

Animation::Animation() : currentFrame_(0), duration_(4), repeat_(true) {}

Animation::Animation(const AnimationFrame& AF)
    : frames_(AF), currentFrame_(0), duration_(4), repeat_(true) {}

Animation::Animation(const AnimationFrame& AF, bool repeat)
    : frames_(AF), currentFrame_(0), duration_(4), repeat_(repeat) {}

Animation::Animation(const AnimationFrame& AF, bool repeat, int duration)
    : frames_(AF), duration_(duration), repeat_(repeat), currentFrame_(0) {}

Frame Animation::nextFrame() {
    // cout << "Current frame : " << ' ' << currentFrame_ << endl;
    if (!repeat_ && animationEnded()) {
        // cout << "ANIM ENDED" << endl;
        return currentFrame();
    }

    currentFrame_++;
    // cout << currentFrame_ << ' ' << frames_.size() << endl;
    if (currentFrame_ >= frames_.size()) {
        if (repeat_) {
            // if (frames_.size() == 3) {
            //     cout << "REPEAT ?" << endl;
            // }
            currentFrame_ = 0;
        }
    }
    return frames_[currentFrame_];
    // TODO: boomerang or repeat
}

bool Animation::animationEnded() const {
    return !repeat_ && (currentFrame_ == frames_.size() - 1);
}

Frame Animation::currentFrame() const {
    return frames_.at(currentFrame_);
}

int Animation::getDuration() const {
    return duration_;
}

int Animation::getNFrames() const {
    return frames_.size();
}

void Animation::restart() {
    currentFrame_ = 0;
}

Animator::Animator() {
    prevFrame_ = 0;
    currentAnim_ = "undefined";
}

Animator::Animator(const Animation& anim) {
    prevFrame_ = 0;
    addAnimation("idle", anim);
    anims_[currentAnim_] = anim;
}

void Animator::addAnimation(std::string name, const Animation& anim) {
    if (currentAnim_ == "undefined") {
        currentAnim_ = name;
    }
    anims_[name] = anim;
}

void Animator::changeAnimation(std::string name) {
    if (currentAnim_ == name) {
        return;
    }
    if (anims_.find(name) != anims_.end()) {
        currentAnim_ = name;
        // cout << "animation changed to " << name << endl;
        // prevFrame_ = 0;
    } else {
        cout << "ANIMATION NOT FOUND" << endl;
    }
}

void Animator::changeAnimationReset(std::string name) {
    changeAnimation(name);
    anims_[name].restart();
}

bool Animator::animationEnded() const {
    return anims_.at(currentAnim_).animationEnded();
}

Frame Animator::currentFrame() const {
    return anims_.at(currentAnim_).currentFrame();
}

Frame Animator::nextFrame() {
    // cout << currentAnim_.getNFrames() << ' ' << window.frame_count() << endl;
    // cout << 'a' << window.frame_count() << ' ' << 20 << endl;
    if (prevFrame_ >= anims_[currentAnim_].getDuration()) {
        // cout << "in" << endl;
        prevFrame_ = 0;

        return anims_[currentAnim_].nextFrame();
    }
    //  cout << "out" << endl;
    prevFrame_++;
    // cout << prevFrame_ << ' ' << anims_[currentAnim_].getDuration() << endl;
    return anims_[currentAnim_].currentFrame();
}

void Animator::spawnParticleOfAnimation(pro2::Pt pos, string name) const {
    ParticleSystemManager::getInstance().spawn(ParticleSystem(pos, anims_.at(name)));
}

string Animator::getAnimation() const {
    return currentAnim_;
}

}  // namespace Anim