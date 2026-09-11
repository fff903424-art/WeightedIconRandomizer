#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include <Randomizer.hpp>
#include <UI.hpp>

using namespace geode::prelude;

class $modify(WIRPlayLayer, PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        if (player && wir::isCubeMode(player)) {
            log::info("Weighted Icon Randomizer: player died in cube mode.");
            wir::randomizeCube();
        }

        PlayLayer::destroyPlayer(player, object);
    }
};

$on_mod(Loaded) {
    log::info("Weighted Icon Randomizer loaded successfully.");
}
