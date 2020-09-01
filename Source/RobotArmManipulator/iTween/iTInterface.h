//Copyright Jared Therriault and Bob Berkbile 2014

#pragma once

//#include "iTweenPCH.h"
#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/SlateCore/Public/Widgets/SCompoundWidget.h"
#include "iTAux.h"
#include "iTInterface.generated.h"

/**
*
*/
using namespace ELoopType;
UINTERFACE()
class ROBOTARMMANIPULATOR_API UiTInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IiTInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "On Tween Start"), Category = "iTween|Interface Events")
		void OnTweenStart(AiTweenEvent* eventOperator, AActor* actorTweening = nullptr, USceneComponent* componentTweening = nullptr, UWidget* widgetTweening = nullptr, FName tweenName = "");

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "On Tween Update"), Category = "iTween|Interface Events")
		void OnTweenUpdate(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FDataTypeValues dataTypeValues, float alphaCompletion = 0.f);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "On Tween Loop"), Category = "iTween|Interface Events")
		void OnTweenLoop(AiTweenEvent* eventOperator, AActor* actorTweening = nullptr, USceneComponent* componentTweening = nullptr, UWidget* widgetTweening = nullptr, FName tweenName = "", int32 numberOfLoopSections = 0, ELoopType::LoopType loopType = once, bool playingBackward = false);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "On Tween Complete"), Category = "iTween|Interface Events")
		void OnTweenComplete(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FHitResult sweepHitResultForMoveEvents, bool successfulTransform);

	virtual void OnTweenStartNative(AiTweenEvent* eventOperator, AActor* actorTweening = nullptr, USceneComponent* componentTweening = nullptr, UWidget* widgetTweening = nullptr, FName tweenName = "");

	virtual void OnTweenUpdateNative(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FDataTypeValues dataTypeValues, float alphaCompletion = 0.f);

	virtual void OnTweenLoopNative(AiTweenEvent* eventOperator, AActor* actorTweening = nullptr, USceneComponent* componentTweening = nullptr, UWidget* widgetTweening = nullptr, FName tweenName = "", int32 numberOfLoopSections = 0, ELoopType::LoopType loopType = once, bool playingBackward = false);

	virtual void OnTweenCompleteNative(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FHitResult sweepHitResultForMoveEvents, bool successfulTransform);

};