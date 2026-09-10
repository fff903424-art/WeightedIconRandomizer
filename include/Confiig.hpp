#pragma once

#include <string>
#include <vector>

namespace wir {

enum class IconSource : int {
    Vanilla = 0,
    MoreIcons = 1,
};

struct CubeEntry {
    IconSource source = IconSource::Vanilla;
    int vanillaID = 1;
    std::string moreIconsName;
    double weight = 1.0;
    bool enabled = true;
};

std::vector<CubeEntry> defaultEntries();
std::vector<CubeEntry> loadEntries();
void saveEntries(std::vector<CubeEntry> const& entries);
void resetEntries();

std::string describe(CubeEntry const& entry);

} // namespace wir
