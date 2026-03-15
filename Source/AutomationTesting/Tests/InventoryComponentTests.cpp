// My game copyright

#if WITH_AUTOMATION_TESTS

#include "AutomationTesting/Tests/InventoryComponentTests.h"
#include "CoreMinimal.h"
#include "TestUtils.h"
#include "Components/InventoryComponent.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentCantBeCreated, "TPSGame.Components.Inventory.ComponentCantBeCreated",
                                 EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags_ApplicationContextMask);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FItemScoresShouldBeZerosByDefault,
                                 "TPSGame.Components.Inventory.ItemScoresShouldBeZerosByDefault",
                                 EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags_ApplicationContextMask);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNegativeScoreShouldntBeAdded,
                                 "TPSGame.Components.Inventory.NegativeScoreShouldntBeAdded",
                                 EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags_ApplicationContextMask);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPositiveScoreShouldBeAdded, "TPSGame.Components.Inventory.PositiveScoreShouldBeAdded",
                                 EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags_ApplicationContextMask);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScoreMoreThanLimitCantBeAdded,
                                 "TPSGame.Components.Inventory.ScoreMoreThanLimitCantBeAdded",
                                 EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags_ApplicationContextMask);


namespace Test
{
	class UInventoryComponentTestable : public UInventoryComponent
	{
	public:
		void SetLimits(const TMap<EInventoryItemType, int32>& Limits) { InventoryLimits = Limits; }
	};

	TMap<EInventoryItemType, int32> InitLimits(UInventoryComponentTestable* InventoryComponent, int32 LimitValue)
	{
		TMap<EInventoryItemType, int32> InvLimits;
		ENUM_LOOP_START(EInventoryItemType, EElem)
			InvLimits.Add(EElem, LimitValue);
		ENUM_LOOP_END
		InventoryComponent->SetLimits(InvLimits);
		return InvLimits;
	}
}


bool FComponentCantBeCreated::RunTest(const FString& Parameters)
{
	const UInventoryComponent* InventoryComponent = NewObject<UInventoryComponent>();
	if (!TestNotNull("Inventory component exists", InventoryComponent)) return false;

	return true;
}


bool FItemScoresShouldBeZerosByDefault::RunTest(const FString& Parameters)
{
	const UInventoryComponent* InventoryComponent = NewObject<UInventoryComponent>();
	if (!TestNotNull("Inventory component exists", InventoryComponent)) return false;

	ENUM_LOOP_START(EInventoryItemType, Element)
		TestTrueExpr(InventoryComponent->GetInventoryAmountByType(Element) == 0);
	ENUM_LOOP_END

	return true;
}

bool FNegativeScoreShouldntBeAdded::RunTest(const FString& Parameters)
{
	Test::UInventoryComponentTestable* InventoryComponent = NewObject<Test::UInventoryComponentTestable>();
	if (!TestNotNull("Inventory component exists", InventoryComponent)) return false;

	InitLimits(InventoryComponent, 100);

	constexpr int32 InitialScoreAmount = 10;
	constexpr int32 NegativeScoreAmount = -5;

	ENUM_LOOP_START(EInventoryItemType, Element)
		TestTrueExpr(InventoryComponent->TryToAddItem({Element, InitialScoreAmount}));
		TestTrueExpr(InventoryComponent->GetInventoryAmountByType(Element) == InitialScoreAmount);
		TestTrueExpr(!InventoryComponent->TryToAddItem({Element, NegativeScoreAmount}));
		TestTrueExpr(InventoryComponent->GetInventoryAmountByType(Element) == InitialScoreAmount);
	ENUM_LOOP_END

	return true;
}

bool FPositiveScoreShouldBeAdded::RunTest(const FString& Parameters)
{
	Test::UInventoryComponentTestable* InventoryComponent = NewObject<Test::UInventoryComponentTestable>();
	if (!TestNotNull("Inventory component exists", InventoryComponent)) return false;

	const auto Limits = InitLimits(InventoryComponent, 100);

	ENUM_LOOP_START(EInventoryItemType, Element)
		for (int32 i = 0; i < Limits[Element]; ++i)
		{
			TestTrueExpr(InventoryComponent->TryToAddItem({Element, 1}));
			TestTrueExpr(InventoryComponent->GetInventoryAmountByType(Element) == i + 1);
		}
	ENUM_LOOP_END

	return true;
}

bool FScoreMoreThanLimitCantBeAdded::RunTest(const FString& Parameters)
{
	Test::UInventoryComponentTestable* InventoryComponent = NewObject<Test::UInventoryComponentTestable>();
	if (!TestNotNull("Inventory component exists", InventoryComponent)) return false;

	constexpr int32 ScoreLimit = 100;
	InitLimits(InventoryComponent, ScoreLimit);


	TestTrueExpr(InventoryComponent->TryToAddItem({EInventoryItemType::Sphere, 10}));
	TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EInventoryItemType::Sphere) == 10);

	TestTrueExpr(!InventoryComponent->TryToAddItem({EInventoryItemType::Sphere, ScoreLimit + 1}));
	TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EInventoryItemType::Sphere) == 10);


	return true;
}

#endif
