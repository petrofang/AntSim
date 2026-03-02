// Fill out your copyright notice in the Description page of Project Settings.

#include "AInstancedAntRenderer.h"
#include "AntSimulationSubsystem.h"
#include "Components/InstancedStaticMeshComponent.h"

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

AInstancedAntRenderer::AInstancedAntRenderer()
{
	PrimaryActorTick.bCanEverTick = true;

	InstancedMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(
		TEXT("InstancedMeshComponent"));
	RootComponent = InstancedMeshComponent;
}

// ---------------------------------------------------------------------------
// BeginPlay
// ---------------------------------------------------------------------------

void AInstancedAntRenderer::BeginPlay()
{
	Super::BeginPlay();

	// Assign the user-specified mesh (may be null before a mesh is set in editor).
	if (AntMesh)
	{
		InstancedMeshComponent->SetStaticMesh(AntMesh);
	}

	// Seed one instance per ant so the instance buffer is the right size from
	// the start.  Transforms will be updated on the first Tick.
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	UAntSimulationSubsystem* Sim = World->GetSubsystem<UAntSimulationSubsystem>();
	if (!Sim)
	{
		return;
	}

	const TArray<FVector> Positions = Sim->GetAntPositions();
	TArray<FTransform> InitialTransforms;
	InitialTransforms.Reserve(Positions.Num());
	for (const FVector& Pos : Positions)
	{
		InitialTransforms.Add(FTransform(Pos));
	}
	InstancedMeshComponent->AddInstances(InitialTransforms, /*bShouldReturnIndices=*/false);
}

// ---------------------------------------------------------------------------
// Tick — update every instance to match the current simulation positions
// ---------------------------------------------------------------------------

void AInstancedAntRenderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	UAntSimulationSubsystem* Sim = World->GetSubsystem<UAntSimulationSubsystem>();
	if (!Sim)
	{
		return;
	}

	const TArray<FVector> Positions = Sim->GetAntPositions();
	const int32 SimCount = Positions.Num();
	const int32 InstanceCount = InstancedMeshComponent->GetInstanceCount();

	// Add or remove instances if the count changed (e.g. ants spawned/died).
	if (SimCount > InstanceCount)
	{
		TArray<FTransform> NewTransforms;
		NewTransforms.Reserve(SimCount - InstanceCount);
		for (int32 i = InstanceCount; i < SimCount; ++i)
		{
			NewTransforms.Add(FTransform(Positions[i]));
		}
		InstancedMeshComponent->AddInstances(NewTransforms, /*bShouldReturnIndices=*/false);
	}
	else if (SimCount < InstanceCount)
	{
		for (int32 i = InstanceCount - 1; i >= SimCount; --i)
		{
			InstancedMeshComponent->RemoveInstance(i);
		}
	}

	// Update every instance transform to match the simulation position.
	for (int32 i = 0; i < SimCount; ++i)
	{
		InstancedMeshComponent->UpdateInstanceTransform(
			i,
			FTransform(Positions[i]),
			/*bWorldSpace=*/true,
			/*bMarkRenderStateDirty=*/(i == SimCount - 1), // flush once at the end
			/*bTeleport=*/true);
	}
}
