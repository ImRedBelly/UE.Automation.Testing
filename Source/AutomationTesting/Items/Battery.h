#pragma once


#include "CoreMinimal.h"

class AUTOMATIONTESTING_API Battery
{
public:
	Battery() = default;
	Battery(float InPercent);

	void Charge();
	void UnCharge();

	float GetPercent() const;
	FColor GetColor() const;
	FString ToString() const;

	bool operator >=(const Battery& rhs) const { return GetPercent() >= rhs.GetPercent(); }
	bool operator <(const Battery& rhs) const { return GetPercent() < rhs.GetPercent(); }
	bool operator ==(const Battery& rhs) const { return FMath::IsNearlyEqual(GetPercent(), rhs.GetPercent()); }

private:
	float Percent = 1.0f;
	void SetPercent(float InPercent);
};
