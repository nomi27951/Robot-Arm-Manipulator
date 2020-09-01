// Fill out your copyright notice in the Description page of Project Settings.


#include "DropOffPawn.h"

// Sets default values
ADropOffPawn::ADropOffPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PickDrop"));

	dropAnchor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drop"));
	dropAnchor->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADropOffPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADropOffPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADropOffPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

