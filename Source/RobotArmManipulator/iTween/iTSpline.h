//Copyright Jared Therriault and Bob Berkbile 2014

#pragma once

//#include "iTweenPCH.h"
#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "iTSpline.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTARMMANIPULATOR_API AiTSpline : public AActor
{
	GENERATED_BODY()

	AiTSpline(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline")
	USplineComponent* spline; 
};
