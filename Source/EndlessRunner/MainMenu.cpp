// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu::NativeConstruct()
{
	if(StartBtn)
	{
		StartBtn->OnClicked.AddDynamic(this, &UMainMenu::OnStartButtonClick);
	}
	if (QuitBtn)
	{
		QuitBtn->OnClicked.AddDynamic(this, &UMainMenu::OnQuitButtonClick);
	}
}

void UMainMenu::OnStartButtonClick()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, TEXT("MainLevel"));
	}
}

void UMainMenu::OnQuitButtonClick()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("Quit"));
	}
}

