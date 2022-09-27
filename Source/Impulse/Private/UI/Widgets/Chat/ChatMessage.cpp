// Copyright Spitfire Labs 2022


#include "UI/Widgets/Chat/ChatMessage.h"

#include "Components/RichTextBlock.h"

void UChatMessage::NativeConstruct()
{
	Super::NativeConstruct();
}

void UChatMessage::SetMessage(const FString& Name, const FText& Message, EChatTypes TypeOfChat)
{
	switch (TypeOfChat)
	{
	case Global: TB_MessageBox->SetText(FText::FromString("<GlobalName>" + Name + ": </><Message>" + Message.ToString() + "</>"));
		break;
	case Team: TB_MessageBox->SetText(FText::FromString("<TeamName>" + Name + ": </><Message>" + Message.ToString() + "</>"));
		break;
	case Party: TB_MessageBox->SetText(FText::FromString("<PartyName>" + Name + ": </><Message>" + Message.ToString() + "</>"));
		break;
	case Private: TB_MessageBox->SetText(FText::FromString("<PrivateName>" + Name + ": </><Message>" + Message.ToString() + "</>"));
		break;
	case System: TB_MessageBox->SetText(FText::FromString("<SystemName>" + Name + ": </><SystemMessage>" + Message.ToString() + "</>"));
		break;
	case Enemy: TB_MessageBox->SetText(FText::FromString("<EnemyName>" + Name + ": </><Message>" + Message.ToString() + "</>"));
		break;
	default: TB_MessageBox->SetText(FText::FromString("<GlobalName>" + Name + ": </><Message>" + Message.ToString() + "</>"));
	}
}
