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
};
