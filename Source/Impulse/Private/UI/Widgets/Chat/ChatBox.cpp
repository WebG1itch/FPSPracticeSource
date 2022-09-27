// Copyright Spitfire Labs 2022


#include "UI/Widgets/Chat/ChatBox.h"
#include "Components/Border.h"
#include "Components/EditableText.h"
#include "Components/RichTextBlock.h"
#include "Components/ScrollBox.h"
#include "PlayerController/ImpulsePlayerController.h"
#include "UI/ImpulseHUD.h"
#include "UI/Widgets/Chat/ChatMessage.h"

void UChatBox::NativeConstruct()
{
	Super::NativeOnInitialized();
	ET_ChatInputBox->OnTextCommitted.AddDynamic(this, &UChatBox::CommitMessage);
	ET_ChatInputBox->OnTextChanged.AddDynamic(this, &UChatBox::EditedText);
	
	ET_ChatInputBox->SetVisibility(ESlateVisibility::Hidden);
	BD_ChatInputBackground->SetVisibility(ESlateVisibility::Hidden);
	BD_ChatMessageBackground->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
}

void UChatBox::ChatRequest()
{
	ET_ChatInputBox->SetVisibility(ESlateVisibility::Visible);
	BD_ChatInputBackground->SetVisibility(ESlateVisibility::Visible);
	BD_ChatMessageBackground->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 1.f));

	ET_ChatInputBox->SetFocus();
	ET_ChatInputBox->SetText(SavedText);
}

void UChatBox::CommitMessage(const FText& Text, ETextCommit::Type CommitMethod)
{
	// TODO: Send message
	switch (CommitMethod)
	{
		case ETextCommit::Default: // Losing focus or similar event caused implicit commit
		{
			ET_ChatInputBox->SetVisibility(ESlateVisibility::Hidden);
			BD_ChatInputBackground->SetVisibility(ESlateVisibility::Hidden);
			BD_ChatMessageBackground->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.f));

			HUDReference->GetOwningPlayerController()->SetShowMouseCursor(false);
			HUDReference->GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
			break;
		}
		case ETextCommit::OnEnter: // User committed via the enter key
		{
			if (!Text.IsEmptyOrWhitespace())
			{
				if (OwningPlayerController)
				{
					UE_LOG(LogTemp, Warning, TEXT("client send: %d"), EChatTypes::Match == OwningPlayerController->AvailableChatMethods[ChatIndex]);
					OwningPlayerController->Server_SendChat(ET_ChatInputBox->GetText(), OwningPlayerController->AvailableChatMethods[ChatIndex]);
				}
			}
			
			ET_ChatInputBox->SetVisibility(ESlateVisibility::Hidden);
			BD_ChatInputBackground->SetVisibility(ESlateVisibility::Hidden);
			BD_ChatMessageBackground->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
			
			SavedText = FText::GetEmpty();

			HUDReference->GetOwningPlayerController()->SetShowMouseCursor(false);
			HUDReference->GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());

			break;
		}
		case ETextCommit::OnCleared: // Keyboard focus was explicitly cleared via the escape key or other similar action
		{
			ET_ChatInputBox->SetVisibility(ESlateVisibility::Hidden);
			BD_ChatInputBackground->SetVisibility(ESlateVisibility::Hidden);
			BD_ChatMessageBackground->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.f));

			HUDReference->GetOwningPlayerController()->SetShowMouseCursor(false);
			HUDReference->GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
			break;
		}
		default: ET_ChatInputBox->SetFocus();
	}
}

void UChatBox::EditedText(const FText& Text)
{
	SavedText = Text;
}

void UChatBox::SetHUDReference(AImpulseHUD* HUDRef)
{
	HUDReference = HUDRef;
	OwningPlayerController = Cast<AImpulsePlayerController>(HUDReference->GetOwningPlayerController());
	TypeOfChat = OwningPlayerController->AvailableChatMethods[ChatIndex];
	SetChatDestinationText();

}

void UChatBox::ReceiveChat(const FString& Name, const FText& Message, EChatTypes ChatType)
{

	TArray<UWidget*> ChatMessages = SB_ChatMessages->GetAllChildren();
	
	UChatMessage* ChatMessage = CreateWidget<UChatMessage>(GetOwningPlayer(), ChatMessageClass, FName(FString::FromInt(SB_ChatMessages->GetChildrenCount())));
	ChatMessage->AddToViewport();
	ChatMessage->SetVisibility(ESlateVisibility::Hidden);
	ChatMessage->SetMessage(Name, Message, ChatType);
	ChatMessage->SetVisibility(ESlateVisibility::Visible);

	ChatMessages.Insert(ChatMessage, 0);
	HandleChatOverflow(ChatMessages);
	SB_ChatMessages->ClearChildren();

	for (UWidget* PreviousMessages : ChatMessages) SB_ChatMessages->AddChild(PreviousMessages);
}

FReply UChatBox::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Tab)
	{
		ChatIndex += 1;
		if (ChatIndex > OwningPlayerController->AvailableChatMethods.Num() - 1) ChatIndex = 0;
		TypeOfChat = OwningPlayerController->AvailableChatMethods[ChatIndex];
		SetChatDestinationText();
	}
	
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UChatBox::SetChatDestinationText()
{
	switch (TypeOfChat)
	{
	case Global: RTB_Destination->SetText(FText::FromString("<GlobalName>Global:</>"));
		break;
	case Match: RTB_Destination->SetText(FText::FromString("<TeamName>All:</>"));
		break;
	case Team: RTB_Destination->SetText(FText::FromString("<TeamName>Team:</>"));
		break;
	case Party: RTB_Destination->SetText(FText::FromString("<PartyName>Party:</>"));
		break;
	case Private: RTB_Destination->SetText(FText::FromString("<PrivateName>To:</>"));
		break;
	case System: RTB_Destination->SetText(FText::FromString("<GlobalName>Invalid Chat Type.</>"));
		break;
	case Enemy: RTB_Destination->SetText(FText::FromString("<GlobalName>Invalid Chat Type.</>"));
		break;
	default: RTB_Destination->SetText(FText::FromString("<GlobalName>Invalid Chat Type.</>"));
	}
}

void UChatBox::HandleChatOverflow(TArray<UWidget*>& ChatMessages)
{
	if (ChatMessages.Num() > 100)
	{
		ChatMessages.RemoveAt(100);
	}
}
