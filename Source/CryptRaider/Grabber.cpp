// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);	

	UPhysicsHandleComponent* PhysicsHandle = CreatePhysicsHandle();
	//components can be dangerous because sometimes can be null and make the game crash.
	//in that case PhysicsHandle pointer will be nullptr and if we use the arrow or something to call whatever should be inside that handler
	//it will crash
	if (PhysicsHandle == nullptr)
	{
		return;//dont finish the rest of the function
	}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}	
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = CreatePhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;//dont finish the rest of the function
	}
	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = CreatePhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;//dont finish the rest of the function
	}

	/*FRotator MyRotation = GetComponentRotation();
	FString RotationString = MyRotation.ToCompactString();

	float WorldTime = GetWorld()->TimeSeconds;

	UE_LOG(LogTemp, Display, TEXT("Grabber Rotation: %s"), *RotationString);
	UE_LOG(LogTemp, Display, TEXT("Grabber Time: %f"), WorldTime);*/ //practica	
	FHitResult HitResult;
	bool HasHit = GetGrabbleInReach(HitResult);
	if(HasHit)
	{
		//DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 5);
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);
		//AActor* HitActor = HitResult.GetActor();		
		//UE_LOG(LogTemp, Display, TEXT("Hit Actor: %s"), *HitActor->GetActorNameOrLabel());//practice

		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();

		HitResult.GetActor()->Tags.Add("Grabbed");

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,//we dont have a skeletal mesh
			HitResult.ImpactPoint,
			GetComponentRotation()
		);

	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No Actor Being Hit"));
	}
	//FQuat::Identity //no rotation
	/*float Damage = 5;
	if (HasDamage(Damage))
	{
		PrintDamage(Damage);
	}*/ //practice
	//float& DamageRef = Damage;//set up a reference to the variable.
	//DamageRef = 5;//we re set de value to test if damageref is pointing damage
	//UE_LOG(LogTemp, Display, TEXT("DamageRef: %f, Damage: %f"), DamageRef, Damage);
}

UPhysicsHandleComponent* UGrabber::CreatePhysicsHandle() const
{
	UPhysicsHandleComponent* result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber Requires a UPhysicsHandleComponent."));
	}
	return result;
}

bool UGrabber::GetGrabbleInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + (GetForwardVector() * MaxGrabDistance);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);
		
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);	
}

	

//void UGrabber::PrintDamage(const float& Damage)
//{
//	UE_LOG(LogTemp, Display, TEXT("Damage: %f"), Damage);
//}
//
//bool UGrabber::HasDamage(float& OutDamage)
//{
//	OutDamage = 2;
//	return true;
//}//practice

//the real place where references shine is in parameters to functions because everytime we create a float deltatime in a tick component
//what we are doing is taking a copy of that original float and we are passing it into the function.
//what i want to do is pass the reference into the function.

