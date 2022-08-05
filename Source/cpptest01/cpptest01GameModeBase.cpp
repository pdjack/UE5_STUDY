// Copyright Epic Games, Inc. All Rights Reserved.


#include "cpptest01GameModeBase.h"

Acpptest01GameModeBase::Acpptest01GameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_MyCharacter.BP_MyCharacter_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}