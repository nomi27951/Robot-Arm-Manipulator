//Copyright Jared Therriault and Bob Berkbile 2014
#include "iTSpline.h"
#include "../RobotArmManipulator.h"
//#include "iTweenPCH.h"

AiTSpline::AiTSpline(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	spline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("Spline"));

	RootComponent = spline;
}


