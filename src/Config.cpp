#include <Config.hpp>

#include <Geode/Geode.hpp>
#include <matjson.hpp>

#include <algorithm>
#include <cmath>
#include <string>

using namespace geode::prelude;

namespace wir {

std::vector<CubeEntry> defaultEntries() {
    return {
        CubeEntry{
            .source = IconSource::Vanilla,
            .vanillaID = 1,
            .moreIconsName = "",
            .weight = 1.0,
            .enabled = true
        },
        CubeEntry{
            .source = IconSource::Vanilla,
            .vanillaID = 2,
            .moreIconsName = "",
            .weight = 1.0,
            .enabled = true
        },
        CubeEntry{
            .source = IconSource::Vanilla,
            .vanillaID = 3,
            .moreIconsName = "",
            .weight = 1.0,
            .enabled = true
        }
    };
}

std::vector<CubeEntry> loadEntries() {
    auto saved =
        Mod::get()->getSavedValue<matjson::Value>(
            "entries",
            matjson::Value::array()
        );

    if (!saved.isArray())
        return defaultEntries();

    std::vector<CubeEntry> entries;

    for (auto const& item : saved) {
        if (!item.isObject())
            continue;

        CubeEntry entry;

        auto source =
            item["source"].asInt().unwrapOr(0);

        if (source ==
            static_cast<int>(IconSource::MoreIcons)) {
            entry.source = IconSource::MoreIcons;
        }
        else {
            entry.source = IconSource::Vanilla;
        }

        auto id =
            item["vanilla-id"].asInt().unwrapOr(1);

        entry.vanillaID =
            std::max(
                1,
                static_cast<int>(id)
            );

        entry.moreIconsName =
            item["more-icons-name"]
                .asString()
                .unwrapOr("");

        entry.weight =
            item["weight"]
                .asDouble()
                .unwrapOr(1.0);

        if (!std::isfinite(entry.weight) ||
            entry.weight < 0.0) {
            entry.weight = 0.0;
        }

        entry.enabled =
            item["enabled"]
                .asBool()
                .unwrapOr(true);

        entries.push_back(entry);
    }

    if (entries.empty())
        return defaultEntries();

    return entries;
}

void saveEntries(
    std::vector<CubeEntry> const& entries
) {
    auto array = matjson::Value::array();

    for (auto const& entry : entries) {
        auto obj = matjson::Value::object();

        obj["source"] =
            static_cast<int>(entry.source);

        obj["vanilla-id"] =
            entry.vanillaID;

        obj["more-icons-name"] =
            entry.moreIconsName;

        obj["weight"] =
            entry.weight;

        obj["enabled"] =
            entry.enabled;

        array.push(obj);
    }

    Mod::get()->setSavedValue(
        "entries",
        array
    );
}

void resetEntries() {
    saveEntries(defaultEntries());
}

std::string describe(
    CubeEntry const& entry
) {
    if (entry.source == IconSource::Vanilla) {
        return fmt::format(
            "Vanilla Cube {} (weight {:.3f}){}",
            entry.vanillaID,
            entry.weight,
            entry.enabled
                ? ""
                : " [disabled]"
        );
    }

    return fmt::format(
        "More Icons '{}' (weight {:.3f}){}",
        entry.moreIconsName,
        entry.weight,
        entry.enabled
            ? ""
            : " [disabled]"
    );
}

} // namespace wir
