#include "managers/blockManager.hh"
#include <iostream>
#include <set>
#include "components/boxCollider.hh"
#include "core/finder.hh"
#include "graphics/window.hh"
#include "managers/entityManager.hh"
#include "mario.hh"

using namespace std;
using namespace pro2;

BlockManager::BlockManager() {
    // Initialize blocks_ with Block pointers
    blocks_ = {};
    const blockMap blockData = {
        {{256, 184}, BRICK},    {{272, 184}, BRICK},     {{288, 184}, BRICK},
        {{304, 184}, BRICK},    {{320, 184}, BRICK},     {{336, 184}, BRICK},
        {{352, 184}, BRICK},    {{368, 184}, BRICK},     {{384, 184}, BRICK},
        {{400, 184}, BRICK},

        {{304, 84}, BRICK},     {{320, 84}, QUESTION},   {{336, 84}, BRICK},

        {{836, 184}, BRICK},    {{850, 184}, BRICK},

        {{930, 184}, BRICK},    {{930, 168}, BRICK},     {{946, 184}, BRICK},
        {{962, 184}, BRICK},    {{978, 184}, BRICK},     {{994, 184}, BRICK},
        {{1010, 184}, BRICK},   {{1026, 184}, BRICK},    {{1042, 184}, BRICK},
        {{1058, 184}, BRICK},   {{1074, 184}, BRICK},    {{1090, 184}, BRICK},
        {{1106, 184}, BRICK},   {{1106, 168}, BRICK},

        {{994, 84}, QUESTION},  {{1010, 84}, BRICK},     {{1026, 84}, BRICK},
        {{1042, 84}, QUESTION},

        {{1186, 184}, BRICK},   {{1202, 184}, BRICK},

        {{1282, 184}, BRICK},   {{1298, 184}, BRICK},    {{1314, 184}, QUESTION},
        {{1330, 184}, BRICK},   {{1346, 184}, BRICK},

        {{2200, 184}, BRICK},   {{2216, 184}, BRICK},    {{2232, 184}, QUESTION},
        {{2248, 184}, BRICK},   {{2264, 184}, BRICK},

        {{2392, 184}, BRICK},   {{2408, 184}, BRICK},    {{2424, 184}, BRICK},
        {{2472, 184}, BRICK},   {{2488, 184}, BRICK},    {{2504, 184}, BRICK},
        {{2472, 102}, BRICK},   {{2488, 102}, BRICK},    {{2504, 102}, BRICK},
        {{2488, 22}, QUESTION},

        {{2552, 184}, BRICK},   {{2568, 184}, BRICK},    {{2584, 184}, BRICK},

        {{2840, 184}, BRICK},   {{2888, 184}, BRICK},    {{2936, 184}, BRICK},
        {{2888, 102}, BRICK},   {{2888, 22}, QUESTION},

        {{3884, 124}, BRICK},   {{3900, 124}, QUESTION}, {{3916, 124}, BRICK},

        {{3964, 216}, BRICK},   {{3980, 216}, BRICK},    {{3996, 216}, BRICK},
        {{4012, 216}, BRICK},   {{4028, 216}, BRICK},

        {{4076, 124}, BRICK},   {{4092, 124}, QUESTION}, {{4108, 124}, BRICK},

        {{5020, 216}, BRICK},   {{5020, 200}, BRICK},    {{5036, 216}, BRICK},
        {{5052, 216}, BRICK},   {{5068, 216}, BRICK},    {{5084, 216}, BRICK},
        {{5100, 216}, BRICK},   {{5116, 216}, BRICK},    {{5132, 216}, BRICK},
        {{5148, 216}, BRICK},   {{5164, 216}, BRICK},    {{5180, 216}, BRICK},
        {{5196, 216}, BRICK},   {{5212, 216}, BRICK},    {{5228, 216}, BRICK},
        {{5228, 200}, BRICK},

        {{5320, 216}, BRICK},   {{5320, 200}, BRICK},    {{5336, 216}, BRICK},
        {{5352, 216}, BRICK},   {{5368, 216}, BRICK},    {{5384, 216}, BRICK},
        {{5400, 216}, BRICK},   {{5416, 216}, BRICK},    {{5432, 216}, BRICK},
        {{5448, 216}, BRICK},   {{5464, 216}, BRICK},    {{5480, 216}, BRICK},
        {{5496, 216}, BRICK},   {{5512, 216}, BRICK},    {{5528, 216}, BRICK},
        {{5528, 200}, BRICK},

        {{5620, 128}, BRICK},   {{5620, 112}, BRICK},    {{5636, 128}, BRICK},
        {{5652, 128}, BRICK},   {{5668, 128}, BRICK},    {{5684, 128}, BRICK},
        {{5700, 128}, BRICK},   {{5716, 128}, BRICK},    {{5732, 128}, BRICK},
        {{5748, 128}, BRICK},   {{5764, 128}, BRICK},    {{5780, 128}, BRICK},
        {{5796, 128}, BRICK},   {{5812, 128}, BRICK},    {{5828, 128}, BRICK},
        {{5828, 112}, BRICK},

    };
    for (const auto& [pos, type] : blockData) {
        blocks_.push_back(new Block(pos, type));
    }

    f_blocks_ = new Finder<Block>();
    for (auto block : blocks_) {
        f_blocks_->add(block);
    }
}

void BlockManager::paint(pro2::Window& window, bool debug) {
    toPaint_ = f_blocks_->query(window.camera_rect());

    set<Block *>::iterator it = toPaint_.begin();
    while (it != toPaint_.end()) {
        (*it)->paint(window, debug);
        it++;
    }
}

void BlockManager::update(Mario *mario, Mario *luigi, EntityManager *gm_) {
    auto handle_player = [this](Mario *player, EntityManager *gm_) {
        if (!player) {
            return;
        }
        const int width =
            player->getCollider()->getRect().right - player->getCollider()->getRect().left;
        const int height =
            player->getCollider()->getRect().bottom - player->getCollider()->getRect().top;

        auto it = blocks_.begin();
        while (it != blocks_.end()) {
            if ((*it)->has_crossed_block_downwards(player->last_pos() + Pt{0, height / 2},
                                                   player->pos() + Pt{0, height / 2})) {
                int spriteHeight = height;
                player->set_y((*it)->get_rect().top - spriteHeight / 2);
                player->set_grounded(true);
            } else if ((*it)->has_crossed_block_upwards(player->last_pos() - Pt{0, height / 2},
                                                        player->pos() - Pt{0, height / 2})) {
                int spriteHeight = height;
                player->set_y((*it)->get_rect().bottom + spriteHeight / 2);
                player->set_y_accel(0);
                if ((*it)->getType() != COLLECTED) {
                    (*it)->headBumped();
                }
                if ((*it)->getType() == QUESTION) {
                    gm_->spawnEntity({(*it)->get_rect().left + 8, (*it)->get_rect().top - 8},
                                     MUSHROOM);
                    (*it)->changeType(COLLECTED);
                }
            } else if ((*it)->has_crossed_block_sideways(
                           {player->last_pos().x - width / 2 - 2, player->last_pos().y},
                           {player->pos().x - width / 2 - 2, player->pos().y}) ||
                       (*it)->has_crossed_block_sideways(
                           {player->last_pos().x + width / 2 + 2, player->last_pos().y},
                           {player->pos().x + width / 2 + 2, player->pos().y})) {
                if ((*it)->get_rect().left < player->pos().x && player->lookingLeft()) {
                    player->set_x((*it)->get_rect().right + width / 2);
                } else if ((*it)->get_rect().right > player->pos().x && !player->lookingLeft()) {
                    player->set_x((*it)->get_rect().left - width / 2);
                }
            }

            if ((*it)->isUp()) {
                (*it)->getDown();
            }

            ++it;
        }
    };

    handle_player(mario, gm_);
    handle_player(luigi, gm_);
}
