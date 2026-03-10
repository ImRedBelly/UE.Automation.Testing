#include "Battery.h"

//using namespace TPS;

constexpr float ChargeAmount = 0.1f;

Battery::Battery(float InPercent)
{
	SetPercent(InPercent);
}

void Battery::Charge()
{
	SetPercent(GetPercent() + ChargeAmount);
}

void Battery::UnCharge()
{
	SetPercent(GetPercent() - ChargeAmount);
}

float Battery::GetPercent() const
{
	return Percent;
}

FColor Battery::GetColor() const
{
	if (GetPercent() > 0.8f) return FColor::Green;
	if (GetPercent() > 0.3f) return FColor::Yellow;
	return FColor::Red;
}

FString Battery::ToString() const
{
	return FString::Printf(TEXT("%i%%"), FMath::RoundToInt(GetPercent() * 100.0f));
}

void Battery::SetPercent(float InPercent)
{
	Percent = FMath::Clamp(InPercent, 0.0f, 1.0f);
}
