// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ant.generated.h"

/** Represents a single ant with position, velocity, and movement speed. */
USTRUCT(BlueprintType)
struct FAnt
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 100.f;
};
