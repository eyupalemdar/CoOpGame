// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "CoOpGame/Public/AI/STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "NavigationSystem/Public/NavigationSystem.h"

ASTrackerBot::ASTrackerBot()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	RootComponent = MeshComp;
}

void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASTrackerBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FVector ASTrackerBot::GetNextPathPoint()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerCharacter);

	if (NavigationPath->PathPoints.Num() > 1)
	{
		return NavigationPath->PathPoints[1]; // First one is the ActorLocation (TrackerBot) so we need to get Next one
	}

	// That means failed to find path
	return GetActorLocation();
}
