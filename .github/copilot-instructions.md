# Copilot Instructions

## Project context
- This is an Unreal Engine 5 C++ project (`AntSim`).
- Source code is under `Source/AntSim/`.
- Ignore generated engine/project files under `Intermediate/`, `Binaries/`, `DerivedDataCache/`, and `Saved/`.

## Coding guidelines
- Prefer minimal, targeted changes.
- Follow existing style in touched files.
- Do not add new dependencies unless necessary.
- Keep simulation updates deterministic where possible (fixed timestep patterns are preferred).

## Unreal-specific expectations
- Keep Unreal macros and class structure intact (`UCLASS`, `UFUNCTION`, `UPROPERTY`).
- Avoid edits to generated project files (for example `Intermediate/ProjectFiles/*.vcxproj`).
- When adding new classes, place headers/sources in `Source/AntSim/` and keep includes local-first.

## Validation
- Ensure the project still builds after changes.
- Avoid unrelated refactors.
