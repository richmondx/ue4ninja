// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnrealNetwork.h"
#include "NinjaCharacter.generated.h"

// Forward declarations
class UCameraComponent;
class USpringArmComponent;
class UBoxComponent;
class UPaperFlipbookComponent;
class UPaperFlipbook;

UCLASS()
class NINJA_API ANinjaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANinjaCharacter(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const override;

	UFUNCTION(Server, unreliable, WithValidation)
		void SetWorldTime_Server(float scale);
	
	FTimerHandle GetDodgeTimerHandle() const;

	UFUNCTION(Server, reliable, WithValidation)
		void Server_SetIsDodging(bool Dodging);

	UFUNCTION()
		void SetIsDodging(bool Dodging);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "NinjaMovement")
		virtual void MoveRight(float AxisValue);

	UCameraComponent* MainCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Debug")
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ninja Attacking")
		UBoxComponent* AttackHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ninja Attacking")
		FVector AttackHitboxLocation = FVector(0.f, -80.f, 30.f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ninja Sprite")
		UPaperFlipbookComponent* Sprite;

	FTimerHandle DodgeTimerHandle;

	FTimerHandle AttackDashTimerHandle;

	UFUNCTION(BlueprintCallable, Category="Ninja Attacking")
		virtual void Attack();

	UFUNCTION()
		virtual void StopAttack();

	UFUNCTION(BlueprintCallable, Category = "Ninja Dodge")
		virtual void Dodge();

	/**
	Sets Rotation of Sprite to face either left or right
	*/
	virtual void SetSpriteRotation();

	UFUNCTION(Server, unreliable, WithValidation, Category="Ninja Movement")
		virtual void ServerSetSpriteRotation(float Value);

	virtual void SetAttackHitboxLocation(float Value);

	UFUNCTION(Server, unreliable, WithValidation, Category = "Ninja Attacking")
		virtual void ServerSetAttackHitboxLocation(float Value);

	UFUNCTION()
		virtual void HandleAttack();

	UFUNCTION(Server, Unreliable, WithValidation)
		virtual void Server_HandleAttack();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ninja Attacking")
		float AttackAnimLength;

	UPROPERTY()
		float AttackAnimTimer;

	float DashAttackSpeed;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, Category="Debug")
		int32 AttackCurrFrame = 0;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Ninja Movement")
		bool bIsMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ninja Attacking")
		int32 GrndAttackStartFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ninja Attacking")
		int32 GrndAttackEndFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ninja Movement")
		FRotator SpriteRightRot = FRotator(0.f, 90.f, 0.f);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ninja Movement")
		FRotator SpriteLeftRot = FRotator(0.f, -90.f, 0.f);

	UFUNCTION(Server, Reliable, WithValidation, Category = "Ninja Attacking")
		void RegisterHit();

	UFUNCTION(Server, Reliable, WithValidation, Category = "Ninja Attacking")
		void Server_SetIsAttacking(bool Attacking);

	UFUNCTION(NetMulticast, unreliable)
		void SetWorldTime_Client(float scale);

	UFUNCTION()
		void SetWorldTime(float scale);

	UFUNCTION(NetMulticast, Reliable)
		void ShowEndScreen();

	UPROPERTY(BlueprintReadOnly)
		bool bIsDodging;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
		UPaperFlipbook* IdleAnimFlipbook;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
		UPaperFlipbook* WalkAnimFlipbook;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
		UPaperFlipbook* JumpAnimFlipbook;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
		UPaperFlipbook* AttackAnimFlipbook;

	UFUNCTION(BlueprintCallable)
		void HandleAnimations();

	UFUNCTION(Server, unreliable, WithValidation)
		void Server_HandleAnimations(float Dir);

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Debug")
		bool bIsAttacking;

	UFUNCTION(Server, Unreliable, WithValidation)
		void Server_PlayFlipbook(UPaperFlipbook* Flipbook);

	UFUNCTION()
		void PlayFlipbook(UPaperFlipbook* Flipbook);

	UFUNCTION()
		bool IsFlipbookPlaying(UPaperFlipbook* Flipbook) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
