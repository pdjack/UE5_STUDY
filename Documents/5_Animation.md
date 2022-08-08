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
AnimInstance Class 에 CurrentPawnSpeed 변수 추가

MyAnimInstance.h
```
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed;
```
