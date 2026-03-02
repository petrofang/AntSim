// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AntSimulationSubsystem.generated.h"

/** Simple ant data: position and velocity in world space. */
USTRUCT(BlueprintType)
struct FAntData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FVector Position = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	FVector Velocity = FVector(100.f, 0.f, 0.f);
};

/**
 * UAntSimulationSubsystem
 *
 * A UTickableWorldSubsystem that runs a fixed-timestep ant simulation.
 * Each ant has a position and velocity; every fixed tick the ants move
 * forward by Velocity * FixedTimestep.
 *
 * Usage:
 *   UAntSimulationSubsystem* Sim = World->GetSubsystem<UAntSimulationSubsystem>();
 *   TArray<FVector> Positions = Sim->GetAntPositions();
 */
UCLASS()
class ANTSIM_API UAntSimulationSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	// USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// FTickableGameObject interface
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	/**
	 * Returns a snapshot of every ant's current world position.
	 */
	UFUNCTION(BlueprintCallable, Category = "AntSim")
	TArray<FVector> GetAntPositions() const;

private:
	/** Simulated ants — position + velocity. */
	TArray<FAntData> Ants;

	/** Leftover real time that hasn't been consumed by fixed steps yet. */
	float AccumulatedTime = 0.f;

	/** Fixed simulation timestep (seconds). 0.05 = 20 Hz. */
	static constexpr float FixedTimestep = 0.05f;

	/** Number of ants spawned at initialization. */
	static constexpr int32 InitialAntCount = 20;

	/** Half-extent (cm) of the square region in which ants are randomly placed. */
	static constexpr float InitialSpawnExtent = 500.f;

	/** Seed for the random stream used during initialization (ensures determinism). */
	static constexpr int32 InitialRandomSeed = 42;

	/** Advance the simulation by exactly one fixed step. */
	void SimStep();
};
