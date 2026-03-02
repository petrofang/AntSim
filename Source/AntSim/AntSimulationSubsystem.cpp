// Fill out your copyright notice in the Description page of Project Settings.

#include "AntSimulationSubsystem.h"

// ---------------------------------------------------------------------------
// USubsystem
// ---------------------------------------------------------------------------

void UAntSimulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Spawn 20 ants at random positions on the ground plane with random velocities.
	// A fixed seed keeps initialization deterministic (same seed → same results).
	FRandomStream Rand(InitialRandomSeed);
	Ants.Reserve(InitialAntCount);
	for (int32 i = 0; i < InitialAntCount; ++i)
	{
		FAntData Ant;
		// Random position on the XY ground plane (Z = 0).
		Ant.Position = FVector(
			Rand.FRandRange(-InitialSpawnExtent, InitialSpawnExtent),
			Rand.FRandRange(-InitialSpawnExtent, InitialSpawnExtent),
			0.f);
		// Random unit-direction in the XY plane; speed is stored separately.
		const float AngleRad = Rand.FRandRange(0.f, 2.f * PI);
		Ant.Velocity = FVector(FMath::Cos(AngleRad), FMath::Sin(AngleRad), 0.f);
		Ants.Add(Ant);
	}
}

// ---------------------------------------------------------------------------
// FTickableGameObject
// ---------------------------------------------------------------------------

void UAntSimulationSubsystem::Tick(float DeltaTime)
{
	AccumulatedTime += DeltaTime;

	// Consume fixed-size steps so the simulation stays deterministic regardless
	// of the render frame rate.
	while (AccumulatedTime >= FixedTimestep)
	{
		SimStep();
		AccumulatedTime -= FixedTimestep;
	}
}

TStatId UAntSimulationSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UAntSimulationSubsystem, STATGROUP_Tickables);
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

TArray<FVector> UAntSimulationSubsystem::GetAntPositions() const
{
	TArray<FVector> Positions;
	Positions.Reserve(Ants.Num());
	for (const FAntData& Ant : Ants)
	{
		Positions.Add(Ant.Position);
	}
	return Positions;
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

void UAntSimulationSubsystem::SimStep()
{
	for (FAntData& Ant : Ants)
	{
		Ant.Position += Ant.Velocity * Ant.Speed * FixedTimestep;
	}
}
