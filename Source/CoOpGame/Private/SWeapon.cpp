// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SWeapon.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

ASWeapon::ASWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SceneComp);
}

void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

