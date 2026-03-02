// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AInstancedAntRenderer.generated.h"

class UInstancedStaticMeshComponent;

/**
 * AInstancedAntRenderer
 *
 * An actor that renders every simulated ant as one instance of a static mesh.
 * Each frame it queries UAntSimulationSubsystem for ant positions and updates
 * the per-instance transforms so the visuals track the simulation.
 *
 * Usage:
 *   1. Place the actor in the level.
 *   2. Assign a mesh to AntMesh (e.g. a small capsule or cone).
 *   3. Press Play — the instances will move with the simulation.
 */
UCLASS()
class ANTSIM_API AInstancedAntRenderer : public AActor
{
	GENERATED_BODY()

public:
	AInstancedAntRenderer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** Mesh used to represent each ant instance. Assign in the Details panel. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AntRenderer")
	UStaticMesh* AntMesh = nullptr;

private:
	/** Instanced mesh component — one instance per simulated ant. */
	UPROPERTY(VisibleAnywhere, Category = "AntRenderer")
	UInstancedStaticMeshComponent* InstancedMeshComponent = nullptr;
};
