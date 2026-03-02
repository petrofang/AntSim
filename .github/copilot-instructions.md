# Copilot Instructions for Antenboro UE5

## Game Concept

**Antenboro** = SimAnt + Dwarf Fortress + RimWorld + Life in the Undergrowth + Honey I Shrunk the Kids

- **SimAnt DNA**: Deep ant colony AI, pheromone-based foraging, worker/soldier/queen roles, deterministic simulation
- **Dwarf Fortress DNA**: Emergent storytelling, complex interconnected systems, unpredictable outcomes
- **RimWorld DNA**: Resource management, base building, survival pressure, narrative moments
- **Undergrowth DNA**: David Attenborough-style narration, realistic ecosystem, reverence for nature
- **Honey I Shrunk the Kids DNA**: Perspective — player controls a hero ant in a human-scale world (grass is trees, water is lakes)

**Tech Stack**: Visual Studio 2022 + Unreal Engine 5.7

---

## Architecture Overview

### Simulation Layer (Pure C++, no UObject)
`Source/AntSim/Sim/` contains **deterministic, decoupled game logic**:

- **`SimWorld.h/cpp`** — Grid-based world (food, terrain, obstacles)
- **`Ant.h/cpp`** — Individual ant with state machine (WANDERING → FOLLOWING → CARRYING → FIGHTING → GUARDING)
  - No health/damage model; ants die only via queen death or explicit removal
  - No hunger or aging; behavior driven by pheromones and colony needs
- **`Colony.h/cpp`** — Colony manager (queen, ants, pheromone grid, food)
  - Queen is the only persistent entity; her survival = colony survival
  - Food reserves drive reproduction and building
- **`Pheromone.h/cpp`** — Per-colony 2D pheromone grid with decay
- **`SimManager.h/cpp`** — Orchestrator, fixed-timestep loop (20 ticks/sec)

### Rendering Layer (Unreal Actors)
`Source/AntSim/` contains **visualization and gameplay**:

- **`ASimManager`** — Already exists; hooks simulation to UE5 tick
- **`PlayerController`** — FPS hero ant controls (FUTURE)
- **`AntVisual`** — Maps sim ants to 3D Skeletal Meshes (FUTURE)
- **`WorldVisual`** — Terrain, food, visual effects (FUTURE)

---

## Design Principles

### 1. Simulation First
- Sim logic is **platform-agnostic** (could run headless, in tests, etc.)
- **No Unreal dependencies** in `Sim/` classes
- Deterministic: same seed = same results (replay support)

### 2. Fixed Timestep
- Simulation ticks at 20 Hz (0.05 sec per frame), independent of render FPS
- `SimManager` accumulates real delta time, calls `SimStep()` when threshold met
- Ensures multiplayer-ready, replay-able, consistent behavior

### 3. Superorganism (No Individual Survival)
- **Ants have no hunger, aging, or lifespan** — they are expendable units of the colony
- Colony health = queen alive + food reserves + nest integrity
- Ant behavior driven by **pheromones and local rules**, not individual needs
- Focus: collective intelligence, emergent colony strategies, inter-colony conflict
- Example: A worker doesn't "need" to find food; it deposits pheromones so *others* can forage

### 4. Scale & Perspective (Undergrowth)
- Player is a **hero ant** in first-person; grass blades are tree-trunks, water is lakes
- World feels **vast and overwhelming** — nature is huge, you are tiny
- Camera stays low, emphasizing towering vegetation and alien landscape
- Photorealistic materials: soil, sand, leaves, water droplets at macro scale
- Dynamic lighting: dappled sunlight through leaf canopy, glow from bioluminescent organisms
- Audio: layered nature soundscape (wind through grass, distant insect sounds, water)
- Optional narration: David Attenborough-style observations of colony behavior

---

## Coding Guidelines

### Sim Classes (Plain C++)
```cpp
// OK: Plain C++
class Ant {
    FAntData Data;
    void Tick(float DeltaTime, SimWorld* World, Pheromone* Phero);
};

// NOT OK: Unreal macros in Sim/
class Ant : public UObject { /* ... */ };  // ❌
```

- Use `FVector`, `TArray<>`, `FMath::*` (Unreal types are fine, but no UObject)
- Keep random seeded for determinism
- No logging in hot paths (use conditional `UE_LOG` if needed)

### Actor Classes (UE5)
```cpp
// OK: Use Unreal macros
UCLASS()
class AMyVisual : public AActor {
    UPROPERTY(EditAnywhere)
    float Scale;
};

// Render logic, input handling, replication go here
```

### Includes
- From `Sim/Ant.cpp`: `#include "Ant.h"` (local)
- From `SimManager.cpp`: `#include "Sim/Ant.h"` (full path)

---

## UE5 Workflow (Visual Studio + Editor)

### Opening the Project
1. **Visual Studio**: Open `AntSim.sln`
2. **Unreal Editor**: Open `AntSim.uproject` (or use VS → `Tools > Open in Unreal Editor`)

### Making Code Changes
1. **Edit** `.h` / `.cpp` in Visual Studio
2. **Save** the files
3. **Visual Studio**: Build (Ctrl+Shift+B) or `Build > Build Solution`
4. **Unreal Editor**: If open, it auto-recompiles; you'll see "Compiling C++" toast
5. **Test**: Press Play in Editor (PIE) to run the game

### If Build Fails
1. Check `Output` window → `Build` pane for errors
2. Common: missing `#include`, typo in class name
3. **Clean**: `Build > Clean Solution`, then rebuild
4. **Last resort**: Close UE5 Editor, delete `Binaries/` and `Intermediate/`, rebuild in VS

### Creating New Classes (via Editor or Manual)
**Option A (Editor UI)**:
1. Content Browser → `New C++ Class` → Choose parent (e.g., `AActor`)
2. Creates `.h` and `.cpp` in `Source/AntSim/`
3. VS auto-detects, compiles

**Option B (Manual)**:
1. Create `.h` and `.cpp` in VS
2. Add to `Source/AntSim/AntSim.Build.cs` if adding new module
3. Build in VS

---

## Performance Targets
- **Ants**: ~300 total (150 per colony)
- **Grid**: ~100×75 cells (O(1) food/pheromone lookups)
- **Memory**: ~200 bytes per ant struct
- **Render**: Use InstancedMesh for ant bodies (not individual actors)

---

## Validation Checklist
- [ ] `Source/AntSim/Sim/` compiles with no Unreal dependencies
- [ ] Fixed-timestep loop runs stably
- [ ] Two colonies spawn, ants wander and forage
- [ ] Pheromone trails form and decay
- [ ] No crashes, warnings in Output window
- [ ] Git commits are atomic (one feature per commit)

---

## Git Workflow
1. Make changes in VS
2. **Visual Studio** → `Git Changes` panel
3. Stage files, write commit message, **Commit**
4. **Push** to `origin/main`
5. Example: `git commit -m "Add Pheromone class and decay logic"`

---

## Quick Reference: File Locations
```
AntSim/
├── .github/
│   └── copilot-instructions.md       ← You are here
├── Source/AntSim/
│   ├── SimManager.h/cpp              ← Fixed-timestep orchestrator
│   ├── Sim/                          ← Pure simulation logic
│   │   ├── SimWorld.h/cpp
│   │   ├── Ant.h/cpp
│   │   ├── Colony.h/cpp
│   │   └── Pheromone.h/cpp
│   └── Visuals/                      ← UE5 rendering (FUTURE)
│       ├── AntVisual.h/cpp
│       └── WorldVisual.h/cpp
├── Content/                          ← UE5 blueprints, meshes, materials, levels
├── AntSim.sln                        ← Open this in Visual Studio
└── AntSim.uproject                   ← Open this to launch Unreal Editor
```

---

## Next Steps (Immediate)
1. **Implement `SimWorld`** — Grid-based world with food patches
2. **Implement `Pheromone`** — 2D grid with deposit/decay
3. **Implement `Ant`** — State machine with simple wandering
4. **Implement `Colony`** — Spawns ants, manages food, calls Tick
5. **Wire up `SimManager`** — Calls Colony::Tick in fixed-timestep loop
6. **Test in PIE** — Verify ants spawn and wander (no visuals yet)
