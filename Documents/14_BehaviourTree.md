
# BehaviourTree

### OnRepeatTimer 관련코드 주석처리

BehaviourTree 로 AI를 구현할 것이므로, OnRepeatTimer 관련코드는 삭제 혹은 주석처리

MyAIController.h
```
private:
...
	//FTimerHandle RepeatTimerHandle;
	//float RepeatInterval;
```

MyAIController.cpp
```
// Fill out your copyright notice in the Description page of Project Settings.


...

AMyAIController::AMyAIController()
{
	//RepeatInterval = 3.0f;
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//OnRepeatTimer는 BehaviourTree 사용하기 전에 간단히 테스트 하고 나서 주석처리한다.
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMyAIController::OnRepeatTimer, RepeatInterval, true);
}

void AMyAIController::OnUnPossess()
{
	Super::OnUnPossess();
	//OnRepeatTimer는 BehaviourTree 사용하기 전에 간단히 테스트 하고 나서 주석처리한다.
	//GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}


```

### BehaviorTree

AI 폴더 생성

![image](https://user-images.githubusercontent.com/29656900/188062592-aa2539a0-4b61-49e8-99a7-500d30d8a13a.png)


인공지능 메뉴에서 BB_MyCharacter 이름으로 블랙보드 애셋을, BT_MyCharacter 라는 이름으로 비헤이비어 트리 애셋을 생성
![image](https://user-images.githubusercontent.com/29656900/188062684-ea946ddf-2e2c-4dd0-975d-eeec616f56e1.png)

![image](https://user-images.githubusercontent.com/29656900/188062761-d191efa3-c07e-4f49-9bdc-1876bc6c5fde.png)

![image](https://user-images.githubusercontent.com/29656900/192129921-9f1a0843-86dd-4669-96c5-7c510593c01e.png)


블랙보드 : 인공지능의 판단에 사용하는 데이터 집합
비헤이비어 트리 : 블랙보드의 데이터에 기반해 설계한 트리 정보를 저장한 애셋

블랙보드에서는 우리가 사용할 키들을 추가한다. HomePos 와 PatrolPos 는 Vector 로, Target 은 Base Class 를 MyCharacter 로 갖는 Object Type 으로 만든다.

![image](https://user-images.githubusercontent.com/29656900/189904603-88d4ba09-b238-44dc-998e-8755bab7e8e6.png)

![image](https://user-images.githubusercontent.com/29656900/192130563-d89ff5e6-00c0-4555-bb0a-7ede082cac86.png)



BehaviourTree

태스크는 독립적으로 실행될 수 없고, 반드시 컴포짓(Composite) 노드를 거쳐 실행돼야 한다. 컴포짓에는 Selector, Sequence, Simple Parallel 이 있다.

Sequence : 연결된 Task 들이 False 결과가 나올 때까지 왼쪽에서 오른쪽으로 태스크를 계속 실행함
Selector : 지정된 키값이나 조건을 기준으로 조건에 맞는 노드를 실행
Simple Parallel : 자식으로 붙은 태스크들이 동시에 실행됨.  Finish Mode 설정을 통해서 메인 테스크가 완료되면, 서브 트리를 중단시키고 즉시(Immediate) 노드를 완료시킬수도 있고. 서브 트리를 완료할 때까지 지연(Delayed) 시킬지를 설정할 수 있음.


비헤이비어 트리는 5개의 요소로 구성된다. Root, Composite(Sequence, Selector, Simple Parallel), Decorator, Service, Task. 컴포짓은 위에서 설명했으니, 나머지 4개의 기능을 정리해보면 :

Root : 비헤이비어 트리의 시작점. 데코레이터나 서비스를 덧붙일 수 없음. 블랙보드 애셋을 설정할 수 있음.
Composite : Sequence, Selector, Simple Parallel (위에서 설명)
Decorator : 조걸절. 컴포짓이나 태스크에 붙여 분기나 노드가 실행될 것인지를 정의한다. 종류는 16 가지가 있다.
Service : 서비스는 컴포짓 노드에 분기가 실행되는 동안 정해진 빈도에 맞춰서 실행된다. 보통 검사를 하고 그 검사를 바탕으로 블랙보드의 내용을 업데이트하는데 사용된다.
Task : AI 의 이동이나 블랙보드의 값 조정 등의 작업을 한다. 태스크 노드의 종류에는 7 가지가 있다.


![image](https://user-images.githubusercontent.com/29656900/188063698-1e04ec5a-733a-4b82-8215-1b466ef17385.png)

![image](https://user-images.githubusercontent.com/29656900/188064051-5022360b-1869-42cd-ba94-417ac225152e.png)

![image](https://user-images.githubusercontent.com/29656900/192129864-cdd4f5d5-d805-4cef-8bbc-7a081e120dc1.png)


### MyAIController 

블랙보드 키로 쓰기 위한 FNAME 
MyAIController.h
```
public :
...
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	

```
MyAIController.cpp
```
#include "MyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

const FName AABAIController::HomePosKey(TEXT("HomePos"));
const FName AABAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AABAIController::TargetKey(TEXT("Target"));


AMyAIController::AMyAIController()
{
	//RepeatInterval = 3.0f;
}

...
```

### AIController Blueprint

C++ MyAIController class 기반 블루프린트 AI폴더에 생성 , 이름은 BP_MyAIController

![image](https://user-images.githubusercontent.com/29656900/188126206-41ab4be1-a58e-4110-b182-e04fab1ee4c3.png)

![image](https://user-images.githubusercontent.com/29656900/188126532-6dc98575-ab40-49f7-a9fe-652357c08060.png)

![image](https://user-images.githubusercontent.com/29656900/188126647-44290b50-fd52-4683-b920-0cab9b94a495.png)



### NPC 추격 기능을 위한 Detect 구현
BTService_Detect 는 BTService 를 부모로 하여 생성한다.

비헤이비어 트리의 서비스 노드는 자신이 속한 컴포짓 노드가 활성화될 경우 주기적으로 TickNode 함수를 호출한다. 호출하는 주기는 서비스 노드 내부에 설정된 Interval 속성 값으로 지정할 수 있다.

TickNode 함수에는 반경 6미터 내에 캐릭터가 있는지 감지하는 기능을 넣고(OverlapMultiByChannel), 캐릭터 정보는 TArray 로 전달한다.

![image](https://user-images.githubusercontent.com/29656900/188064493-6739b44c-14c8-40cd-ab6e-6dccb186b053.png)


![image](https://user-images.githubusercontent.com/29656900/188064401-fcb95b93-93d2-416f-8f29-48c49d9f0b98.png)

BTService_Detect.h
```

#include "EngineMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect.generated.h"

/**
 * 
 */
UCLASS()
class CPPTEST01_API UBTService_Detect : public UBTService
{
	GENERATED_BODY()
public:
		UBTService_Detect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};

```



BTService_Detect.cpp
```
#include "BTService_Detect.h"
#include "MyAIController.h"
#include "../MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AMyCharacter* MyCharacter = Cast<AMyCharacter>(OverlapResult.GetActor());
			if (MyCharacter && MyCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::TargetKey, MyCharacter);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, MyCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), MyCharacter->GetActorLocation(), FColor::Blue, false, 0.27f);
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::TargetKey, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
```

### Behaviour Tree 에 Detect Service 추가

![image](https://user-images.githubusercontent.com/29656900/188126876-6c9eb334-876b-4847-85ea-7f85dec99291.png)
![image](https://user-images.githubusercontent.com/29656900/189906298-2e555081-8ca9-4b04-a3cd-3761515ff649.png)
![image](https://user-images.githubusercontent.com/29656900/189906549-ceff0c77-2f39-46e1-a5e8-fac5963ffba2.png)


### FindPatrol
BTTaskNode 클래스 상속하여 BTTaskNode_FindPatrolPos 클래스 
![image](https://user-images.githubusercontent.com/29656900/189899877-6e2700f6-429b-45fe-a7f3-23d1cbb1be2d.png)
![image](https://user-images.githubusercontent.com/29656900/189900017-a7d51a58-8457-4651-b676-57fbb37122e2.png)

BTTaskNode_FindPatrolPos.h
```
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPos.generated.h"

/**
 *
 */
UCLASS()
class ARENABATTLE_API UBTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()

	public:
	UBTTask_FindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
```

BTTaskNode_FindPatrolPos.cpp
```
// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindPatrolPos.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
		return EBTNodeResult::Failed;

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMyAIController::HomePosKey);
	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMyAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
```
![image](https://user-images.githubusercontent.com/29656900/189902774-13ceeaf5-dcbe-4339-ae68-8fbc618d5259.png)
![image](https://user-images.githubusercontent.com/29656900/189903176-f22ca9f5-76ec-4594-a03c-d04c11b539c0.png)

![image](https://user-images.githubusercontent.com/29656900/189904782-842def26-8060-4059-a80f-3a06df25a75f.png)


### NPC공격

Selector 컴포짓으로 변경 (Selector 컴포짓을 새로 만들어서 TargetOn데코레이터를 드래그해 옮긴다)
![image](https://user-images.githubusercontent.com/29656900/190150405-f0d62cae-8cc8-4547-a91e-cb4637b4849a.png)
![image](https://user-images.githubusercontent.com/29656900/190150545-1fdbf3b1-f105-4036-9ccc-bdd5b85b0fcb.png)

![image](https://user-images.githubusercontent.com/29656900/190150792-4e01a574-a8dc-4349-9b61-e5b99acf8859.png)

공격 범위 내에 있는지 판단하는 데코레이터를 생성한다. BTDecorator 를 상속하고 , 이름은 BTDecorator_IsInAttackRange 로 지정한다.
![image](https://user-images.githubusercontent.com/29656900/190151331-5dc12156-7200-449c-88fe-f688947e0f9e.png)

데코레이터 클래스는 CalculateRawConditionValue 함수를 상속받아 원하는 조건이 달성됐는지를 파악하도록 설계됐다. 이 함수는 const로 선언돼 데코레이터 클래스의 멤버 변수 값은 변경할 수 없다.

BTDecorator_InInAttackRange.h
```
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class CPPTEST01_API UBTDecorator_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_IsInAttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
```

BTDecorator_IsInAttackRange.cpp
```
#include "BTDecorator_IsInAttackRange.h"
#include "MyAIController.h"
#include "../MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f);
	return bResult;

}
```

완성된 데코레이터를 가장 왼쪽에 위치한 시퀀스 컴포짓에 부착한다.
![image](https://user-images.githubusercontent.com/29656900/190154067-fa8e97d4-6105-4eac-875a-8af87cd0c573.png)

공격 수행하는 코드를 작성하기 전에 1.5초간 Wait

반대쪽의 오른쪽 시퀀스 컴포짓에 동일한 데코레이터를 추가 하고 Inverse Condition속성값을 체크해 조건을 반대로 설정한다.
![image](https://user-images.githubusercontent.com/29656900/190155180-905e2f0c-9629-4b89-81aa-b1c987ce114b.png)
![image](https://user-images.githubusercontent.com/29656900/190156090-83a3e4af-7124-4743-9832-328d7c02b5fa.png)


이제 Wait 대신 실제로 플레이어를 공격할 테스크 생성

BTTask를 상속받아 BTTask_Attack 클래스 생성.
![image](https://user-images.githubusercontent.com/29656900/190157113-54734345-2fa5-4fab-b46f-84536f9b6782.png)

공격 태스크는 공격 애니메이션이 끝날 때까지 대기해야 하는 지연 태스크이므로 ExecuteTask의 결과 값을 InProgress로 일단 반환하고 공격이 끝났을때 태스크가 끝났다고 알려줘야 한다. 이를 알려주는 함수가 FinishLatentTask다. 태스크에서 이 함수를 나중에 호출해주지 않으면 비헤이비어 트리 시스템은 현재 태스크에 계속 머물게 된다. 

차후에 FinishLatentTask를 호출할 수 있도록 노드의 Tick 기능을 활성화하고 Tick에서 조건을 파악한 후 태스크 종료 명령을 내려줘야 한다.

BTTask_Attack.h
```
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class CPPTEST01_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};

```
BTTask_Attack.cpp
```
#include "BTTask_Attack.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
```

이번에는 실제로 캐릭터에 공격 명령을 내리고 공격이 끝났을 때 태스크를 종료하도록 구현한다.

먼저 AI컨트롤러에서도 공격 명령을 내릴 수 있도록 MyCharacter 클래스의 Attack 함수를 public 섹터로 이동시킨다.
플레이어의 공격이 종료되면 공격 태스크에서 해당 알림을 받을 수 있도록 델리게이트를 새로 선언하고 공격이 종료될 때 이를 호출한다. 캐릭터의 델리게이트 선언후 태스크에서 람다 함수를 해당 델리게이트에 등록하고 Tick 함수 로직에서 FinishLatentTask 함수를 호출하여 태스크를 종료한다.

MyCharacter.h
```
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class CPPTEST01_API AMyCharacter : public ACharacter
{
...
public:
...
//public 으로 옮긴다.
void Attack();
FOnAttackEndDelegate OnAttackEnd;

private:
...
UFUNCTION()
void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
```

MyCharacter.cpp
```
void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	...
	if (MyAnim)
		MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
	...
}
void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

OnAttackEnd.Broadcast();
}
```

BTTask_Attack.h
```
...
private:
	bool IsAttacking = false;
```

BTTask_Attack.cpp
```
#include "BTTask_Attack.h"
#include "MyAIController.h"
#include "../MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MyCharacter = Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MyCharacter)
		return EBTNodeResult::Failed;
		
	auto Target = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - MyCharacter->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	MyCharacter->SetActorRotation(TargetRot);
	

	MyCharacter->Attack();
	IsAttacking = false;
	MyCharacter->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
}
```
Attack Task 를 추가하고 Wait Task는 1.5초로 
![image](https://user-images.githubusercontent.com/29656900/190167650-a6c34266-4d9b-4bac-a9ad-028f85c4a34f.png)

