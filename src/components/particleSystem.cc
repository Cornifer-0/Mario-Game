#include <algorithm>
#include <components/particleSystem.hh>
#include <core/utils.hh>
#include <graphics/window.hh>
#include <iostream>

using namespace Anim;
using namespace pro2;
using namespace std;

ParticleSystem::ParticleSystem(pro2::Pt pos, const Anim::Animation& anim) {
    animator_ = new Animator(anim);
    pos_ = pos;
}

void ParticleSystem::paint(pro2::Window& window) {
    // Frame currentFrame = animator_->currentFrame();
    // int   width = currentFrame[0].size();
    // int   height = currentFrame.size();
    // Pt    top_left = pos_ - Pt{width / 2, height / 2};
    // paint_sprite(window, top_left, animator_->nextFrame(), false);

    paint_sprite_center(window, pos_, animator_->nextFrame(), false);
}

bool ParticleSystem::hasEnded() const {
    return animator_->animationEnded();
}

void ParticleSystem::die() {
    dead_ = true;
}

void ParticleSystemManager::spawn(const ParticleSystem& ps) {
    particles_.push_back(ps);
}

void ParticleSystemManager::paint(pro2::Window& window) {
    for (ParticleSystem& ps : particles_) {
        ps.paint(window);
    }

    // Remove all destroyed collectables
    particles_.erase(remove_if(particles_.begin(), particles_.end(),
                               [](const ParticleSystem& c) { return c.hasEnded(); }),
                     particles_.end());
}
