// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LitUpLightTarget.generated.h"

UCLASS()
class LITUP_API ALitUpLightTarget : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent* Target;

	UPROPERTY(EditAnywhere)
		UStaticMesh* TargetMeshAsset; //Mesh de la cible

	UPROPERTY(EditAnywhere)
		bool UseWaveLength = false; //Si on utilise la longeur d'onde comme restriction

	UPROPERTY(EditAnywhere)
		float WaveLength = 780.f; //Longeur d'onde qui peut affecter la cible

	/** Allows Tick To happen in the editor viewport*/
	virtual bool ShouldTickIfViewportsOnly() const override;

public:	
	// Sets default values for this actor's properties
	ALitUpLightTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void exec(float &waveLength);

	UPROPERTY(EditAnywhere, Category = "BlueprintClass")
		AActor* bpActor;

private:
	bool isHit; //Variable qui contient l'�tat de la cible
	double lastTime = 0; //Temps lorsque la cible a commenc� � �tre touch�e
};