// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ObjectMacros.h"
#include "FiremanMainUI.generated.h"

UENUM()
enum class ESubMissions : uint8
{
	FireOff = 0, MaskOut, Evacuation, ForceOpenDoor
};

UCLASS()
class FIREPJT_FIRST_API UFiremanMainUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFiremanMainUI(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	
public:
	// INFO UI
	void AddInfoUI(int32 idx);

	// Mission Ui
	UFUNCTION()
	void ShowSubMissionUI(ESubMissions subMission);
	UFUNCTION()
	void SuccessSubMission();
	int32 GetCurSubMissionNum() { return CurrentSubMission; }
	int32 GetMaxSubMissionNum() { return MaxSubMission; }

	// Text Alert
	UFUNCTION()
	void ShowAlert();
	
protected:
	// Info UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInfoWidget> InfoWidgetClass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class USizeBox> SizeBox_Info;

	// Info UI DataAsset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UInfoDataAsset> DAFireman;

	// SubMission UI
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UOverlay> OverlaySubMission;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTextBlock> TextSubMission;

	UPROPERTY(EditAnywhere, meta=(MultiLine="true"))
	TObjectPtr<class UTextBlock> TextContent;

	UPROPERTY(EditAnywhere, meta=(MultiLine="true"))
	TObjectPtr<class UTextBlock> TextAlert;

	// SubMission Variable
	int32 CurrentSubMission = 0;
	int32 MaxSubMission = 4;
	
	// fire off submission
	UPROPERTY()
	TSubclassOf<class AFireActor> FireActorClass;
	int32 FireMaxNum;
	int32 FireCurrentNum;
	UFUNCTION()
	void ShowFireOffSubMission();
	UFUNCTION()
	void UpdateFireCurrentNum(AActor* DestroyedActor);

	// mask out submission
	UFUNCTION()
	void ShowMaskOutSubMission();

	// evacuation submission
	UFUNCTION()
	void ShowEvacuationSubMission();

	// force open door submission
	UFUNCTION()
	void ShowForceOpenDoorSubMission();
};
