#include <assets/text_sprites_data.hh>
#include <cmath>
#include <core/utils.hh>
#include <iostream>
#include <sstream>
using namespace std;

namespace pro2 {

void paint_hline(pro2::Window& window, int xini, int xfin, int y, Color color) {
    for (int x = xini; x <= xfin; x++) {
        window.set_pixel({x, y}, color);
    }
}

void paint_vline(pro2::Window& window, int x, int yini, int yfin, Color color) {
    for (int y = yini; y <= yfin; y++) {
        window.set_pixel({x, y}, color);
    }
}

void paint_sprite_center(pro2::Window&              window,
                         pro2::Pt                   orig,
                         const vector<vector<int>>& sprite,
                         bool                       mirror,
                         float                      scae) {
    const Pt top_left = {orig.x - static_cast<int>(sprite[0].size() / 2),
                         orig.y - static_cast<int>(sprite.size() / 2)};
    paint_sprite(window, top_left, sprite, mirror);
}

void paint_sprite(pro2::Window&              window,
                  pro2::Pt                   orig,
                  const vector<vector<int>>& sprite,
                  bool                       mirror,
                  float                      scale) {
    for (int i = 0; i < sprite.size(); i++) {
        const vector<int>& line = sprite[i];
        for (int j = 0; j < line.size(); j++) {
            const int _j = mirror ? line.size() - j - 1 : j;
            if (line[_j] >= 0) {
                int x = static_cast<int>((orig.x + j) * scale);
                int y = static_cast<int>((orig.y + i) * scale);
                int size = static_cast<int>(scale);

                // Draw a filled square of size `scale x scale`
                for (int dy = 0; dy < size; dy++) {
                    for (int dx = 0; dx < size; dx++) {
                        window.set_pixel({x + dx, y + dy}, line[_j]);
                    }
                }
            }
        }
    }
}

void paint_background_sprite(pro2::Window&                        window,
                             const std::vector<std::vector<int>>& sprite,
                             double                               parallaxMultiplier,
                             const std::vector<std::vector<int>>& sky) {
    int upwardsOffset = 100;

    pro2::Pt it = window.topleft();
    pro2::Pt end = it + pro2::Pt{window.width(), window.height()};

    while (it.y != end.y) {
        int row_idx = (it.y + upwardsOffset) % sprite.size();
        int col_idx =
            (it.x - static_cast<int>(std::floor(window.topleft().x * parallaxMultiplier))) %
            sprite[0].size();

        int col;
        if (it.y < -upwardsOffset) {
            col = sky[row_idx % sky.size()][col_idx % sky[0].size()];
        } else {
            col = sprite[row_idx][col_idx];
        }

        window.set_pixel(it, col);

        it.x++;
        if (it.x >= end.x) {
            it.x = window.topleft().x;
            it.y++;
        }
    }
}

void paint_number(pro2::Window& window, pro2::Pt orig, int number) {
    ostringstream oss;
    oss << number;
    string text = oss.str();
    while (text.size() <= 0) {
        text.insert(text.begin(), '0');
    }

    while (text.size() > 0) {
        auto sprite = TextSprites::numberSprites[(text[0] - '0')];
        paint_sprite(window, orig, sprite, false);
        orig = {orig.x + static_cast<int>(sprite[0].size()), orig.y};
        text.erase(text.begin());
    }
}

void paint_end_circle(Window& window, int radius, Pt center) {
    pro2::Pt it = window.topleft();
    pro2::Pt end = it + pro2::Pt{window.width(), window.height()};

    while (it.y != end.y) {
        if (sqrt((center.x - it.x) * (center.x - it.x) + (center.y - it.y) * (center.y - it.y)) >=
            radius) {
            window.set_pixel(it, 0x000000);
        }
        it.x++;
        if (it.x >= end.x) {
            it.x = window.topleft().x;
            it.y++;
        }
    }
}

}  // namespace pro2