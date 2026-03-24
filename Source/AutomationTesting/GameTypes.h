#pragma once

#include "CoreMinimal.h"
#include "GameTypes.generated.h"

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{
	Sphere = 0 UMETA(DisplayName = "Sphere"),
	Cube = 1 UMETA(DisplayName = "Cube"),
	Cylinder = 2 UMETA(DisplayName = "Cylinder"),
	Cone = 3 UMETA(DisplayName = "Cone"),
};

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	EInventoryItemType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta=(ClampMin = 0))
	int32 Score;

	FString ToString() const { return FString::Printf(TEXT("(Type=%i,Score=%i)"), Type, Score); }
};

USTRUCT(BlueprintType)
struct FHealthData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1.0"))
	float MaxHealth{100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1"))
	float HealModifier{10.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", Units = "s"))
	float HealRate{0.5f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", Units = "s"))
	float LifeSpan{5.0f};
};
