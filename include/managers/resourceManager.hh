/**
 * @file resourceManager.hh
 * @brief
 * @version 0.1
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef RESOURCEMANAGER_HH
#define RESOURCEMANAGER_HH

#include <random>
#include <vector>
#include "world/collectable.hh"

typedef std::vector<std::pair<pro2::Pt, COLLECTABLE_TYPE>> CollectablesMap;

// Forward declaration of Collectable class
class BoxCollider;

namespace pro2 {
class Window;
}

template <typename T>
class Finder;

/**
 * @brief The class Resource Manager is a container for collectables.
 * It helps organize them , spawn and destroy them.
 */
class ResourceManager {
 private:
    CollectablesMap          collectablesMap_;
    std::vector<Collectable> collectables_;
    int                      collected_;

    Finder<Collectable> *f_Collectable_;

 public:
    /**
     * @brief Construct a ResourceManager with the defaultCollectablesMap.
     */
    ResourceManager();

    /**
     * @brief Construct a new ReousrceManager with a custom collectables map.
     *
     * @param cm
     */
    ResourceManager(CollectablesMap cm);

    /**
     * @brief Retruns the amount of pionts collected . SACKS -> 5, COINS -> 1
     *
     * @return int
     */
    int getCollected() const;

    /**
     * @brief Checks if a collider has collided with a coin, if it has it collects that coin and
     * erase it from the container.
     *
     * @param marioCollider
     */
    int update(BoxCollider *marioCollider);

    /**
     * @brief Respawns all collectables, does not reset the collected_ counter.
     *
     */
    void respawnCollectables();

    /**
     * @brief Paints all collectables on the window.
     *
     * @param window
     * @param debug
     */
    void paint(pro2::Window& window, bool debug);

 private:
    const CollectablesMap defaultCollectablesMap = {{{75, 225}, SMALL_COIN},
                                                    {{125, 225}, SMALL_COIN},
                                                    {{175, 225}, SMALL_COIN},
                                                    {{225, 225}, SMALL_COIN},
                                                    {{275, 225}, SMALL_COIN},
                                                    {{325, 225}, SMALL_COIN},
                                                    {{375, 225}, SMALL_COIN},
                                                    {{425, 225}, SMALL_COIN},

                                                    {{75, 165}, COIN},
                                                    {{125, 165}, SMALL_COIN},
                                                    {{375, 165}, SMALL_COIN},
                                                    {{425, 165}, COIN},

                                                    {{75, 100}, COIN},
                                                    {{425, 100}, SMALL_COIN},
                                                    // Added more sacks
                                                    {{200, 200}, COIN},
                                                    {{300, 180}, COIN},
                                                    {{350, 120}, COIN},
                                                    {{150, 140}, COIN},
                                                    {{250, 160}, COIN}};

    // 100x100 matrix of collectables, each position (x, y) from (0,0) to (99,99)
    // REDUCED: Now only every 40 pixels, and only SACKS
    const CollectablesMap coll1Map = []() {
        CollectablesMap map;
        for (int x = 0; x < 1000; x += 40) {
            for (int y = 0; y < 1000; y += 40) {
                map.push_back({{x, y}, COIN});
            }
        }
        return map;
    }();

    const CollectablesMap entrega2 = {{{32, -32}, COIN},
                                      {{396, 220}, SMALL_COIN},
                                      {{100, 275}, SMALL_COIN},
                                      {{475, 270}, SMALL_COIN},
                                      {{550, 220}, SMALL_COIN},
                                      {{650, 220}, SMALL_COIN},
                                      {{1390, 200}, COIN},
                                      {{232, 75}, SMALL_COIN},
                                      // Added more sacks
                                      {{300, 300}, COIN},
                                      {{500, 100}, COIN},
                                      {{700, 150}, COIN}};

    // Random grids of coins and sacks from left=0 to right=7400, top=0 to bottom=320
    // REDUCED: Fewer grids, fewer coins per grid, more sacks, fewer random sacks
    const CollectablesMap judit = []() {
        CollectablesMap map;
        unsigned int    seed = 42;
        srand(seed);
        int numGrids = 4 + rand() % 4;        // 4-7 grids (was 10-19)
        for (int g = 0; g < numGrids; ++g) {
            int gridCols = 2 + rand() % 4;    // 2-5 columns (was 5-14)
            int gridRows = 1 + rand() % 3;    // 1-3 rows (was 2-7)
            int tileSize = 48 + rand() % 32;  // 48-79 px (was 32-63)
            int left = rand() % (7400 - gridCols * tileSize);
            int top = rand() % (320 - gridRows * tileSize);
            for (int i = 0; i < gridCols; ++i) {
                for (int j = 0; j < gridRows; ++j) {
                    int x = left + i * tileSize;
                    int y = top + j * tileSize;
                    // Randomly decide COIN or SACK (now 40% coin, 60% sack)
                    COLLECTABLE_TYPE type = (rand() % 10 < 4) ? SMALL_COIN : COIN;
                    map.push_back({{x, y}, type});
                }
            }
        }
        // Add a few random sacks
        int numSacks = 8 + rand() % 5;  // 8-12 sacks (was 30-49)
        for (int i = 0; i < numSacks; ++i) {
            int x = rand() % 7400;
            int y = rand() % 320;
            map.push_back({{x, y}, COIN});
        }

        map.push_back({{7200, 150}, END_STAR});
        map.push_back({{100, 150}, END_STAR});
        return map;
    }();
};

#endif