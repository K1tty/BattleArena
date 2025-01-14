#pragma once
#include "Ball.generated.h"


UCLASS()
class ABall: public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetColor(bool Red);

	UFUNCTION(BlueprintImplementableEvent)
	void SetHealthPercent(float HealthPercent);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayMoveToEffect(const FVector& From, const FVector& To);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDamageEffect(float HealthPercent);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAttackEffect(const AActor* Target);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDeathEffect();
};