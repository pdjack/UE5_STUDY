## Animation Blueprint 
### C++ Anim Instance Class
![image](https://user-images.githubusercontent.com/29656900/183334501-cbdadaf3-16ad-4c01-9c94-36ac08d216df.png)
![image](https://user-images.githubusercontent.com/29656900/183334598-014071fe-7a05-4d9e-8f2e-259e90e55139.png)
![image](https://user-images.githubusercontent.com/29656900/183334630-566c0738-8469-45d4-b6ba-6a8ff1c769fb.png)



Create Animation Blueprint 
![image](https://user-images.githubusercontent.com/29656900/183333625-03370f01-697c-4191-8f4a-dd5188ebb936.png)
캐릭터 스켈레탈 메시와 동일한 스켈레탈메시 선택
![image](https://user-images.githubusercontent.com/29656900/183335319-97fd9ad8-a050-427c-b4b9-6b6db5e17b16.png)
이름은 MyAnimBP 
![image](https://user-images.githubusercontent.com/29656900/183335417-285e3514-157b-4c5f-bab3-a411a0f8d3a6.png)
Class Setting 눌러서 Parent Class 를 C++ Class 인 MyAnimInstance로 변경
![image](https://user-images.githubusercontent.com/29656900/183335561-49991286-a47e-416c-853d-9defcf8046fc.png)

옵션에서 Show Inherited Variables 체크
![image](https://user-images.githubusercontent.com/29656900/183335758-4d3fe918-22f7-421f-aec7-0bf3d974e5d9.png)

Animation Asset 을 드래그 하여 Output Pose 연결하고 Compile & Save
![image](https://user-images.githubusercontent.com/29656900/183335968-6894d57a-fad2-4580-a5d1-ceb13a11b5c7.png)

## Character 에 AnimBP 세팅
### 첫번째 방법 : C++ Class MyCharacter에서 AnimInstance 세팅
AnimBP 레퍼런스 저장

![image](https://user-images.githubusercontent.com/29656900/183338372-f587912f-8261-4c7c-ad68-935cf3042074.png)

MyCharacter.h
```
public:
	UPROPERTY()
	TSubclassOf<UAnimInstance> MALE_ANIM_CLASS;
```
MyCharacter.cpp
```
AMyCharacter::AMyCharacter()
{
...
	//AnimBlueprint'/Game/Blueprints/MyAnimBP.MyAnimBP'
	static ConstructorHelpers::FClassFinder<UAnimInstance> MALE_ANIM(TEXT("/Game/Blueprints/MyAnimBP.MyAnimBP_C"));
	if (MALE_ANIM.Succeeded())
	{
		MALE_ANIM_CLASS = MALE_ANIM.Class;
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	GetMesh()->SetAnimInstanceClass(MALE_ANIM_CLASS);
  ...
 }
 ```

### 두번째 방법 : MyCharacter Blueprint 세팅
AnimClass 를 MyAnimBP 로 세팅
![image](https://user-images.githubusercontent.com/29656900/183336199-03f9046c-cf66-4b05-b15d-22d1f55c28c5.png)

Editor에서 실행한 화면
![image](https://user-images.githubusercontent.com/29656900/183336267-17925ea9-fdb5-4908-8fc5-fe8ad6960523.png)


## 캐릭터 이동 애니메이션

MyAnimInstance.h
```
public:
	UMyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed;
```
MyAnimInstace.cpp
```
UMyAnimInstance::UMyAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		
	}
}
```
### CurrentPawSpeed 가 0.0 이상일경우 Run , 아닐경우 Idle 애니메이션이 되도록 설정
CurrentPawnSpeed 변수를 끌어다 놓고, ">" 을 검색 
![image](https://user-images.githubusercontent.com/29656900/183343460-22d530d3-3dac-42b5-846e-72d8c32ac994.png)
Bend poses by bool 검색
![image](https://user-images.githubusercontent.com/29656900/183343553-8a0655cb-ac23-4cb9-8cd2-20e72bc6ba20.png)
최종 : 
![image](https://user-images.githubusercontent.com/29656900/183343667-a5a16e2d-19a3-4a9b-81e1-5e039a6e5414.png)


이제 에디터에서 실행하면, 이동키를 누를경우 이동하면서 이동 애니메이션이 재생되고 이동하지 않을 경우 Idle 애니메이션이 재생된다. 

## State Machine
State Machine 생성하여 기존 그래프를 이동시킨다.

![image](https://user-images.githubusercontent.com/29656900/183345211-cd55c667-d887-45a2-bbd3-7cf004743e3a.png)

![image](https://user-images.githubusercontent.com/29656900/183345424-9826b78d-9bc6-4c97-8297-bbbd2068bef6.png)

State Machine 에 State를 추가한다.
![image](https://user-images.githubusercontent.com/29656900/183345288-61848244-4453-4db2-9e93-8eb6ef8aa46c.png)

Ground State를 생성한다
![image](https://user-images.githubusercontent.com/29656900/183345322-f751e5f9-2fa8-400e-893c-ff84d19f1d2d.png)

Ground State에 기존 그래프를 복사한다.
![image](https://user-images.githubusercontent.com/29656900/183345378-0a29d8ab-989a-4940-af83-ee96a5827b2b.png)


## Jump Animation
MyCharacter.h
```
private:
...
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	bool IsInAir;
```
MyCharacter.cpp
```
UMyAnimInstance::UMyAnimInstance()
{
...
	IsInAir = false;
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}
```

### State 추가
![image](https://user-images.githubusercontent.com/29656900/183347337-1ba7d6b3-2a99-4fa2-b337-b0ed6aad56a9.png)

Jump State
![image](https://user-images.githubusercontent.com/29656900/183347439-5984e9b4-7de0-4e3f-bf81-6d2fdb4327b1.png)
Fall Loop
![image](https://user-images.githubusercontent.com/29656900/183347506-b0b6a057-c065-4203-ba24-099e0b6a94fd.png)
Land
![image](https://user-images.githubusercontent.com/29656900/183347580-d4237543-7917-468b-92e1-888b68f0334e.png)

### Transition Rule
![image](https://user-images.githubusercontent.com/29656900/183347738-8951c101-25c3-40df-a2d4-0d89071d06c0.png)

Ground -> Jump
![image](https://user-images.githubusercontent.com/29656900/183349456-50a8aecf-6c34-416e-ba92-7c7bc5024e66.png)

Jump -> Fall Loop
![image](https://user-images.githubusercontent.com/29656900/183553944-0251550f-76c0-4880-8945-5a5a6598f38e.png)



Fall Loop ->Land
![image](https://user-images.githubusercontent.com/29656900/183349273-d9d965a4-d9d2-469b-8ea6-cb2c3efc36c6.png)
![image](https://user-images.githubusercontent.com/29656900/183349407-195e7a8e-fb23-4969-a929-5294b9eedfa9.png)

Land -> Ground
![image](https://user-images.githubusercontent.com/29656900/183554008-c9a5a351-76a9-4634-9575-ba43d85dfe77.png)


### Jump 구현
![image](https://user-images.githubusercontent.com/29656900/183349748-7ef5a3a8-a466-4ab9-ab21-918293cbcbbe.png)
MyCharacter.cpp
```
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
...
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);
...
}
```
