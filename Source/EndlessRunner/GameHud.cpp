// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHud.h"
#include "RunnerGameModeBase.h"
#include "Components/TextBlock.h"

void UGameHud::InitializeHUD(ARunnerGameModeBase* RunGameMode)
{
	if (RunGameMode) 
	{
		CoinsCount->SetText(FText::AsNumber(0));	
		RunGameMode->OnCoinsCountChanged.AddDynamic(this, &UGameHud::SetCoinsCount);
	}
}

void UGameHud::SetCoinsCount(const int32 Count)
{
	CoinsCount->SetText(FText::AsNumber(Count));
}
