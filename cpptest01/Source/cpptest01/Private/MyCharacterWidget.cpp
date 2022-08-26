// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterWidget.h"
#include "CharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UMyCharacterWidget::BindCharacterStat(UCharacterStatComponent* NewCharacterStat)
{
	ensure(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;

	NewCharacterStat->OnHPChanged.AddUObject(this, &UMyCharacterWidget::UpdateHPWidget);
}

void UMyCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBAR")));
	ensure(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void UMyCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}