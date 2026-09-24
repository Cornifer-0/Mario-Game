/**
 * @file blockManager.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-05-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef BLOCKMANAGER_HH
#define BLOCKMANAGER_HH

#include <set>
#include <utility>
#include <vector>
#include "core/geometry.hh"
#include "world/block.hh"

template <typename T>
class Finder;

class Mario;
class EntityManager;

typedef std::vector<std::pair<pro2::Pt, BlockType>> blockMap;

/**
 * @brief Class for managing blocks
 *
 */
class BlockManager {
 private:
    std::vector<Block *> blocks_;
    std::set<Block *>    toPaint_;
    Finder<Block>       *f_blocks_;

 public:
    /**
     * @brief Construct a new Block Manager object
     *
     */
    BlockManager();
    /**
     * @brief Paints all the blocks on the screen (all that are inside the screen thanks to the
     * finder)
     *
     * @param window
     * @param debug
     */
    void paint(pro2::Window& window, bool debug);

    /**
     * @brief Updates mario so he interacts with the blocks.
     *
     * @param mario
     */
    void update(Mario *mario, Mario *luigi, EntityManager *gm_);

    inline const std::vector<Block *>& get_blocks() const {
        return blocks_;
    }

    inline const std::set<Block *>& get_blocks_f(pro2::Window& window) const {
        return toPaint_;
    }

    //  private:
    // const blockMap defaultBlockMap = []() {
    //     blockMap map;
    //     int      rows = 5;   // Number of rows
    //     int      cols = 10;  // Number of columns
    //     int      startX = 100;
    //     int      startY = 100;
    //     for (int row = 0; row < rows; ++row) {
    //         for (int col = 0; col < cols; ++col) {
    //             map.push_back({{startX + col * 16, startY + row * 16}, QUESTION});
    //         }
    //     }
    //     return map;
    // }();

    // const blockMap defaultBlockMap = {
    //     {{200, 100}, BRICK}, {{216, 100}, BRICK}, {{232, 100}, QUESTION}, {{248, 100}, BRICK},
    //     {{264, 100}, BRICK}, {{280, 150}, BRICK}, {{296, 150}, BRICK},    {{312, 150}, BRICK},
    //     {{168, 50}, BRICK},  {{152, 50}, BRICK},  {{136, 50}, BRICK}};

    // const blockMap juditMap = {
    //     {{256, 184}, BRICK},    {{272, 184}, BRICK},     {{288, 184}, BRICK},
    //     {{304, 184}, BRICK},    {{320, 184}, BRICK},     {{336, 184}, BRICK},
    //     {{352, 184}, BRICK},    {{368, 184}, BRICK},     {{384, 184}, BRICK},
    //     {{400, 184}, BRICK},

    //     {{304, 84}, BRICK},     {{320, 84}, QUESTION},   {{336, 84}, BRICK},

    //     {{836, 184}, BRICK},    {{850, 184}, BRICK},

    //     {{930, 184}, BRICK},    {{930, 168}, BRICK},     {{946, 184}, BRICK},
    //     {{962, 184}, BRICK},    {{978, 184}, BRICK},     {{994, 184}, BRICK},
    //     {{1010, 184}, BRICK},   {{1026, 184}, BRICK},    {{1042, 184}, BRICK},
    //     {{1058, 184}, BRICK},   {{1074, 184}, BRICK},    {{1090, 184}, BRICK},
    //     {{1106, 184}, BRICK},   {{1106, 168}, BRICK},

    //     {{994, 84}, QUESTION},  {{1010, 84}, BRICK},     {{1026, 84}, BRICK},
    //     {{1042, 84}, QUESTION},

    //     {{1186, 184}, BRICK},   {{1202, 184}, BRICK},

    //     {{1282, 184}, BRICK},   {{1298, 184}, BRICK},    {{1314, 184}, QUESTION},
    //     {{1330, 184}, BRICK},   {{1346, 184}, BRICK},

    //     {{2500, 184}, BRICK},   {{2516, 184}, BRICK},    {{2532, 184}, BRICK},
    //     {{2500, 84}, QUESTION},

    //     {{3884, 124}, BRICK},   {{3900, 124}, QUESTION}, {{3916, 124}, BRICK},

    //     {{3964, 216}, BRICK},   {{3980, 216}, BRICK},    {{3996, 216}, BRICK},
    //     {{4012, 216}, BRICK},   {{4028, 216}, BRICK},

    //     {{4076, 124}, BRICK},   {{4092, 124}, QUESTION}, {{4108, 124}, BRICK},

    // {{1400, 184}, BRICK},
    // {{1416, 184}, BRICK},
    // {{1432, 184}, BRICK},
    // {{1448, 184}, BRICK},
    // {{1464, 184}, BRICK},
    // {{1480, 184}, QUESTION},
    // {{1496, 184}, BRICK},
    // {{1512, 184}, BRICK},
    // {{1528, 184}, BRICK},
    // {{1544, 184}, BRICK},

    // {{1600, 184}, BRICK},
    // {{1616, 184}, BRICK},
    // {{1632, 184}, BRICK},
    // {{1648, 184}, QUESTION},
    // {{1664, 184}, BRICK},
    // {{1680, 184}, BRICK},

    // {{1750, 184}, BRICK},
    // {{1766, 184}, BRICK},
    // {{1782, 184}, BRICK},
    // {{1798, 184}, BRICK},
    // {{1814, 184}, BRICK},
    // {{1830, 184}, BRICK},

    // {{1900, 184}, QUESTION},
    // {{1916, 184}, BRICK},
    // {{1932, 184}, BRICK},
    // {{1948, 184}, BRICK},
    // {{1964, 184}, BRICK},

    // {{2000, 184}, BRICK},
    // {{2016, 184}, BRICK},
    // {{2032, 184}, BRICK},
    // {{2048, 184}, BRICK},
    // {{2064, 184}, BRICK},

    // {{2100, 184}, BRICK},
    // {{2116, 184}, QUESTION},
    // {{2132, 184}, BRICK},
    // {{2148, 184}, BRICK},
    // {{2164, 184}, BRICK},

    // {{2200, 184}, BRICK},
    // {{2216, 184}, BRICK},
    // {{2232, 184}, BRICK},
    // {{2248, 184}, BRICK},
    // {{2264, 184}, BRICK},

    // {{2300, 184}, BRICK},
    // {{2316, 184}, BRICK},
    // {{2332, 184}, BRICK},
    // {{2348, 184}, BRICK},
    // {{2364, 184}, BRICK},

    // {{2400, 184}, BRICK},
    // {{2416, 184}, BRICK},
    // {{2432, 184}, BRICK},
    // {{2448, 184}, BRICK},
    // {{2464, 184}, BRICK},

    // {{2550, 184}, BRICK},
    // {{2566, 184}, BRICK},
    // {{2582, 184}, BRICK},
    // {{2598, 184}, BRICK},
    // {{2614, 184}, BRICK},

    // {{2650, 184}, BRICK},
    // {{2666, 184}, BRICK},
    // {{2682, 184}, BRICK},
    // {{2698, 184}, BRICK},
    // {{2714, 184}, BRICK},

    // {{2750, 184}, BRICK},
    // {{2766, 184}, BRICK},
    // {{2782, 184}, BRICK},
    // {{2798, 184}, BRICK},
    // {{2814, 184}, BRICK},

    // {{2850, 184}, BRICK},
    // {{2866, 184}, BRICK},
    // {{2882, 184}, BRICK},
    // {{2898, 184}, BRICK},
    // {{2914, 184}, BRICK},

    // {{2950, 184}, BRICK},
    // {{2966, 184}, BRICK},
    // {{2982, 184}, BRICK},
    // {{2998, 184}, BRICK},
    // {{3014, 184}, BRICK},

    // {{3050, 184}, BRICK},
    // {{3066, 184}, BRICK},
    // {{3082, 184}, BRICK},
    // {{3098, 184}, BRICK},
    // {{3114, 184}, BRICK},

    // {{3150, 184}, BRICK},
    // {{3166, 184}, BRICK},
    // {{3182, 184}, BRICK},
    // {{3198, 184}, BRICK},
    // {{3214, 184}, BRICK},

    // {{3250, 184}, BRICK},
    // {{3266, 184}, BRICK},
    // {{3282, 184}, BRICK},
    // {{3298, 184}, BRICK},
    // {{3314, 184}, BRICK},

    // {{3350, 184}, BRICK},
    // {{3366, 184}, BRICK},
    // {{3382, 184}, BRICK},
    // {{3398, 184}, BRICK},
    // {{3414, 184}, BRICK},

    // {{3450, 184}, BRICK},
    // {{3466, 184}, BRICK},
    // {{3482, 184}, BRICK},
    // {{3498, 184}, BRICK},
    // {{3514, 184}, BRICK},

    // {{3550, 184}, BRICK},
    // {{3566, 184}, BRICK},
    // {{3582, 184}, BRICK},
    // {{3598, 184}, BRICK},
    // {{3614, 184}, BRICK},

    // {{3650, 184}, BRICK},
    // {{3666, 184}, BRICK},
    // {{3682, 184}, BRICK},
    // {{3698, 184}, BRICK},
    // {{3714, 184}, BRICK},

    // {{3750, 184}, BRICK},
    // {{3766, 184}, BRICK},
    // {{3782, 184}, BRICK},
    // {{3798, 184}, BRICK},
    // {{3814, 184}, BRICK},

    // {{3850, 184}, BRICK},
    // {{3866, 184}, BRICK},
    // {{3882, 184}, BRICK},
    // {{3898, 184}, BRICK},
    // {{3914, 184}, BRICK},

    // {{3950, 184}, BRICK},
    // {{3966, 184}, BRICK},
    // {{3982, 184}, BRICK},
    // {{3998, 184}, BRICK},
    // {{4014, 184}, BRICK},
};

#endif
