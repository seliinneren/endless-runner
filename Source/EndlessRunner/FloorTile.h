// Fill out your copyright notice in the Description page of Project Settings.
class ACoinItem;
class AObstacle;
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "FloorTile.generated.h"

UCLASS()
class ENDLESSRUNNER_API AFloorTile : public AActor
{
    GENERATED_BODY()

public:

   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TSubclassOf<AObstacle> SmallObstacleClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    TSubclassOf<AObstacle> BigObstacleClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    TSubclassOf<ACoinItem> CoinItemClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* SceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* FloorMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UArrowComponent* AttachPoint;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UArrowComponent* CenterLane;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UArrowComponent* LeftLane;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UArrowComponent* RightLane;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* FloorTriggerBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> ChildActors;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    float SpawnPercent1 = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    float SpawnPercent2 = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    float SpawnPercent3 = 0.5f;

    UFUNCTION()
    void SpawnItems();

    UFUNCTION(BlueprintCallable)
    void DestroyFloorTile();

    // Sets default values for this actor's properties
    AFloorTile();

    FORCEINLINE const FTransform& GetAttachTransform() const { return AttachPoint->GetComponentTransform(); }

protected:

    UPROPERTY(VisibleInstanceOnly)
	class ARunnerGameModeBase* RunGameMode;

    UPROPERTY()
	FTimerHandle DestroyHandle;

    UFUNCTION()
    void OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
	void SpawnLaneItem(UArrowComponent* Lane, int32& NumBigs);


    // Called when the game starts or when spawned
    virtual void BeginPlay() override;


};