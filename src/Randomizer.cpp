#define MORE_ICONS_EVENTS
#include <hiimjustin000.more_icons/include/MoreIcons.hpp>

#include <Randomizer.hpp>

#include <Geode/Geode.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include <algorithm>
#include <cmath>
#include <random>

using namespace geode::prelude;

namespace wir {

static std::mt19937_64& rng() {
    static std::mt19937_64 engine(std::random_device{}());
    return engine;
}

static bool sameEntry(CubeEntry const& a, CubeEntry const& b) {
    if (a.source != b.source) return false;
    if (a.source == IconSource::Vanilla) return a.vanillaID == b.vanillaID;
    return a.moreIconsName == b.moreIconsName;
}

std::optional<CubeEntry> chooseEntry(
    std::vector<CubeEntry> const& entries,
    std::optional<CubeEntry> const& previous,
    bool avoidRepeat
) {
    auto valid = [&](CubeEntry const& e) {
        if (!e.enabled || !std::isfinite(e.weight) || e.weight <= 0.0)
            return false;

        if (e.source == IconSource::Vanilla)
            return e.vanillaID > 0;

        return !e.moreIconsName.empty();
    };

    std::vector<CubeEntry const*> candidates;

    for (auto const& e : entries) {
        if (valid(e))
            candidates.push_back(&e);
    }

    if (candidates.empty())
        return std::nullopt;

    if (avoidRepeat && previous && candidates.size() > 1) {
        std::vector<CubeEntry const*> filtered;

        for (auto* e : candidates) {
            if (!sameEntry(*e, *previous))
                filtered.push_back(e);
        }

        if (!filtered.empty())
            candidates = std::move(filtered);
    }

    double total = 0.0;

    for (auto* e : candidates)
        total += e->weight;

    if (!(total > 0.0) || !std::isfinite(total))
        return std::nullopt;

    std::uniform_real_distribution<double> dist(0.0, total);
    double roll = dist(rng());

    for (auto* e : candidates) {
        roll -= e->weight;

        if (roll < 0.0)
            return *e;
    }

    return *candidates.back();
}

bool isCubeMode(PlayerObject* player) {
    if (!player)
        return false;

    // These are the explicit non-cube gamemode flags exposed by the current
    // PlayerObject binding. Jetpack uses the ship state, so it is covered by
    // m_isShip as well.
    return !player->m_isShip &&
           !player->m_isBird &&
           !player->m_isBall &&
           !player->m_isDart &&
           !player->m_isRobot &&
           !player->m_isSpider &&
           !player->m_isSwing;
}

static bool moreIconsAvailable() {
    return Loader::get()->isModLoaded("hiimjustin000.more_icons");
}

bool applyCube(CubeEntry const& entry) {
    if (entry.source == IconSource::Vanilla) {
        if (entry.vanillaID <= 0)
            return false;

        GameManager::get()->setPlayerFrame(entry.vanillaID);
        return true;
    }

    if (!moreIconsAvailable()) {
        log::warn(
            "Cannot select '{}': More Icons is not installed.",
            entry.moreIconsName
        );
        return false;
    }

    auto* icon = more_icons::getIcon(
        entry.moreIconsName,
        IconType::Cube
    );

    if (!icon) {
        log::warn(
            "More Icons cube '{}' was not found.",
            entry.moreIconsName
        );
        return false;
    }

    more_icons::setIcon(
        icon,
        IconType::Cube
    );

    return true;
}

void randomizeCube() {
    if (!Mod::get()->getSettingValue<bool>("enabled"))
        return;

    static std::optional<CubeEntry> previous;

    bool avoidRepeat =
        Mod::get()->getSettingValue<bool>("avoid-repeat");

    auto entries = loadEntries();

    auto selected =
        chooseEntry(entries, previous, avoidRepeat);

    if (!selected)
        return;

    if (applyCube(*selected)) {
        previous = *selected;
    }
}

} // namespace wirvoid randomizeCube() {
    if (!Mod::get()->getSettingValue<bool>("enabled")) return;

    static std::optional<CubeEntry> previous;
    bool avoidRepeat = Mod::get()->getSettingValue<bool>("avoid-repeat");

    auto entries = loadEntries();
    auto selected = chooseEntry(entries, previous, avoidRepeat);
    if (!selected) return;

    if (applyCube(*selected)) {
        previous = *selected;
    }
}

} // namespace wir
