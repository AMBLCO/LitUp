// Fill out your copyright notice in the Description page of Project Settings.


#include "LitUpMirror.h"

// Sets default values
ALitUpMirror::ALitUpMirror()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Cr�er le mirroir et d�finir son mesh et son mat�riau
	Mirror = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mirror"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	static ConstructorHelpers::FObjectFinder<UMaterial>mirrorMaterial(TEXT("Material '/Game/CustomActors/Materials/M_Mirror'"));
	Mirror->SetMaterial(0, mirrorMaterial.Object);
	Mirror->SetStaticMesh(CubeMeshAsset.Object);
	SetRootComponent(Mirror);
}

// Called when the game starts or when spawned
void ALitUpMirror::BeginPlay()
{
	Super::BeginPlay();
	Mirror->SetStaticMesh(MirrorMeshAsset);  //Appliquer le mesh choisi sur l'actor
}

// Called every frame
void ALitUpMirror::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// This ultimately is what controls whether or not it can even tick at all in the editor view port. 
//But, it is EVERY view port so it still needs to be blocked from preview windows and junk.
bool ALitUpMirror::ShouldTickIfViewportsOnly() const
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