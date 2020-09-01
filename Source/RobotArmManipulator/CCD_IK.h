// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Pawn.h"
#include "RotationLimit.h"
#include "CCD_IK.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTARMMANIPULATOR_API UCCD_IK : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCCD_IK();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingActor")
		float x = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingActor")
		float y = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingActor")
		float z = 150.0f;

	//The target Transform. Solver IKPosition will be automatically set to the position of the target.
	//UStaticMeshComponent* Target;

	//The hierarchy of bones.
	TArray<USceneComponent*,TFixedAllocator<6>> bones;
	TArray<FVector, TFixedAllocator<6>> defaultBonesPosition;
	TArray<FRotator, TFixedAllocator<6>> defaultBonesRotation;

	//Collection of rotation limit on each bone
	TArray<RotationLimit*, TFixedAllocator<5>> rotationLimits;

	//lenght of each bone until next bone
	TArray<float> bonesLength;

protected:

	FVector IKPosition;

	//Max iterations per frame
	int maxIterations;

	//Minimum distance from last reached position. Will stop solving if difference from previous reached position is less than tolerance. If tolerance is zero, will iterate until maxIterations.
	float tolerance;

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual FVector GetSingularityOffset();

	virtual bool SingularityDetected();

	virtual void InitiateBones();

	//Solves the CCD algorithm
	virtual void Solve(FVector targetPosition);

	virtual void FixTransform();

	virtual FQuat FromToRotation(FVector from, FVector to);

	virtual FQuat AngleAxis(float angle, FVector axis);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//lenght of current chain to be solved by solver
	float chainLength;

	FVector* GetIKPosition();
	void SetIKPosition(FVector pos);
};
