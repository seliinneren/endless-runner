// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunnerCharacter.generated.h"

UCLASS()
class ENDLESSRUNNER_API ARunnerCharacter : public ACharacter
{
GENERATED_BODY()

protected:

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"));
class USpringArmComponent* CameraArm;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"));
class UCameraComponent* Camera;

UPROPERTY(VisibleInstanceOnly)
class ARunnerGameModeBase* RunGameMode;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
class UInputMappingContext* DefaultMappingContext;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
class UInputAction* JumpAction;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
class UInputAction* MoveRightAction;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
class UInputAction* MoveLeftAction;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
class UInputAction* MoveDownAction;

UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input");
class UInputAction* TouchAction;

public:

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Assets")
class UParticleSystem* DeathParticleSystem;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Assets")
class USoundBase* DeathSound;

UPROPERTY(EditAnywhere, BlueprintReadWrite)
float MoveDownImpulse = -1000.f;

UPROPERTY(VisibleInstanceOnly,BlueprintReadWrite)
int32 CurrentLane = 1;

UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
int32 NextLane = 0;

UFUNCTION(BlueprintImplementableEvent, Category = "Lane")
void ChangeLane();

UFUNCTION(BlueprintCallable, Category = "Lane")
void ChangeLaneUpdate(float Value);

UFUNCTION(BlueprintCallable, Category = "Lane")
void ChangeLaneFinished();

UFUNCTION(BlueprintCallable)
void Death();

// Sets default values for this character's properties
ARunnerCharacter();

protected:
// Called when the game starts or when spawned
virtual void BeginPlay() override;

UFUNCTION()
void OnDeath();

UFUNCTION()
void MoveRight();

UFUNCTION()
void MoveLeft();

UFUNCTION()
void MoveDown();

UFUNCTION()
void OnTouchStarted(const FInputActionInstance& Instance);

UFUNCTION()
void OnTouchTriggered(const FInputActionInstance& Instance);

UFUNCTION()
void OnTouchCompleted(const FInputActionInstance& Instance);

UFUNCTION()
void ResetLevel();

UPROPERTY()
FTimerHandle RestartTimerHandle;

UPROPERTY()
class APlayerStart* PlayerStart;

UPROPERTY()
bool bIsDeath = false;

UPROPERTY()
FVector2D TouchStart2D;

UPROPERTY()
FVector2D TouchEnd2D;

UPROPERTY()
FVector2D TouchCurrent2D;

UPROPERTY()
bool bIsTouchTriggered = false;

public:	

UFUNCTION()
void AddCoin();

// Called every frame
virtual void Tick(float DeltaTime) override;

// Called to bind functionality to input
virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
