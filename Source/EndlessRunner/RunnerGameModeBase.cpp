// Fill out your copyright notice in the Description page of Project Settings.

#include "RunnerGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "GameHud.h"
#include "FloorTile.h"

void ARunnerGameModeBase::BeginPlay()
{
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	GameHud = Cast<UGameHud>(CreateWidget(GetWorld(), GameHudClass));
	check(GameHud);

	GameHud->InitializeHUD(this);

	GameHud->AddToViewport();

	CreateInitalFloorTiles();
}

void ARunnerGameModeBase::CreateInitalFloorTiles()
{
	AFloorTile* Tile = AddFloorTile(false);
	if (Tile)
	{
		LaneSwitchValues.Add(Tile->LeftLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->CenterLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->RightLane->GetComponentLocation().Y);
	}
	/*for (float Val : LaneSwitchValues)
	{
		UE_LOG(LogTemp, Warning, TEXT("Lane Switch Value: %f"), Val);
	}*/

	AddFloorTile(false);
	AddFloorTile(false);

	for (int i = 0; i < NumInitalFloorTiles; i++)
	{
		AddFloorTile(true);
	}
}

AFloorTile* ARunnerGameModeBase::AddFloorTile(const bool bSpawnItems)
{
	UWorld* World = GetWorld();
	if (World)
	{
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass, NextSpawnPoint);

		if (Tile)
		{
			if (bSpawnItems)
			{
				 Tile->SpawnItems();
			}

			NextSpawnPoint = Tile->GetAttachTransform();
		}
		return Tile;
	}
	return nullptr;
}

void ARunnerGameModeBase::AddCoin()
{
	TotalCoins += 1;

	OnCoinsCountChanged.Broadcast(TotalCoins);
}

