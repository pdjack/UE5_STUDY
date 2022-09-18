### C++ AIController
AIController 를 상속받아서 MyAIController class 생성
![image](https://user-images.githubusercontent.com/29656900/188049913-4f6f2357-b311-4df2-9b5e-1abae5e35f84.png)


### MyCharacter.cpp
클래스 속성을 AIController 클래스 속성을 AMyAIController 로 정하고 AI 의 생성 옵션을 PlacedInWorldOrSpawned 로 설정한다. 그러면 앞으로 레벨에 배치하거나 새롭게 생성되는 MyCharacter 마다 AMyIController 액터가 생성되며, 플레이어가 조종하는 캐릭터를 제외한 모든 캐릭터는 MYAIController 의 지배를 받는다.
```
...
#include "MyAIController.h"

AMyCharacter::AMyCharacter()
{
    ...
    AIControllerClass = AMyAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

```

### 네비게이션 메시
AIController 에 플레이어를 따라가도록 만들기 위해 네비게이션 메시(Navigation Mesh) 기반의 길 찾기 시스템을 이용한다.
![image](https://user-images.githubusercontent.com/29656900/188051087-18b95556-f38e-404c-b439-03d5a89f3950.png)


![image](https://user-images.githubusercontent.com/29656900/188051223-25c68f8f-2a6e-4709-9605-d3c50a79a333.png)


![image](https://user-images.githubusercontent.com/29656900/188051269-2ddc0f55-4872-450a-9008-1869d438a949.png)

![image](https://user-images.githubusercontent.com/29656900/188051305-ca620973-f23d-4e88-9755-75ce2c0d6a02.png)


### 랜덤하게 이동

내비게이션 시스템을 이용해 이동 가능한 목적지를 랜덤으로 가져오는 GetRandomPointInNavigableRadius 함수와 목적지로 폰을 이동시키는 SimpleMoveToLocation 함수를 이용한다.

MyAIController.h
```
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	void OnRepeatTimer();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
};
```

AIController.cpp
```
// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

AMyAIController::AMyAIController()
{
	RepeatInterval = 3.0f;
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMyAIController::OnRepeatTimer, RepeatInterval, true);
}

void AMyAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void AMyAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	ensure(nullptr != CurrentPawn);
	//ABCHECK(nullptr != CurrentPawn);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem) {
		UE_LOG(LogTemp, Warning, TEXT("NavSystem is nullptr!"));
		return;
	}

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(CurrentPawn->GetActorLocation(), 500.0f, NextLocation))
	{
		//UNavigationSystemV1::SimpleMoveToLocation(this, NextLocation.Location);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		UE_LOG(LogTemp,Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
	}
}
```

### AI관련 모듈 추가

cpptest01.build.cs
```
// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class cpptest01 : ModuleRules
{
	public cpptest01(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "NavigationSystem", "AIModule", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
```
