// Copyright Spitfire Labs 2022

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/EChatTypes.h"
#include "Kismet/GameplayStatics.h"
#include "ChatBox.generated.h"

class UScrollBox;
class UEditableText;
class UBorder;
class AImpulseHUD;
class AImpulsePlayerController;
class URichTextBlock;

UCLASS()
class IMPULSE_API UChatBox : public UUserWidget
{
	GENERATED_BODY()
	UChatBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	UPROPERTY()
	AImpulseHUD* HUDReference;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* SB_ChatMessages;

	UPROPERTY(meta = (BindWidget))
	UEditableText* ET_ChatInputBox;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* RTB_Destination;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* BD_ChatMessageBackground;

	UPROPERTY(meta = (BindWidget))
	UBorder* BD_ChatInputBackground;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ChatMessageClass;
	
	FText SavedText;
	
	void HandleChatOverflow(TArray<UWidget*>& ChatMessages);

	UFUNCTION()
	void CommitMessage(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void EditedText(const FText& Text);

	EChatTypes TypeOfChat;
	uint8 ChatIndex = 0;

	UPROPERTY()
	AImpulsePlayerController* OwningPlayerController;
public:

	virtual void NativeConstruct() override;

	void ChatRequest();

	void SetHUDReference(AImpulseHUD* HUDRef);

	void ReceiveChat(const FString& Name, const FText& Message, EChatTypes ChatType);

	void SetChatDestinationText();

	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
