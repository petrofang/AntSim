// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ant.h"
#include "SimManager.generated.h"

UCLASS()
class ANTSIM_API ASimManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/** Simulated ants — position, velocity direction, and speed. */
	TArray<FAnt> Ants;

	/** Leftover real time that hasn't been consumed by fixed steps yet. */
	float AccumulatedTime = 0.f;

	/** Fixed simulation timestep (seconds). ~0.0333 = 30 Hz. */
	static constexpr float FixedTimestep = 1.0f / 30.0f;

	/** Advance the simulation by exactly one fixed step. */
	void SimStep();

};
