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
	void PlayMoveToEffect(const FVector& From, const FVector& To);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDamageEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAttackEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDeathEffect();
};