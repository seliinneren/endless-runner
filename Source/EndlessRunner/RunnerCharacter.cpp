
#include "RunnerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "RunnerGameModeBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include <Kismet/GameplayStatics.h>

ARunnerCharacter::ARunnerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->TargetArmLength = 350.f;
	CameraArm->SocketOffset = FVector(0.f, 0.f, 100.f);
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
}

void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);

		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
	RunGameMode = Cast<ARunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(RunGameMode);

	RunGameMode->OnLevelReset.AddDynamic(this, &ARunnerCharacter::ResetLevel);

	PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(),APlayerStart::StaticClass()));

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}


void ARunnerCharacter::AddCoin()
{

	RunGameMode->AddCoin();
}

void ARunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FRotator ControlRot = GetControlRotation();
	ControlRot.Roll = 0.f;
	ControlRot.Pitch = 0.f;

	AddMovementInput(ControlRot.Vector());


}

void ARunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(TouchAction, ETriggerEvent::Started, this, &ARunnerCharacter::OnTouchStarted);
		EnhancedInputComponent->BindAction(TouchAction, ETriggerEvent::Triggered, this, &ARunnerCharacter::OnTouchTriggered);
		EnhancedInputComponent->BindAction(TouchAction, ETriggerEvent::Completed, this, &ARunnerCharacter::OnTouchCompleted);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInputComponent is not an EnhancedInputComponent!"));
	}
}

void ARunnerCharacter::ChangeLaneUpdate(const float Value)
{
	FVector Location = GetCapsuleComponent()->GetComponentLocation();
	Location.Y = FMath::Lerp(RunGameMode->LaneSwitchValues[CurrentLane], RunGameMode->LaneSwitchValues[NextLane], Value);
	SetActorLocation(Location);
}

void ARunnerCharacter::ChangeLaneFinished()
{
	CurrentLane = NextLane;
}

void ARunnerCharacter::Death()
{
	if (!bIsDeath)
	{

		const FVector Location = GetActorLocation();

		UWorld* World = GetWorld();

		if (World)
		{
			bIsDeath = true;
			DisableInput(nullptr);

			if (DeathParticleSystem)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, DeathParticleSystem, Location);
			}

			if (DeathSound)
			{
				UGameplayStatics::PlaySoundAtLocation(World, DeathSound, Location);
			}

			GetMesh()->SetVisibility(false);

			World->GetTimerManager().SetTimer(RestartTimerHandle, this, &ARunnerCharacter::OnDeath, 1.f);
		}
	}
}

void ARunnerCharacter::OnDeath()
{

	if (RestartTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
	}

	RunGameMode->PlayerDied();
}

void ARunnerCharacter::ResetLevel()
{
	bIsDeath = false;
	EnableInput(nullptr);
	GetMesh()->SetVisibility(true);

	if (PlayerStart)
	{
		SetActorLocation(PlayerStart->GetActorLocation());
		SetActorRotation(PlayerStart->GetActorRotation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No PlayerStart found!"));
	}
}

void ARunnerCharacter::MoveRight()
{
	NextLane = FMath::Clamp(CurrentLane + 1, 0, 2);
	ChangeLane();
}

void ARunnerCharacter::MoveLeft()
{
	NextLane = FMath::Clamp(CurrentLane - 1, 0, 2);
	ChangeLane();
}

void ARunnerCharacter::MoveDown()
{
	static FVector Impulse = FVector(0.f, 0.f, MoveDownImpulse);
	GetCharacterMovement()->AddImpulse(Impulse, true);
}

void ARunnerCharacter::OnTouchStarted(const FInputActionInstance& Instance)
{

	bIsTouchTriggered = true;
	TouchStart2D = Instance.GetValue().Get<FVector2D>();
	TouchEnd2D = FVector2D::ZeroVector; 
}

void ARunnerCharacter::OnTouchTriggered(const FInputActionInstance& Instance)
{
	if (!bIsTouchTriggered) return;

	if (TouchStart2D.IsZero())
	{
		TouchStart2D = Instance.GetValue().Get<FVector2D>();
	}

	TouchCurrent2D = Instance.GetValue().Get<FVector2D>();

}

void ARunnerCharacter::OnTouchCompleted(const FInputActionInstance& Instance)
{

	if (!bIsTouchTriggered) return;

	TouchEnd2D = TouchCurrent2D;
	bIsTouchTriggered = false;

	FVector2D Delta = TouchEnd2D - TouchStart2D;

	if (FMath::Abs(Delta.X) > FMath::Abs(Delta.Y))
	{
		if (Delta.X > 0.5f) 
		{
			MoveRight();
		}
		else if (Delta.X < -0.5f) 
		{
			MoveLeft();
		}
	}
	else
	{
		if (Delta.Y > 0.5f) 
		{
			MoveDown();
		}
		else if (Delta.Y < -0.5f) 
		{
			Jump();
		}
	}
}



