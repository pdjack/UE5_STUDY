// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class CPPTEST01_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void PostInitializeComponents() override;


	void MoveForward(float NewAxisValue);

	void MoveRight(float NewAxisValue);

	void Turn(float NewAxisValue);

	void LookUp(float NewAxisValue);

	void Attack();

	void AttackHitCheck();

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY()
	TSubclassOf<UAnimInstance> MALE_ANIM_CLASS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	UStaticMeshComponent* Weapon;

private:
	UPROPERTY()
	class UMyAnimInstance* MyAnim;
};