// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/DamageIndicatorWidget.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/CanvasPanel.h"

void UDamageIndicatorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RotateWidget();
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UDamageIndicatorWidget::RotateWidget, 0.1f, true);
}

void UDamageIndicatorWidget::RotateWidget()
{
	if(GetOwningPlayer())
	{
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, DamageOrigin);
		SetRenderTransformAngle(Rotation.Yaw);

		if(GetOwningPlayerCameraManager())
		{
			CP_Canvas->SetRenderTransformAngle(GetOwningPlayerCameraManager()->GetCameraRotation().Yaw * -1.f);
		}
	}
}

void UDamageIndicatorWidget::SetDamageLocation(FVector DamageLocation, FVector ReceiverLocation)
{
	DamageOrigin = DamageLocation;
	CurrentLocation = ReceiverLocation;
}

void UDamageIndicatorWidget::SelfDestruct()
{
	GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
	RemoveFromParent();
	Destruct();
}
