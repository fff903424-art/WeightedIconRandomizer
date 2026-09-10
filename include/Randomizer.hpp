#pragma once

#include "Config.hpp"
#include <optional>
#include <vector>

namespace wir {

std::optional<CubeEntry> chooseEntry(
    std::vector<CubeEntry> const& entries,
    std::optional<CubeEntry> const& previous,
    bool avoidRepeat
);

bool isCubeMode(class PlayerObject* player);
bool applyCube(CubeEntry const& entry);
void randomizeCube();

} // namespace wir
