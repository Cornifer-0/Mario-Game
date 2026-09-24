#include <assets/animation_data.hh>
#include <components/animator.hh>
#include <components/boxCollider.hh>
#include <components/particleSystem.hh>
#include <core/utils.hh>
#include <enemies/entity.hh>
#include <graphics/window.hh>
#include <iostream>
#include "world/block.hh"
#include "world/platform.hh"

using namespace pro2;
using namespace std;
using namespace Anim;

Entity::Entity() {}

Entity::Entity(pro2::Pt pos) {
    Entity(pos, GOOMBA);
}

Entity::Entity(pro2::Pt pos, EntityType tp) {
    pos_ = pos;
    last_pos_ = pos;
    type_ = tp;
    speed_ = accel_ = {0, 0};

    if (tp == GOOMBA) {
        animator_ = new Animator(Animation(AnimData::goomba_walk, 4));
        animator_->addAnimation("death", Animation(AnimData::goomba_death, false, 30));
    } else if (tp == ANGRY) {
        animator_ = new Animator(Animation(AnimData::angry_goomba_walk, 8));
        animator_->addAnimation("death", Animation(AnimData::angry_goomba_death, false, 30));
    } else if (tp == KOOPA) {
        animator_ = new Animator(Animation(AnimData::koopa_walking, 8));
        animator_->addAnimation("death", Animation(AnimData::angry_goomba_death, false, 30));
    } else if (tp == MUSHROOM) {
        int randomMushroomIndex = rand() % (AnimData::mushrooms.size());
        animator_ = new Animator(Animation({AnimData::mushrooms[randomMushroomIndex]}));
        animator_->addAnimation("death", Animation(AnimData::blue_shine_1_, false));
    }

    bodyCollider_ = new BoxCollider();
    headCollider_ = new BoxCollider();
    visibilityCollider_ = new BoxCollider();

    bodyCollider_->setPadding(2);
    headCollider_->setPadding(-1);

    grounded_ = true;

    int dice = rand() % 2;

    if (dice == 0) {
        looking_left_ = false;
    } else {
        looking_left_ = true;
    }
}

void Entity::update() {
    last_pos_ = pos_;

    // calculates the falling goomba
    update_horizontal_movement();
    update_collider();
}

void Entity::changeDirection() {
    looking_left_ = (looking_left_) ? false : true;
    speed_.x = -speed_.x;
}

void Entity::update_collider() {
    Frame firstFrame = animator_->currentFrame();
    int   width = firstFrame[0].size();
    int   height = firstFrame.size();
    Pt    top_left = pos_ - Pt{width / 2, height / 2} + Pt{0, 15};
    Pt    bottom_right = pos_ + Pt{width / 2, height / 2};

    Pt top_left_damagable = top_left - Pt{0, 15};
    Pt bottom_right_damagable = {bottom_right.x, top_left.y};

    // cout << "STUFF " << endl;
    // cout << top_left_damagable.x << ' ' << top_left_damagable.y << endl;
    // cout << bottom_right_damagable.x << ' ' << bottom_right_damagable.y << endl;
    headCollider_->move(top_left_damagable, bottom_right_damagable);

    bodyCollider_->move(top_left, bottom_right);

    visibilityCollider_->move(top_left - Pt{150, 0}, bottom_right + Pt{150, 0});
}

void Entity::update_horizontal_movement() {
    int direction = (looking_left_) ? -velocity_ : velocity_;
    if (type_ == ANGRY) {
        direction *= 2;
    }

    if (mario_in_sight_) {
        direction *= 1.5;
        // cout << "mario in sight" << endl;
    }

    if (grounded_) {
        speed_.x = direction;
    }
}

void Entity::check_collisions(const std::vector<Platform *> platforms,
                              const std::set<Block *>&      blocks) {
    apply_physics();

    const Frame curr = animator_->currentFrame();
    const int   height = static_cast<int>(curr.size());
    const int   width = static_cast<int>(curr[0].size());
    for (const Platform *platform : platforms) {
        if (platform->has_crossed_floor_downwards({last_pos_.x, last_pos_.y + height / 2 - 2},
                                                  {pos_.x, pos_.y + height / 2 - 2})) {
            grounded_ = true;
            pos_.y = platform->top() - height / 2 + 1;
        }
    }

    for (const Block *block : blocks) {
        if (block->has_crossed_block_downwards({last_pos_.x, last_pos_.y + height / 2 - 2},
                                               {pos_.x, pos_.y + height / 2 - 2})) {
            grounded_ = true;
            pos_.y = block->get_rect().top - height / 2 + 1;
        }
    }

    for (const Platform *platform : platforms) {
        if ((platform->has_crossed_platform(last_pos_ - Pt{width / 2, 0},
                                            pos_ - Pt{width / 2, 0}) &&
             looking_left_) ||
            (platform->has_crossed_platform(last_pos_ + Pt{width / 2, 0},
                                            pos_ + Pt{width / 2, 0}) &&
             !looking_left_)) {
            changeDirection();
            // cout << "collided looking left: " << looking_left_ << " speed: " << speed_.x << endl;
        }
    }
}

void Entity::apply_physics() {
    if (grounded_) {
        speed_.y = 0;
        accel_.y = 0;
    }

    // Always falling to check if we aren't grounded
    // If we are, we will return to the same spot

    const int gravity = 2;  // gravity = 1 pixel / frame_time^2
    speed_.y += gravity;

    if (accel_time_ > 0) {
        speed_.y += accel_.y;
        accel_time_--;
    }

    pos_.x += speed_.x;
    pos_.y += speed_.y;
}

void Entity::paint(pro2::Window& window, bool debug) {
    if (!bodyCollider_->collidingWith(window.camera_rect() + 100)) {
        return;
    }
    // Frame    animFrame = animator_->nextFrame();
    // const Pt top_left = {pos_.x - static_cast<int>(animFrame[0].size() / 2),
    //                      pos_.y - static_cast<int>(animFrame.size() / 2)};
    // paint_sprite(window, top_left, animFrame, (looking_left_) ? false : true);

    paint_sprite_center(window, pos_, animator_->nextFrame(), !looking_left_);

    if (debug) {
        bodyCollider_->printDebugLines(window, 0xFF0000);
        headCollider_->printDebugLines(window);
        // visibilityCollider_->printDebugLines(window, 0x0000FF);
        window.set_pixel(pos_, red);
    }
}

BoxCollider *Entity::getHeadCollider() const {
    return headCollider_;
}

BoxCollider *Entity::getBodyCollider() const {
    return bodyCollider_;
}

BoxCollider *Entity::getVisibilityCollider() const {
    return visibilityCollider_;
}

Pt Entity::pos() const {
    return pos_;
}

bool Entity::looking_left() const {
    return looking_left_;
}

void Entity::set_mario_in_sight(bool b) {
    mario_in_sight_ = b;
}

void Entity::die() {
    if (type_ == GOOMBA || type_ == KOOPA || type_ == ANGRY) {
        animator_->spawnParticleOfAnimation(pos_ + Pt{0, 8}, "death");
    } else {
        animator_->spawnParticleOfAnimation(pos_, "death");
    }
}

EntityType Entity::getType() const {
    return type_;
}

pro2::Rect Entity::get_rect() const {
    return bodyCollider_->getRect();
}
