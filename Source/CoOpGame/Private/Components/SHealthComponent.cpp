// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "CoOpGame/Public/Components/SHealthComponent.h"

USHealthComponent::USHealthComponent()
{
	MaxHealth = 100;
	CurrentHealth = MaxHealth;
}

void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

}