// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "CoOpGame/Public/Components/SHealthComponent.h"
#include "SGameMode.h"
#include "Net/UnrealNetwork.h"

USHealthComponent::USHealthComponent()
{
	MaxHealth = 100;
	bIsDead = false;

	SetIsReplicatedByDefault(true);
}

float USHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

void USHealthComponent::SetMaxHealth(float Health)
{
	if (Health > 0)
	{
		MaxHealth = Health;
	}
}

void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
		}
	}

	CurrentHealth = MaxHealth;
}

void USHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0 || bIsDead)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	bIsDead = CurrentHealth <= 0.0f;

	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead)
	{
		ASGameMode* GM = GetWorld()->GetAuthGameMode<ASGameMode>();
		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}
}

void USHealthComponent::OnRep_CurrentHealth(float PreviousHealth)
{
	float DamageApplied = CurrentHealth - PreviousHealth;

	OnHealthChanged.Broadcast(this, CurrentHealth, DamageApplied, nullptr, nullptr, nullptr);
}

void USHealthComponent::Heal(float AmouthOfHealth)
{
	if (AmouthOfHealth <= 0 || CurrentHealth <= 0)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth + AmouthOfHealth, 0.0f, MaxHealth);

	OnHealthChanged.Broadcast(this, CurrentHealth, -AmouthOfHealth, nullptr, nullptr, nullptr);
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USHealthComponent, CurrentHealth);
}
