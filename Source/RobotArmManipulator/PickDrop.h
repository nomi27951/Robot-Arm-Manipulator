// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "CCD_IK.h"
#include "iTween/iTween.h"
#include "iTween/iTweenEvent.h"
#include "PickDrop.generated.h"

UCLASS()
class ROBOTARMMANIPULATOR_API APickDrop : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickDrop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector RandomInsideUnitSphere();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		virtual void SelectedPawn(AActor* actor);

	AActor* currentlySelectedPawn;

	virtual void Move_XAxis(float AxisValue);
	virtual void Move_YAxis(float AxisValue);
	virtual void Move_ZAxis(float AxisValue);

	TArray<FVector> CreatePath(FVector startingPosition, FVector target);

	float speed;


	UCCD_IK* ik;
	FVector ikDefaultPosition;
	TArray<FVector> path;

	UFUNCTION(BlueprintCallable)
		virtual void Pick();

	UFUNCTION()
		void Drop();
	UFUNCTION()
		void Reset();
	UFUNCTION()
		void ResetComplete();

	UPROPERTY(EditAnywhere)
		AActor* Arm;

	UPROPERTY(EditAnywhere)
		AActor* actorMovingAlongSpline;

	UPROPERTY(EditAnywhere)
		AActor* pickUpPawn;

	UPROPERTY(EditAnywhere)
		AActor* dropOffPawn;

	UPROPERTY(EditAnywhere)
		AActor* target;

	bool inProgress;
};
