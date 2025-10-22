// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/FireGameInstance.h"

UFireGameInstance::UFireGameInstance()
{

}

void UFireGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Log, TEXT("[FireGameInstance] Initialized. This instance will persist across map transitions."));
}

void UFireGameInstance::SetPlayerRole(APlayerController* PlayerController, EPlayerRole Role)
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("[FireGameInstance] SetPlayerRole: Invalid PlayerController"));
		return;
	}

	// PlayerState의 PlayerName을 Key로 사용 (맵 전환 시에도 유지됨)
	APlayerState* PS = PlayerController->GetPlayerState<APlayerState>();
	if (!PS)
	{
		UE_LOG(LogTemp, Error, TEXT("[FireGameInstance] SetPlayerRole: PlayerState not found!"));
		return;
	}

	FString PlayerKey = PS->GetPlayerName();

	// PlayerName이 비어있으면 PlayerId 사용 (Fallback)
	if (PlayerKey.IsEmpty())
	{
		PlayerKey = FString::Printf(TEXT("Player_%d"), PS->GetPlayerId());
		UE_LOG(LogTemp, Warning, TEXT("[FireGameInstance] PlayerName empty, using PlayerId: %s"), *PlayerKey);
	}

	PlayerRoleMap.Add(PlayerKey, Role);

	UE_LOG(LogTemp, Warning, TEXT("[FireGameInstance] Player %s (Key: %s) selected role: %s"),
		*PlayerController->GetName(),
		*PlayerKey,
		*UEnum::GetValueAsString(Role));
}

EPlayerRole UFireGameInstance::GetPlayerRole(APlayerController* PlayerController) const
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("[FireGameInstance] GetPlayerRole: Invalid PlayerController"));
		return EPlayerRole::None;
	}

	// PlayerState의 PlayerName을 Key로 사용 (맵 전환 시에도 유지됨)
	APlayerState* PS = PlayerController->GetPlayerState<APlayerState>();
	if (!PS)
	{
		UE_LOG(LogTemp, Error, TEXT("[FireGameInstance] GetPlayerRole: PlayerState not found!"));
		return EPlayerRole::None;
	}

	FString PlayerKey = PS->GetPlayerName();

	// PlayerName이 비어있으면 PlayerId 사용 (Fallback)
	if (PlayerKey.IsEmpty())
	{
		PlayerKey = FString::Printf(TEXT("Player_%d"), PS->GetPlayerId());
		UE_LOG(LogTemp, Warning, TEXT("[FireGameInstance] PlayerName empty, using PlayerId: %s"), *PlayerKey);
	}

	// TMap에서 Role 조회
	const EPlayerRole* FoundRole = PlayerRoleMap.Find(PlayerKey);

	if (FoundRole)
	{
		UE_LOG(LogTemp, Warning, TEXT("[FireGameInstance] Found role for %s (Key: %s): %s"),
			*PlayerController->GetName(),
			*PlayerKey,
			*UEnum::GetValueAsString(*FoundRole));
		return *FoundRole;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[FireGameInstance] No role found for %s (Key: %s), returning None"),
			*PlayerController->GetName(),
			*PlayerKey);
		return EPlayerRole::None;
	}
}

void UFireGameInstance::DebugPrintPlayerRoles() const
{
	UE_LOG(LogTemp, Warning, TEXT("=== [FireGameInstance] Current Player Roles ==="));

	if (PlayerRoleMap.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("  (No players registered)"));
		return;
	}

	for (const auto& Entry : PlayerRoleMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("  Key: %s -> Role: %s"),
			*Entry.Key,
			*UEnum::GetValueAsString(Entry.Value));
	}

	UE_LOG(LogTemp, Warning, TEXT("==========================================="));
}
