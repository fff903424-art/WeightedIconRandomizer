#pragma once

#include "Config.hpp"

#include <optional>
#include <vector>

class PlayerObject;

namespace wir {

std::optional<CubeEntry> chooseEntry(
    std::vector<CubeEntry> const& entries,
    std::optional<CubeEntry> const& previous,
    bool avoidRepeat
);

bool isCubeMode(PlayerObject* player);

bool applyCube(
    CubeEntry const& entry,
    PlayerObject* player = nullptr
);

void randomizeCube(
    PlayerObject* player = nullptr
);

} // namespace wir
