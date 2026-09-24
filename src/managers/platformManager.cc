#include "managers/platformManager.hh"
#include "core/finder.hh"
#include "graphics/window.hh"
#include "world/platform.hh"

PlatformManager::PlatformManager() {
    platforms_ = {
        // new Platform(50, 450, 250, 300), new Platform(460, 890, 250, 300),
        // new Platform(50, 193, 190, 206), new Platform(300, 447, 190, 206),
        // new Platform(50, 97, 130, 146),  new Platform(403, 450, 130, 146),
        // new Platform(-14, 50, 234, 250), new Platform(1500, 1660, 234, 250),

        //   new Platform(0, 64, 0, 300),
        //   new Platform(64, 384, 250, 300),
        //   new Platform(384, 448, 220, 300),
        //   new Platform(448, 512, 300, 350),
        //   new Platform(512, 1000, 300, 350),
        //   new Platform(1000, 1100, 200, 350)

        new Platform(-150, 0, 0, 320),      new Platform(0, 256, 300, 320),
        new Platform(256, 512, 300, 320),   new Platform(512, 786, 250, 320),
        new Platform(786, 1536, 300, 320),  new Platform(1536, 1712, 250, 320),
        new Platform(1792, 1952, 250, 320), new Platform(2048, 2080, 250, 320),
        new Platform(2080, 3072, 300, 320), new Platform(3072, 3120, 250, 320),
        new Platform(3200, 3232, 220, 320), new Platform(3312, 3424, 190, 320),
        new Platform(3552, 3584, 250, 320), new Platform(3584, 4408, 300, 320),
        new Platform(4408, 4920, 250, 320), new Platform(4920, 5912, 300, 320),
        new Platform(5912, 5976, 250, 320), new Platform(5976, 6968, 300, 320),
        new Platform(6968, 7480, 250, 320),

    };

    f_Platforms_ = new Finder<Platform>;

    for (auto platform : platforms_) {
        f_Platforms_->add(platform);
    }
}

void PlatformManager::update(pro2::Window& window) {
    s_platforms_ = f_Platforms_->query(window.camera_rect());
}

void PlatformManager::paint(pro2::Window& window, bool debug) {
    auto it = s_platforms_.begin();
    while (it != s_platforms_.end()) {
        (*it)->paint(window, debug);
        it++;
    }
}

const std::vector<Platform *>& PlatformManager::getAllPlatforms() const {
    return platforms_;
}

const std::set<Platform *>& PlatformManager::getFinderPlatforms() const {
    return s_platforms_;
}
