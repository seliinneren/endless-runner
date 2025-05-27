// Fill out your copyright notice in the Description page of Project Settings.

#include "RunnerGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "GameHud.h"
#include "FloorTile.h"


void ARunnerGameModeBase::BeginPlay()
{

	GameHud = Cast<UGameHud>(CreateWidget(GetWorld(), GameHudClass));
	check(GameHud);

	GameHud->InitializeHUD(this);

	GameHud->AddToViewport();

	NumberOfLives = MaxLives;

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
			FloorTiles.Add(Tile);

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

void ARunnerGameModeBase::PlayerDied()
{
	NumberOfLives -= 1;
	OnLivesCountChanged.Broadcast(NumberOfLives);

	if (NumberOfLives > 0)
	{
		for (int32 i = FloorTiles.Num() - 1; i >= 0; i--)
		{
			AFloorTile* Tile = FloorTiles[i];
			if (Tile != nullptr)
			{
				Tile->DestroyFloorTile();
			}
		}
		FloorTiles.Empty();

		NextSpawnPoint = FTransform();
		CreateInitalFloorTiles();
		OnLevelReset.Broadcast();
	}
	else
	{
		GameOver();
	}
}

void ARunnerGameModeBase::GameOver()
{
	if (IsValid(GameOverScreenClass)) 
	{	
		UUserWidget* Widget = CreateWidget(GetWorld(), GameOverScreenClass);

		if (Widget) 
		{
			Widget->AddToViewport();
		}
	}
}

void ARunnerGameModeBase::RemoveTile(AFloorTile* Tile)
{
	FloorTiles.Remove(Tile);
}


