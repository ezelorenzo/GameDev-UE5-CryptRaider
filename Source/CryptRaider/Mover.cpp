// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();

	// ...
	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//AActor* Owner = GetOwner();

	///*float myFloat = 5;
	//float* yourFloat = &myFloat;
	////if I want to print my yourFloat I cant do UE_LOG(LogTemp, Display, TEXT("yourFloat Value: %f"), yourFloat)
	////Because the address is what is going to be printed and is not a float so it is going to create a problem	
	////I need to get the value 
	////float FloatValue = *yourFloat; //this is correct but I am creating an unnecessary variable.
	////when we use the * after the type it means it is a pointer.
	////when we use the * before a pointer expression it means to get the value at that location.
	////UE_LOG(LogTemp, Display, TEXT("yourFloat Value: %f"), *yourFloat) //this was a test so I will delete it.
	////UE_LOG(LogTemp, Display, TEXT("Mover Owner Address %u"), Owner);	*/

	//FString Name = *Owner->GetActorNameOrLabel();
	//FString ownerVector = *Owner->GetActorLocation().ToCompactString();
	//UE_LOG(LogTemp, Display, TEXT("Mover Owner Name %s"), *Name);
	//UE_LOG(LogTemp, Display, TEXT("Mover Owner Vector %s"), *ownerVector);// all this was practice

		
	if (ShouldMove)
	{
		FVector currentLocation = GetOwner()->GetActorLocation();
		FVector targetLocation = OriginalLocation + Moveoffset;
		float Speed = FVector::Distance(OriginalLocation, targetLocation) / MoveTime;
		FVector newLocation = FMath::VInterpConstantTo(currentLocation, targetLocation, DeltaTime, Speed);
		GetOwner()->SetActorLocation(newLocation);
	}	
}

void UMover::SetShouldMove(bool NewShouldMove)
{
	ShouldMove = NewShouldMove;
}

