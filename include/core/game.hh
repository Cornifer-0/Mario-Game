/**
 * @file game.hh
 * @brief
 * @version 0.1
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef GAME_HH
#define GAME_HH

#include <vector>

namespace pro2 {
class Window;
}
class Mario;
class Manager;
class Kamek;

template <typename T>
class Finder;

class Game {
    Mario   *mario_, *luigi_;
    Manager *manager_;

    bool finished_;
    bool end_sequence_ = false;
    int  end_sequence_time_ = 400;

    bool twoPlayers = false;

    bool debug_;
    bool paused_;

    void process_keys(pro2::Window& window);
    void update_objects(pro2::Window& window);
    void update_camera(pro2::Window& window);

 public:
    Game(int width, int height);
    ~Game();

    void update(pro2::Window& window);
    void paint(pro2::Window& window);

    bool is_finished() const {
        return finished_;
    }

 private:
    static constexpr int sky_blue = 0x5c94fc;
};

#endif