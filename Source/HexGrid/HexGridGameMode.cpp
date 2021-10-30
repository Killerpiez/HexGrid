// Copyright Epic Games, Inc. All Rights Reserved.

#include "HexGridGameMode.h"
#include "HexGridPlayerController.h"
#include "HexGridCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHexGridGameMode::AHexGridGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AHexGridPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}