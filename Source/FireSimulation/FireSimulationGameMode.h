// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FireSimulationGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AFireSimulationGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	AFireSimulationGameMode();
};



