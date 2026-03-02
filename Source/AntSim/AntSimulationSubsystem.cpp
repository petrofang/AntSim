// Fill out your copyright notice in the Description page of Project Settings.

#include "AntSimulationSubsystem.h"

// ---------------------------------------------------------------------------
// USubsystem
// ---------------------------------------------------------------------------

void UAntSimulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Seed the initial ant population with varied velocities so they spread out.
	Ants.Reserve(InitialAntCount);
	for (int32 i = 0; i < InitialAntCount; ++i)
	{
		FAntData Ant;
		// Spread starting positions slightly so ants don't stack on top of each other.
		Ant.Position = FVector(i * InitialAntSpacing, 0.f, 0.f);
		// Give each ant a unique heading (evenly spaced around a circle).
		const float AngleDeg = (360.f / InitialAntCount) * i;
		const float AngleRad = FMath::DegreesToRadians(AngleDeg);
		Ant.Velocity = FVector(FMath::Cos(AngleRad), FMath::Sin(AngleRad), 0.f) * 100.f;
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
		Ant.Position += Ant.Velocity * FixedTimestep;
	}
}
