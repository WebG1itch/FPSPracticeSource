// Copyright Spitfire Labs 2022

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/EChatTypes.h"
#include "ChatMessage.generated.h"

class URichTextBlock;

UCLASS()
class IMPULSE_API UChatMessage : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* TB_MessageBox;
	
	virtual void NativeConstruct() override;

public:
	void SetMessage(const FString& Name, const FText& Message, EChatTypes TypeOfChat);
};
