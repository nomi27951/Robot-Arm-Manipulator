// Fill out your copyright notice in the Description page of Project Settings.


#include "CCD_IK.h"



// Sets default values for this component's properties
UCCD_IK::UCCD_IK()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	maxIterations = 10;
	tolerance = 0.0f;
}


// Called when the game starts
void UCCD_IK::BeginPlay()
{
	Super::BeginPlay();

	InitiateBones();

	SetIKPosition(FVector(x, y, z));
	/*FQuat t = FromToRotation(FVector::UpVector, FVector::OneVector);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *(FVector::OneVector.Rotation().ToString()));*/
}

//Reference all bones and get chain length
void UCCD_IK::InitiateBones()
{
	TInlineComponentArray<UActorComponent*> Components;
	GetOwner()->GetComponents(Components);

	bones.Init(nullptr, 6);
	for (int32 CompIdx = 0; CompIdx < Components.Num(); CompIdx++)
	{
		UActorComponent* Comp = Components[CompIdx];
		FName n = Comp->GetFName();
		if (n.Compare("JOINT 0") == 0)
			bones[0] = (USceneComponent*)Comp;
		else if( n.Compare("JOINT 1") == 0 )
			bones[1] = (USceneComponent*)Comp;
		else if (n.Compare("JOINT 2") == 0)
			bones[2] = (USceneComponent*)Comp;
		else if (n.Compare("JOINT 3") == 0)
			bones[3] = (USceneComponent*)Comp;
		else if (n.Compare("JOINT 4") == 0)
			bones[4] = (USceneComponent*)Comp;
		else if (n.Compare("HAND") == 0)
			bones[5] = (USceneComponent*)Comp;
		/*else if (n.Compare("Target"))
			Target = (UStaticMeshComponent*)Comp;*/
	}

	UE_LOG(LogTemp, Warning, TEXT("Count:  %d"), bones.Num());

	chainLength = 0;

	for (int i = 0; i < bones.Num(); i++)
	{
		//finding out which local axis is directed towards child's position
		if (i < bones.Num() - 1)
		{
			bonesLength.Add((bones[i]->GetComponentLocation() - bones[i + 1]->GetComponentLocation()).Size());
			chainLength += bonesLength[i];
		}
		defaultBonesPosition.Add(bones[i]->GetRelativeLocation());
		defaultBonesRotation.Add(bones[i]->GetRelativeRotation());
	}

	rotationLimits.Emplace(new RotationLimit(0.0f, 0.0f, FVector(0, 1, 0), false, bones[0], defaultBonesRotation[0].Quaternion()));
	rotationLimits.Emplace(new RotationLimit(-130, 145, FVector(1, 0, 0), true, bones[1], defaultBonesRotation[1].Quaternion()));
	rotationLimits.Emplace(new RotationLimit(-90, 90, FVector(1, 0, 0), true, bones[2], defaultBonesRotation[2].Quaternion()));
	rotationLimits.Emplace(new RotationLimit(-60, 60, FVector(1, 0, 0), true, bones[3], defaultBonesRotation[3].Quaternion()));
	rotationLimits.Emplace(new RotationLimit(-60, 60, FVector(1, 0, 0), true, bones[4], defaultBonesRotation[4].Quaternion()));
}

//Get target offset to break out of linear singularity issue
FVector UCCD_IK::GetSingularityOffset()
{
	if (!SingularityDetected()) return FVector::ZeroVector;

	FVector IKDirection = (IKPosition - bones[0]->GetComponentLocation()).GetSafeNormal();
	FVector secondaryDirection = bones[bones.Num() - 2]->GetComponentRotation().Quaternion() * rotationLimits[rotationLimits.Num() - 1]->hingeAxis;



	return FVector::CrossProduct(IKDirection, secondaryDirection) * bonesLength[bonesLength.Num() - 2] * 0.5f;
}

//Detects linear singularity if vector from FIRST bone to TARGET matches the vector from FIRST bone to LAST bone. 
bool UCCD_IK::SingularityDetected()
{
	FVector toLastBone = bones[bones.Num() - 1]->GetComponentLocation() - bones[0]->GetComponentLocation();
	FVector toTarget = IKPosition - bones[0]->GetComponentLocation();

	float toLastBoneDistance = toLastBone.Size();
	float toTargetDistance = toTarget.Size();

	if (toLastBoneDistance == 0 || toTargetDistance == 0 || toLastBoneDistance < toTargetDistance) return false;
	if (toLastBoneDistance < chainLength - (bonesLength[bonesLength.Num() - 2] * 0.1f)) return false;

	float dot = FVector::DotProduct(toLastBone.GetUnsafeNormal(), toTarget.GetUnsafeNormal());
	if (dot < 0.999f) return false;

	return true;
}




// Called every frame
void UCCD_IK::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...

	FVector singularityOffset = maxIterations > 1 ? GetSingularityOffset() : FVector::ZeroVector;

	//iterating the solver
	//FixTransform();
	for (int i = 0; i < maxIterations; i++)
	{

		//optimizations
		//if (singularityOffset == FVector::ZeroVector && i >= 1 && tolerance > 0) break;
		Solve(IKPosition+(i == 0 ? singularityOffset : FVector::ZeroVector));
	}
}

FVector* UCCD_IK::GetIKPosition()
{
	return &IKPosition;
}

void UCCD_IK::SetIKPosition(FVector pos)
{
	IKPosition = GetOwner()->GetTransform().InverseTransformPosition(pos);
}

// Solves the CCD algorithm
void UCCD_IK::Solve(FVector targetPosition)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *(targetPosition.ToString()));
	for (int i = bones.Num() - 2; i > -1; i--)
	{
		//CCD tends to overemphasise the rotations of the bones closer to the target position.
		//we can reduce this by reducing weight down the hierarchy but for now lets just complete this without weights

		FVector toLastBone = bones[bones.Num() - 1]->GetComponentLocation() - bones[i]->GetComponentLocation();
		FVector toTarget = targetPosition - bones[i]->GetComponentLocation();

		//Get the rotation to direct the last bone towards target
		FQuat targetRotation =  FromToRotation(toLastBone, toTarget) * bones[i]->GetComponentQuat();

		
		bones[i]->SetWorldRotation(targetRotation);

		rotationLimits[i]->Apply();
	}

}

void UCCD_IK::FixTransform()
{
	for (int i = 0; i < bones.Num(); i++)
	{
		if (bones[i]->GetRelativeLocation() != defaultBonesPosition[i]) bones[i]->SetRelativeLocation(defaultBonesPosition[i]);
		if (bones[i]->GetRelativeRotation() != defaultBonesRotation[i]) bones[i]->SetRelativeRotation(defaultBonesRotation[i]);
	}
}

FQuat UCCD_IK::AngleAxis(float angle, FVector axis)
{
	axis.Normalize();
	float rad = FMath::DegreesToRadians(angle) * 0.5f;
	axis *= FMath::Sin(rad);
	return FQuat(axis.X, axis.Y, axis.Z, FMath::Cos(rad));
}

FQuat UCCD_IK::FromToRotation(FVector from, FVector to)
{
	from.Normalize();
	to.Normalize();
	FVector axis = FVector::CrossProduct(from, to);
	float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(from.GetSafeNormal(), to.GetSafeNormal())));
	return AngleAxis(angle, axis);
}

