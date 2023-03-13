// Fill out your copyright notice in the Description page of Project Settings.


#include "LitUpLightRay.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

//Ajouter les include des Actors ici
#include "LitUpLightTarget.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLightRay, Log, All);
DEFINE_LOG_CATEGORY(LogLightRay);

// Sets default values
ALitUpLightRay::ALitUpLightRay()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Origin = CreateDefaultSubobject<USceneComponent>(TEXT("Origin"));
	SetRootComponent(Origin);

	LightRay = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightRay"));
	LightRay->SetupAttachment(Origin);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CylinderMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	LightRay->SetStaticMesh(CylinderMeshAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial>laserMaterial(TEXT("Material '/Game/Materials/M_Laser.M_Laser'"));
	LightRay->SetMaterial(0, laserMaterial.Object);
	LightRay->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LightRay->SetRelativeTransform(FTransform(FRotator(90, 0, 0), FVector(length / 2.f, 0, 0), FVector(0.05, 0.05, length / 100.f)));
	LightRay->CastShadow = false;
	LightRay->bEmissiveLightSource = true;
}

// Called when the game starts or when spawned
void ALitUpLightRay::BeginPlay()
{
	Super::BeginPlay();

	goNext(false);
}

// Called every frame
void ALitUpLightRay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Start = Origin->GetComponentLocation();
	FVector ForwardVector = Origin->GetForwardVector();
	FHitResult OutHit;
	FVector End = ((ForwardVector * length) + Start);
	FCollisionQueryParams CollisionParams;

	CollisionParams.AddIgnoredActor(GetParentActor());

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.04, 0, 10);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic, CollisionParams))
	{
		LightRay->SetWorldTransform(FTransform(FRotator(90, 0, 0) + ForwardVector.Rotation(), (OutHit.Location - Start) / 2.f + Start, FVector(0.05, 0.05, OutHit.Distance / 100.f)));

		if (OutHit.GetActor()->IsA(ALitUpLightTarget::StaticClass()))
		{
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::`Green, TEXT("Target is getting hit!"));
			//UE_LOG(LogLightRay, Display, TEXT("Target is getting hit!"));

			//Pour tests
			goNext(true);

			FVector Reflection = FMath::GetReflectionVector(ForwardVector, OutHit.Normal);
			
			if (nextLightRay) nextLightRay->SetActorTransform(FTransform(Reflection.Rotation(), OutHit.Location + (Reflection * 0.0001)));
		}
//		else if (OutHit.GetActor()->IsA(ALitUpMirror::StaticClass()))
//		{
//			goNext(true);
//
//			FVector Reflection;
//
//			//Calculer le vecteur ici
//
//			nextLightRay->SetActorTransform(FTransform(Reflection.Rotation(), OutHit.Location + (Reflection * 0.0001)));
//		}
//		else if (OutHit.GetActor()->IsA(ALitUpPrism::StaticClass()))
//		{
//			goNext(true);
//
//			FVector Refraction;
//
//			//Calculer le vecteur ici
//
//			nextLightRay->SetActorTransform(FTransform(Refraction.Rotation(), OutHit.Location + (Refraction * 0.0001)));
//		}
//		else if (OutHit.GetActor()->IsA(ALitUpDiffractor::StaticClass()))
//		{
//			goNext(false);
//
//			//Calculer les trucs ici
//		}

		/*
		Exemple pour ajouter des objets qui interagissent avec la lumi�re

		Prendre exemple ci-haut

		Cr�er une classe Actor
		Ajouter le include du .h en ici en haut
		Ajouter un else if suivant le template ci-dessous
		Mettre la fonction goNext(true/false) true = le rayon de lumi�re continue, false = le rayon de lumi�re s'arr�te
		�crire le code � ex�cuter lorsque l'Actor est touch� dans le if

		else if (OutHit.GetActor()->IsA(/La classe de l'Actor/::StaticClass()))
		{
			goNext(true/false)
		}
		*/

		else
		{
			goNext(false);

			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No hit!"));
			//UE_LOG(LogLightRay, Display, TEXT("No hit!"));
		}
	}
	else
	{
		LightRay->SetRelativeTransform(FTransform(FRotator(90, 0, 0), FVector(length / 2.f, 0, 0), FVector(0.05, 0.05, length / 100.f)));
	}
}

// This ultimately is what controls whether or not it can even tick at all in the editor view port. 
//But, it is EVERY view port so it still needs to be blocked from preview windows and junk.
bool ALitUpLightRay::ShouldTickIfViewportsOnly() const
{
	if (GetWorld() != nullptr && GetWorld()->WorldType == EWorldType::Editor)
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline void ALitUpLightRay::goNext(bool goNext)
{
	if (next == true && goNext == true)
	{
		return;
	}
	else if (next == false && goNext == true && maxRays > 0)
	{
		next = true;

		nextLightRay = GetWorld()->SpawnActor<ALitUpLightRay>(ALitUpLightRay::StaticClass());
		nextLightRay->maxRays = maxRays - 1;
		nextLightRay->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("New ray created!"));
		//UE_LOG(LogLightRay, Display, TEXT("New ray created!"));
	}
	else
	{
		next = false;

		if (nextLightRay)
		{
			nextLightRay->goNext(false);

			nextLightRay->K2_DestroyActor();
			nextLightRay = nullptr;
			if (GEngine) GEngine->ForceGarbageCollection(true);

			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Ray deleted!"));
			//UE_LOG(LogLightRay, Display, TEXT("Ray deleted!"));
		}
	}

	return;
}