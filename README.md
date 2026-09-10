# Weighted Icon Randomizer

A Geode mod for Geometry Dash 2.2081 / Geode 5.10.1.

## What it does

Weighted Icon Randomizer has exactly one gameplay purpose: after a player death, it can select a new **cube** from a weighted list. The original Geometry Dash death handler is called first.

It does not randomize ship, ball, UFO, wave, robot, spider, swing, or other non-cube gamemodes. Jetpack is covered by Geometry Dash's ship state.

### Example

- Cube 1 = 50
- Cube 7 = 25
- Cube 21 = 15
- Cube 40 = 10

Weights are relative. They do not need to total 100. With the example above, the probabilities are exactly 50%, 25%, 15%, and 10%.

Zero/negative weights are never selected.

## More Icons

More Icons is an optional dependency. When it is installed, entries can select its custom cube icons by name. The mod uses More Icons' cube API rather than treating custom icons as vanilla Geometry Dash IDs.

If More Icons is absent, vanilla cube entries continue to work. More Icons entries simply cannot be selected until More Icons is installed.

## Settings

- Enable randomizer
- Avoid immediate repeat
- Cube entries editor
  - add/remove entries
  - enable/disable entries
  - switch Vanilla / More Icons source
  - set cube ID or More Icons name
  - set weight
- Test randomize
- Reset configuration

Configuration is saved through Geode saved data.

## Build

Install the current Geode SDK first:

```text
geode sdk install
geode sdk install-binaries
```

Make sure it is Geode **5.10.1** and that `GEODE_SDK` points to that SDK. Then from this directory:

```text
geode build
```

The Geode CLI automatically resolves the optional More Icons dependency headers when it is declared in `mod.json`.

## Verification notes

The source targets Geode v5.10.1 and GD 2.2081. The death hook uses the current `PlayLayer::destroyPlayer(PlayerObject*, GameObject*)` binding. The cube-only guard checks the current PlayerObject non-cube gamemode flags before doing anything. Vanilla cubes use `GameManager::setPlayerFrame`, while More Icons cubes use its `IconType::Cube` API.

An actual native compile/runtime test requires a machine with the Geode 5.10.1 SDK/toolchain and Geometry Dash installation. This environment cannot truthfully claim that final binary/runtime test was performed.
