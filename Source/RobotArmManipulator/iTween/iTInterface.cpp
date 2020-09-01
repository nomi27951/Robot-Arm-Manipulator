//Copyright Jared Therriault and Bob Berkbile 2014

#include "iTInterface.h"
#include "../RobotArmManipulator.h"
//#include "iTweenPCH.h"
#include "iTween.h"

UiTInterface::UiTInterface(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void IiTInterface::OnTweenStartNative(AiTweenEvent* eventOperator, AActor* actorTweening /*= nullptr*/, USceneComponent* componentTweening /*= nullptr*/, UWidget* widgetTweening /*= nullptr*/, FName tweenName /*= ""*/)
{
	UiTween::Print("Interface message sending...");
}

void IiTInterface::OnTweenUpdateNative(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FDataTypeValues dataTypeValues, float alphaCompletion /*= 0.f*/)
{
	UiTween::Print("Interface message sending...");
}

void IiTInterface::OnTweenLoopNative(AiTweenEvent* eventOperator, AActor* actorTweening /*= nullptr*/, USceneComponent* componentTweening /*= nullptr*/, UWidget* widgetTweening /*= nullptr*/, FName tweenName /*= ""*/, int32 numberOfLoopSections /*= 0*/, ELoopType::LoopType loopType /*= once*/, bool playingBackward /*= false*/)
{
	UiTween::Print("Interface message sending...");
}

void IiTInterface::OnTweenCompleteNative(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FHitResult sweepHitResultForMoveEvents, bool successfulTransform)
{
	UiTween::Print("Interface message sending...");
}
