// Fill out your copyright notice in the Description page of Project Settings.


#include "RotationLimit.h"

RotationLimit::RotationLimit()
{
}

RotationLimit::~RotationLimit()
{
}

RotationLimit::RotationLimit(float min, float max, FVector hingeAxis, bool useLimits, USceneComponent* joint, FQuat rotation)
	: minAngle(min), maxAngle(max), hingeAxis(hingeAxis), useLimits(useLimits), bone(joint), defaultLocalRotation(rotation)
{
	lastRotation = FQuat::Identity;
	lastAngle = 0;
}

bool RotationLimit::Apply()
{
	bool changed = false;
	//joint->rotation
	bone->SetRelativeRotation(GetLimitedLocalRotation(bone->GetRelativeRotation().Quaternion(), changed));
	return changed;
}

//Returns the limited local rotation
FQuat RotationLimit::GetLimitedLocalRotation(FQuat localRotation, bool& changed)
{
	//subtracting default local rotation
	FQuat rotation = defaultLocalRotation.Inverse() * localRotation;
	FQuat limitedRotation = LimitHinge(rotation);

	changed = limitedRotation != rotation;

	if (!changed) return localRotation;

	return defaultLocalRotation * limitedRotation;
}

FQuat RotationLimit::LimitHinge(FQuat rotation)
{
	//if limit is zero return rotation fixed to axis
	if (minAngle == 0 && maxAngle == 0 && useLimits)
		return AngleAxis(0.0f, hingeAxis);//FVector::ZeroVector.RotateAngleAxis(0.0f, hingeAxis).Rotation().Quaternion();

	//Get 1DOF rotation along axis
	FQuat free1DOF = Limit1DOF(rotation, hingeAxis);
	if (!useLimits)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *(free1DOF.Euler().ToString()));
		return free1DOF;
	}

	//Get offset from last rotation in terms of angle axis
	FQuat addR = free1DOF * lastRotation.Inverse();
	float addA = AngleBetweenQuat(FQuat::Identity, addR);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), lastAngle + addA);

	FVector secondaryHingeAxis = FVector(hingeAxis.Z, hingeAxis.X, hingeAxis.Y);
	FVector cross = FVector::CrossProduct(secondaryHingeAxis, hingeAxis);

	if (FVector::DotProduct(addR * secondaryHingeAxis, cross) > 0.0f)
		addA = -addA;

	//clamp to limits outside restricted movement
	/*float t = (lastAngle + addA - minAngle) / (maxAngle - minAngle);
	UE_LOG(LogTemp, Warning, TEXT("%f"),t);
	if (t > 1.0f || t < 0)
		lastAngle = lastAngle + addA;*/
	lastAngle = FMath::Clamp(lastAngle + addA, minAngle, maxAngle);

	lastRotation = AngleAxis(lastAngle, hingeAxis);
	return lastRotation;
}

FQuat RotationLimit::Limit1DOF(FQuat rotation, FVector axis)
{
	return FromToRotation(rotation * axis, axis) * rotation;
}

FQuat RotationLimit::FromToRotation(FVector from, FVector to)
{
	from.Normalize();
	to.Normalize();
	FVector axis = FVector::CrossProduct(from, to);
	float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(from.GetSafeNormal(), to.GetSafeNormal())));
	return AngleAxis(angle, axis);
}

FQuat RotationLimit::AngleAxis(float angle, FVector axis)
{
	axis.Normalize();
	float rad = FMath::DegreesToRadians(angle) * 0.5f;
	axis *= FMath::Sin(rad);
	return FQuat(axis.X, axis.Y, axis.Z, FMath::Cos(rad));
}

float RotationLimit::AngleBetweenQuat(FQuat a, FQuat b)
{
	//2 * acos(dot(a,b))
	float f = a | b;
	f = FMath::Min(FMath::Abs(f), 1.0f);
	return 2 * FMath::RadiansToDegrees(FMath::Acos(f));
}
