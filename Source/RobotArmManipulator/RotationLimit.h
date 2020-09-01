// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ROBOTARMMANIPULATOR_API RotationLimit
{
private:
	float minAngle;
	float maxAngle;
	bool useLimits;

	FQuat defaultLocalRotation;
	FQuat lastRotation;
	float lastAngle;

	USceneComponent* bone;

public:
	RotationLimit();
	~RotationLimit();

	FVector hingeAxis;


	RotationLimit(float min, float max, FVector hingeAxis, bool useLimits, USceneComponent* joint, FQuat rotation);
	bool Apply();
	//Returns the limited local rotation
	FQuat GetLimitedLocalRotation(FQuat localRotation, bool& changed);
	FQuat LimitHinge(FQuat rotation);
	FQuat Limit1DOF(FQuat rotation, FVector axis);
	FQuat FromToRotation(FVector from, FVector to);
	FQuat AngleAxis(float angle, FVector axis);
	float AngleBetweenQuat(FQuat a, FQuat b);
};
