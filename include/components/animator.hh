/**
 * @file animator.hh
 * @brief
 * @version 0.1
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef ANIMATOR_HH
#define ANIMATOR_HH

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "core/geometry.hh"

/**
 * @brief The class Anim manager everything to do with sprite animations. Mainly the classes
 * Animation and Animator.
 */
namespace Anim {

typedef std::vector<std::vector<int>> Frame;
typedef std::vector<Frame>            AnimationFrame;

/**
 * @brief Class Animation let's you construct a simple sprite animation from an AnimationFrame
 * (vector<Frame>)
 *
 */
class Animation {
 private:
    int            duration_;  // in frames
    AnimationFrame frames_;
    int            currentFrame_;
    bool           repeat_;

 public:
    /**
     * @brief Constructs an empty animatoin,
     */
    Animation();

    /**
     * @brief Constructs an animation with the AnimationFrame.
     * @param AF
     */
    Animation(const AnimationFrame& AF);

    /**
     * @brief Constructs an animation with an AnimationFrame with extra parameters repeat and
     * duration.
     *
     * @param AF
     * @param duration
     */
    Animation(const AnimationFrame& AF, bool repeat, int duration);

    /**
     * @brief Constructs an animation with an AnimationFrame with extra parameter
     * repeat.
     *
     * @param AF
     * @param duration
     */
    Animation(const AnimationFrame& AF, bool repeat);

    /**
     * @brief Returns the duration of the animation.
     *
     * @return int
     */
    int getDuration() const;

    /**
     * @brief Returns the number of frames of the animation.
     *
     * @return int
     */
    int getNFrames() const;

    /**
     * @brief Returns true only if the animation has ended. This only happens when repeat is false
     * and the last frame of the AnimationFrame has been reached.
     *
     * @return true
     * @return false
     */
    bool animationEnded() const;

    /**
     * @brief Restarts the animation. (Sets the currentFrame to 0)
     *
     */
    void restart();

    /**
     * @brief Returns the current Frame.
     *
     * @return Frame
     */
    Frame currentFrame() const;

    /**
     * @brief Returns the next Frame in the animation, If the currentFrame is the last frame of the
     * AnimationFrame, returns the last frame in case repeat is false, the first frame otherwise.
     *
     * @return Frame
     */
    Frame nextFrame();  // returns next frame
};

/**
 * @brief The class animator a container for storing the different animations an object might have.
 *
 */
class Animator {
 private:
    std::map<std::string, Animation> anims_;
    std::string                      currentAnim_;
    uint64_t                         prevFrame_;

 public:
    /**
     * @brief Construct an empty Animator with no Animations.
     *
     */
    Animator();

    /**
     * @brief Constructs an Animator with the first and only animation anim, being named as "idle"
     * by default.
     *
     * @param anim
     */
    Animator(const Animation& anim);

    /**
     * @brief Adds an animation to the anims_ map, the the name name.
     *
     * @param name
     * @param anim
     */
    void addAnimation(std::string name, const Animation& anim);

    /**
     * @brief Changes the currentAnim_ to name and sets currentFrame_ to 0. In case of currentAnim_
     * being equal to name in the first place, the method does nothing.
     *
     * @param name
     */
    void changeAnimation(std::string name);

    /**
     * @brief Changes the currentAnim_ to name and sets currentFrame_ to 0. In case of currentAnim_
     * being equal to name it still resets the animation by setting currentFrame_ to 0.
     *
     * @param name
     */
    void changeAnimationReset(std::string name);

    /**
     * @brief Returns the currentAnim_ name.
     *
     * @return std::string
     */
    std::string getAnimation() const;

    /**
     * @brief spawnsParticlesOfAnimation, Spawns the particle system with the name name at a
     * position pos.
     *
     * @return Animation*
     */
    void spawnParticleOfAnimation(pro2::Pt pos, std::string name) const;

    /**
     * @brief Returns true if and only if the animation has ended. For this the anims_[currentAnim_]
     * repeat attribute must be false and anims_[currentAnim_] currentFrame must be the last of the
     * AnimationFrame.
     *
     * @return true
     * @return false
     */
    bool animationEnded() const;

    /**
     * @brief Returns the currentFrame of the currentAnimation
     *
     * @return Frame
     */
    Frame currentFrame() const;

    /**
     * @brief Returns the nextFrame of the animation and sets prevFrame_ to 0 if and only if
     * prevFrame_ >= than the duration of the current animation. If that is not the case returns the
     * currentFrame and prevFrame_ gets incremented by one.
     *
     * @return Frame
     */
    Frame nextFrame();
};

}  // namespace Anim

#endif