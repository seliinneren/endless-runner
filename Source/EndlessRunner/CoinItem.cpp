// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "RunnerCharacter.h"
#include <Kismet/GameplayStatics.h>

ACoinItem::ACoinItem()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(SceneComponent);
	SphereCollider->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoinMesh"));
	CoinMesh->SetupAttachment(SphereCollider);
	CoinMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate = FRotator(0.0f, 180.0f, 0.0f);


}

void ACoinItem::BeginPlay()
{
	Super::BeginPlay();

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ACoinItem::OnSphereOverlap);
	
}

void ACoinItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunnerCharacter* RunnerCharacter = Cast<ARunnerCharacter>(OtherActor);

	if (RunnerCharacter) 
	{
		if (OverlapSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), OverlapSound, GetActorLocation());
		}

		RunnerCharacter->AddCoin();

		Destroy();
	}
}


