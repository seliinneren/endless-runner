// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerGameModeBase.h"

void ARunnerGameModeBase::BeginPlay()
{
	CreateInitalFloorTiles();
}

void ARunnerGameModeBase::CreateInitalFloorTiles()
{
	AFloorTile* Tile = AddFloorTile();
	if (Tile)
	{
		LaneSwitchValues.Add(Tile->LeftLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->CenterLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->RightLane->GetComponentLocation().Y);
	}
	for (float Val : LaneSwitchValues)
	{
		UE_LOG(LogTemp, Warning, TEXT("Lane Switch Value: %f"), Val);
	}

	for (int i = 0; i < NumInitalFloorTiles; i++)
	{
		AddFloorTile();
	}
}

AFloorTile* ARunnerGameModeBase::AddFloorTile()
{
	UWorld* World = GetWorld();
	if (World)
	{
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass, NextSpawnPoint);

		if (Tile)
		{
			NextSpawnPoint = Tile->GetAttachTransform();
		}
		return Tile;
	}
	return nullptr;
}


