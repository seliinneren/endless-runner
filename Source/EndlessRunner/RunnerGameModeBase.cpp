// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerGameModeBase.h"

void ARunnerGameModeBase::BeginPlay()
{
	CreateInitalFloorTiles();
}

void ARunnerGameModeBase::CreateInitalFloorTiles()
{
	for (int i = 0; i < NumInitalFloorTiles; i++)
	{
		AddFloorTile();
	}
}

void ARunnerGameModeBase::AddFloorTile()
{
	UWorld* World = GetWorld();
	if (World)
	{
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass, NextSpawnPoint);

		if (Tile)
		{
			NextSpawnPoint = Tile->GetAttachTransform();
		}
	}
}


