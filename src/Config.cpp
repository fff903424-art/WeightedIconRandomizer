#include <Config.hpp>

#include <Geode/Geode.hpp>
#include <cmath>
#include <algorithm>

using namespace geode::prelude;

namespace wir {

static constexpr char SAVE_KEY[] = "cube-entries";

std::vector<CubeEntry> defaultEntries() {
    return {
        {IconSource::Vanilla, 1, "", 50.0, true},
        {IconSource::Vanilla, 7, "", 25.0, true},
        {IconSource::Vanilla, 21, "", 15.0, true},
        {IconSource::Vanilla, 40, "", 10.0, true},
    };
}

static matjson::Value toJSON(std::vector<CubeEntry> const& entries) {
    matjson::Value array = matjson::Array();
    for (auto const& entry : entries) {
        matjson::Value obj = matjson::Object();
        obj["source"] = static_cast<int>(entry.source);
        obj["vanilla-id"] = entry.vanillaID;
        obj["more-icons-name"] = entry.moreIconsName;
        obj["weight"] = entry.weight;
        obj["enabled"] = entry.enabled;
        array.push(obj);
    }
    return array;
}

static std::vector<CubeEntry> fromJSON(matjson::Value const& value) {
    std::vector<CubeEntry> result;
    if (!value.isArray()) return result;

    for (auto const& item : value) {
        if (!item.isObject()) continue;
        CubeEntry entry;
        entry.source = item["source"].asInt().unwrapOr(0) == 1 ? IconSource::MoreIcons : IconSource::Vanilla;
        entry.vanillaID = std::max(1, item["vanilla-id"].asInt().unwrapOr(1));
        entry.moreIconsName = item["more-icons-name"].asString().unwrapOr("");
        entry.weight = item["weight"].asDouble().unwrapOr(0.0);
        if (!std::isfinite(entry.weight) || entry.weight < 0.0) entry.weight = 0.0;
        entry.enabled = item["enabled"].asBool().unwrapOr(true);
        result.push_back(std::move(entry));
    }
    return result;
}

std::vector<CubeEntry> loadEntries() {
    auto saved = Mod::get()->getSavedValue<matjson::Value>(SAVE_KEY, toJSON(defaultEntries()));
    auto entries = fromJSON(saved);
    return entries.empty() ? defaultEntries() : entries;
}

void saveEntries(std::vector<CubeEntry> const& entries) {
    Mod::get()->setSavedValue<matjson::Value>(SAVE_KEY, toJSON(entries));
}

void resetEntries() {
    saveEntries(defaultEntries());
}

std::string describe(CubeEntry const& entry) {
    if (entry.source == IconSource::Vanilla)
        return fmt::format("Cube {}", entry.vanillaID);
    return entry.moreIconsName.empty() ? "More Icons: <unnamed>" : fmt::format("More Icons: {}", entry.moreIconsName);
}

} // namespace wir
