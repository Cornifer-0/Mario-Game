#include "world/block.hh"
#include "assets/animation_data.hh"
#include "components/animator.hh"
#include "components/boxCollider.hh"
#include "core/utils.hh"
#include "graphics/window.hh"

using namespace std;
using namespace pro2;
using namespace Anim;

Block::Block() {}

Block::Block(Pt topleft, BlockType type)
    : topleft_(topleft),
      boxCollider_(new BoxCollider({topleft.x, topleft.y}, {topleft.x + 16, topleft.y + 16})),
      type_(type) {
    if (type == BRICK) {
        animator_ = new Animator(Animation(AnimData::shining_block, false));

    } else if (type == QUESTION) {
        animator_ = new Animator(Animation(AnimData::question_block));
    } else if (type == COLLECTED) {
        animator_ = new Animator(Animation(AnimData::collected_block, false));
    }
}

void Block::paint(pro2::Window& window, bool debug) {
    paint_sprite(window, topleft_, animator_->nextFrame(), false);

    if (debug) {
        boxCollider_->printDebugLines(window);
    }
}

bool Block::has_crossed_block_downwards(pro2::Pt plast, pro2::Pt pcurr) const {
    if (pcurr.y <= plast.y) {
        return false;
    }

    if (boxCollider_->collidingWith(new BoxCollider{plast, pcurr})) {
        return true;
    }
    return false;
}

bool Block::has_crossed_block_upwards(pro2::Pt plast, pro2::Pt pcurr) const {
    if (pcurr.y >= plast.y) {
        return false;
    }

    if (boxCollider_->collidingWith(new BoxCollider{pcurr, plast})) {
        return true;
    }
    return false;
}

bool Block::has_crossed_block_sideways(pro2::Pt plast, pro2::Pt pcurr) const {
    if (pcurr.x == plast.x) {
        return false;
    }

    // Check collision along the horizontal movement
    pro2::Pt from = plast;
    pro2::Pt to = pcurr;
    if (boxCollider_->collidingWith(new BoxCollider{from, to})) {
        return true;
    }
    return false;
}

BoxCollider *Block::getCollider() const {
    return boxCollider_;
}

pro2::Rect Block::get_rect() const {
    return boxCollider_->getRect();
}

void Block::headBumped() {
    if (up_) {
        return;
    }
    animator_->changeAnimationReset("idle");
    up_ = true;
    frames_up_ = UP_TIME;
    topleft_ = topleft_ - Pt{0, PIXELS_UP};
    boxCollider_->move(topleft_, topleft_ + Pt{16, 16});
}

bool Block::isUp() const {
    return up_;
}

void Block::getDown() {
    if (up_ && frames_up_ <= 0) {
        up_ = false;
        topleft_ = topleft_ + Pt{0, PIXELS_UP};
        boxCollider_->move(topleft_, topleft_ + Pt{16, 16});
    } else {
        // cout << 'f' << endl;
        frames_up_--;
    }
}

BlockType Block::getType() const {
    return type_;
}

void Block::changeType(BlockType type) {
    if (type == COLLECTED) {
        animator_ = new Animator(Animation(AnimData::collected_block, false));
        type_ = COLLECTED;
    }
}