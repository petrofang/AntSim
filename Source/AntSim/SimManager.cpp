// Fill out your copyright notice in the Description page of Project Settings.


#include "SimManager.h"

// Sets default values
ASimManager::ASimManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASimManager::BeginPlay()
{
	Super::BeginPlay();

	// Spawn 10 ants at random positions with random unit-direction velocities.
	// A fixed seed keeps initialization deterministic (same seed → same results).
	FRandomStream Rand(42);
	Ants.Reserve(10);
	for (int32 i = 0; i < 10; ++i)
	{
		FAnt Ant;
		// Random position on the XY ground plane (Z = 0).
		Ant.Position = FVector(
			Rand.FRandRange(-500.f, 500.f),
			Rand.FRandRange(-500.f, 500.f),
			0.f);
		// Random unit direction in the XY plane; speed stays at default (100 cm/s).
		const float AngleRad = Rand.FRandRange(0.f, 2.f * PI);
		Ant.Velocity = FVector(FMath::Cos(AngleRad), FMath::Sin(AngleRad), 0.f);
		Ants.Add(Ant);
	}
}

// Called every frame
void ASimManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AccumulatedTime += DeltaTime;

	// Consume fixed-size steps so the simulation stays deterministic regardless
	// of the render frame rate.
	while (AccumulatedTime >= FixedTimestep)
	{
		SimStep();
		AccumulatedTime -= FixedTimestep;
	}
}

void ASimManager::SimStep()
{
	for (FAnt& Ant : Ants)
	{
		Ant.Position += Ant.Velocity * Ant.Speed * FixedTimestep;
	}
}

