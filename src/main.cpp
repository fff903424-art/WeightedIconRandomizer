#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include <Randomizer.hpp>
#include <UI.hpp>

using namespace geode::prelude;

class $modify(WIRPlayLayer, PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        PlayLayer::destroyPlayer(player, object);

        if (!player)
            return;

        if (wir::isCubeMode(player)) {
            wir::randomizeCube();
        }
    }
};

$on_mod(Loaded) {
    log::info("Weighted Icon Randomizer loaded.");

    // The editor is intentionally exposed through the mod's
    // settings/menu UI rather than putting configuration logic here.
}
