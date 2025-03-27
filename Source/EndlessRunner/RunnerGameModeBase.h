// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FloorTile.h"
#include "RunnerGameModeBase.generated.h"
/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API ARunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<AFloorTile> FloorTileClass;

	UPROPERTY(EditAnywhere, Category = "Config")
	int32 NumInitalFloorTiles = 10;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	FTransform NextSpawnPoint;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	TArray<float> LaneSwitchValues;

	UFUNCTION(BlueprintCallable)
	void CreateInitalFloorTiles();

	UFUNCTION(BlueprintCallable)
	AFloorTile* AddFloorTile(const bool bSpawnItems);

protected:
	virtual void BeginPlay() override;
	
};
