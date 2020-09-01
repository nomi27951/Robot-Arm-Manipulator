// Fill out your copyright notice in the Description page of Project Settings.


#include "PickDrop.h"

// Sets default values
APickDrop::APickDrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	speed = 4;
}

// Called when the game starts or when spawned
void APickDrop::BeginPlay()
{
	Super::BeginPlay();
	ik = (UCCD_IK*)(Arm->GetComponentByClass(UCCD_IK::StaticClass()));
	ikDefaultPosition = actorMovingAlongSpline->GetActorLocation();
}

// Called every frame
void APickDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ik->SetIKPosition(actorMovingAlongSpline->GetActorLocation());
}

// Called to bind functionality to input
void APickDrop::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UE_LOG(LogTemp, Warning, TEXT("Called"));
	InputComponent->BindAxis("MoveX", this, &APickDrop::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &APickDrop::Move_YAxis);
	InputComponent->BindAxis("MoveZ", this, &APickDrop::Move_ZAxis);

	InputComponent->BindAction("PickDrop", IE_Pressed, this, &APickDrop::Pick);
}


void APickDrop::SelectedPawn(AActor* actor)
{
	currentlySelectedPawn = actor;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *(actor->GetFName().ToString()));
}

void APickDrop::Move_XAxis(float AxisValue)
{
	if (inProgress)
		return;

	AxisValue = -AxisValue;
	if (FMath::Abs(AxisValue) > 0.0f)
	{
		if (currentlySelectedPawn != nullptr)
		{
			FVector location = currentlySelectedPawn->GetActorLocation();
			location.X += AxisValue * speed;

			//UCCD_IK* ik = (UCCD_IK*)(Arm->GetComponentByClass(UCCD_IK::StaticClass()));
			float distance = FVector::Distance(Arm->GetActorLocation(), location);
			if (distance <= ik->chainLength)
			{
				currentlySelectedPawn->SetActorLocation(location);

				//ik->SetIKPosition(location);
			}
		}
	}
}

void APickDrop::Move_YAxis(float AxisValue)
{
	if (inProgress)
		return;
	if (FMath::Abs(AxisValue) > 0.0f)
	{
		if (currentlySelectedPawn != nullptr)
		{
			FVector location = currentlySelectedPawn->GetActorLocation();
			location.Y += AxisValue * speed;

			//UCCD_IK* ik = (UCCD_IK*)(Arm->GetComponentByClass(UCCD_IK::StaticClass()));
			float distance = FVector::Distance(Arm->GetActorLocation(), location);
			if (distance <= ik->chainLength)
			{
				currentlySelectedPawn->SetActorLocation(location);
				//ik->SetIKPosition(location);

			}
		}
	}
}

void APickDrop::Move_ZAxis(float AxisValue)
{
	if (inProgress)
		return;
	if (FMath::Abs(AxisValue) > 0.0f)
	{
		if (currentlySelectedPawn != nullptr)
		{
			FVector location = currentlySelectedPawn->GetActorLocation();
			location.Z += AxisValue * speed;

			float distance = FVector::Distance(Arm->GetActorLocation(), location);
			if (distance <= ik->chainLength && location.Z >= Arm->GetActorLocation().Z)
			{
				currentlySelectedPawn->SetActorLocation(location);
				//ik->SetIKPosition(location);
			}
		}
	}
}

void APickDrop::Pick()
{
	if (inProgress)
		return;

	inProgress = true;
	actorMovingAlongSpline->SetActorLocation(*(ik->GetIKPosition()));

	path = CreatePath(*(ik->GetIKPosition()), pickUpPawn->GetActorLocation() + FVector(0,0,52));
	//UE_LOG(LogTemp, Warning, TEXT("%d"), path.Num());


	USplineComponent* spline;
	AiTSpline* a;
	UiTween::GenerateSplineFromVectorArray(a, spline, FVector::ZeroVector, FRotator::ZeroRotator, path);


	AiTweenEvent* e = UiTween::ActorMoveToSplinePointSimple(TEXT("move"), actorMovingAlongSpline, spline, true,false,2.0f);
	e->onTweenCompleteTarget = this;  e->OnTweenCompleteFunctionName = "Drop";
	e->BindDelegates();
}

void APickDrop::Drop()
{
	//UE_LOG(LogTemp, Warning, TEXT("Drop"));
	target->AttachToActor(actorMovingAlongSpline, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	target->SetActorRelativeLocation(FVector(0, 0, -52));
	
	path = CreatePath(*(ik->GetIKPosition()), dropOffPawn->GetActorLocation() + FVector(0, 0, 52));
	USplineComponent* spline;
	AiTSpline* a;
	UiTween::GenerateSplineFromVectorArray(a, spline, FVector::ZeroVector, FRotator::ZeroRotator, path);


	AiTweenEvent* ie = (AiTweenEvent*)UiTween::SpawnEvent(UiTween::GetAux());

	ie->eventType = EEventType::EventType::actorMoveToSplinePoint;
	ie->tweenName = TEXT("Drop");
	ie->actorTweening = actorMovingAlongSpline;
	ie->splineComponent = spline;
	ie->interpolateToSpline = true;
	ie->tickTypeValue = 2.0f;
	ie->onTweenCompleteTarget = this;
	ie->OnTweenCompleteFunctionName = "Reset";
	ie->delay = 2.0f;
	ie->delayType = EDelayType::DelayType::first;

	ie->InitEvent();
}

void APickDrop::Reset()
{
	target->AttachToActor(dropOffPawn, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	path = CreatePath(*(ik->GetIKPosition()), ikDefaultPosition);
	USplineComponent* spline;
	AiTSpline* a;
	UiTween::GenerateSplineFromVectorArray(a, spline, FVector::ZeroVector, FRotator::ZeroRotator, path);


	AiTweenEvent* ie = (AiTweenEvent*)UiTween::SpawnEvent(UiTween::GetAux());

	ie->eventType = EEventType::EventType::actorMoveToSplinePoint;
	ie->tweenName = TEXT("Reset");
	ie->actorTweening = actorMovingAlongSpline;
	ie->splineComponent = spline;
	ie->interpolateToSpline = true;
	ie->tickTypeValue = 2.0f;
	ie->onTweenCompleteTarget = this;
	ie->OnTweenCompleteFunctionName = "ResetComplete";
	ie->delay = 1.0f;
	ie->delayType = EDelayType::DelayType::first;

	ie->InitEvent();
}

void APickDrop::ResetComplete()
{
	target->AttachToActor(pickUpPawn, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	inProgress = false;
}

TArray<FVector> APickDrop::CreatePath(FVector startingPosition, FVector targetPosition)
{
	FVector direction = targetPosition - startingPosition;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *(direction.ToString()));
	FVector dirN = direction.GetSafeNormal();
	FVector cross = FVector::CrossProduct(FVector::UpVector, dirN);

	float variance = direction.Size() * 0.25f;
	float pointDeviation = 2.0f;
	float amount = 0;

	TArray<FVector> rawSpline;

	while (amount < 1.0f)
	{
		amount = FMath::Clamp(amount + /*FMath::FRandRange(0.0f, 0.3f)*/0.2f,0.0f, 1.0f);
		FVector point = FMath::LerpStable<FVector>(startingPosition, targetPosition, amount);
		//point += cross * FMath::FRandRange(-variance, variance) * RandomInsideUnitSphere() *FMath::FRandRange(-pointDeviation, pointDeviation);
		//point.X = FMath::Clamp<float>(point.X, FMath::Max<float>(startingPosition.X, targetPosition.X), 2000);
		
		if (amount >= 1.0)
			point = targetPosition;

		rawSpline.Add(point);
	}
	return rawSpline;
}

FVector APickDrop::RandomInsideUnitSphere()
{
	return FRotator(FMath::FRandRange(-90, 90), FMath::FRandRange(0, 360), 0).Vector();
}

