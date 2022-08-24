// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/GameData/MyCharacterData.MyCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MYCHARACTER(*CharacterDataPath);

	if (DT_MYCHARACTER.Succeeded())
	{
		MyCharacterTable = DT_MYCHARACTER.Object;
		ensure(MyCharacterTable->GetRowMap().Num() > 0);
	}
}

void UMyGameInstance::Init()
{
	Super::Init();

	//UE_LOG(LogTemp, Warning, TEXT("DropExp of Level 2 MyCharacter : %d"), GetMyCharacterData(2)->DropExp);
}

FMyCharacterData* UMyGameInstance::GetMyCharacterData(int32 Level)
{
	return MyCharacterTable->FindRow<FMyCharacterData>(* FString::FromInt(Level), TEXT(""));
}