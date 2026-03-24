// My game copyright

#if WITH_AUTOMATION_TESTS

#include "AutomationTesting/Tests/InventoryItemTests.h"
#include "CoreMinimal.h"
#include "AutomationTestingCharacter.h"
#include "TestUtils.h"
#include "Components/InventoryComponent.h"
#include "Items/InventoryItem.h"
#include "Misc/AutomationTest.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCanBeCreated, "TPSGame.Items.Inventory.CppActorCanBeCreated",
                                 EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags_ApplicationContextMask);


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly,
                                 "TPSGame.Items.Inventory.BlueprintShouldBeSetupCorrectly",
                                 EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags_ApplicationContextMask);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryDataShouldBeSetupCorrectly,
                                 "TPSGame.Items.Inventory.InventoryDataShouldBeSetupCorrectly",
                                 EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags_ApplicationContextMask);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryCanBeTaken, "TPSGame.Items.Inventory.InventoryCanBeTaken",
                                 EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags_ApplicationContextMask);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMeshForEveryInventoryItemShouldExist,
                                 "TPSGame.Items.Inventory.MeshForEveryInventoryItemShouldExist",
                                 EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags_ApplicationContextMask);


using namespace TestGame::Test;

namespace
{
	const FString InventoryItemBPName = FString("Blueprint'/Game/Tests/BP_TestInventoryItem.BP_TestInventoryItem'");
}


bool FCppActorCanBeCreated::RunTest(const FString& Parameters)
{
	const FString ExpectedWarnMsg = FString::Printf(
		TEXT("SpawnActor failed because class %s is abstract"), *AInventoryItem::StaticClass()->GetName());
	AddExpectedError(ExpectedWarnMsg, EAutomationExpectedErrorFlags::Exact);

	LevelScope("/Game/Tests/EmptyTestLevel");
	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World exists", World)) return false;

	const FTransform InitialTransform{FVector{1000.0f}};
	const AInventoryItem* InvItem = World->SpawnActor<AInventoryItem>(AInventoryItem::StaticClass(), InitialTransform);
	if (!TestNull("Inventory item exists", InvItem)) return false;

	return true;
}

bool FBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
	LevelScope("/Game/Tests/EmptyTestLevel");

	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World exists", World)) return false;

	const FTransform InitialTransform{FVector{1000.0f}};
	const AInventoryItem* InvItem = CreateBlueprint<AInventoryItem>(World, InventoryItemBPName, InitialTransform);
	if (!TestNotNull("Inventory item exists", InvItem)) return false;

	const auto CollisionComp = InvItem->FindComponentByClass<USphereComponent>();
	if (!TestNotNull("Sphere component exists", CollisionComp)) return false;

	TestTrueExpr(CollisionComp->GetUnscaledSphereRadius() >= 30.0f);
	TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
	TestTrueExpr(
		CollisionComp->GetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic) == ECollisionResponse::
		ECR_Overlap);
	TestTrueExpr(CollisionComp->GetGenerateOverlapEvents());
	TestTrueExpr(InvItem->GetRootComponent() == CollisionComp);

	ENUM_LOOP_START(ECollisionChannel, EElem)
		if (EElem != ECC_OverlapAll_Deprecated)
		{
			TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(EElem) == ECollisionResponse::ECR_Overlap);
		}
	ENUM_LOOP_END

	const auto TextRenderComp = InvItem->FindComponentByClass<UTextRenderComponent>();
	if (!TestNotNull("Text render component exists", TextRenderComp)) return false;

	const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
	if (!TestNotNull("Static mesh component exists", StaticMeshComp)) return false;

	TestTrueExpr(StaticMeshComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision);

	return true;
}

bool FInventoryDataShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
	LevelScope("/Game/Tests/EmptyTestLevel");

	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World exists", World)) return false;

	const FTransform InitialTransform{FVector{1000.0f}};
	AInventoryItem* InvItem = CreateBlueprint<AInventoryItem>(World, InventoryItemBPName, InitialTransform);
	if (!TestNotNull("Inventory item exists", InvItem)) return false;

	constexpr FInventoryData InvData{EInventoryItemType::Cylinder, 13};
	const FLinearColor Color = FLinearColor::Yellow;
	CallFuncByNameWithParams(InvItem, "SetInventoryData", {InvData.ToString(), Color.ToString()});

	const auto TextRenderComp = InvItem->FindComponentByClass<UTextRenderComponent>();
	if (!TestNotNull("Text render component exists", TextRenderComp)) return false;

	TestTrueExpr(TextRenderComp->Text.ToString().Equals(FString::FromInt(InvData.Score)));
	TestTrueExpr(TextRenderComp->TextRenderColor == Color.ToFColor(true));

	const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
	if (!TestNotNull("Static mesh component exists", StaticMeshComp)) return false;

	const auto Material = StaticMeshComp->GetMaterial(0);
	if (!TestNotNull("Material exists", Material)) return false;

	FLinearColor MaterialColor;
	Material->GetVectorParameterValue(FHashedMaterialParameterInfo{"Color"}, MaterialColor);
	TestTrueExpr(MaterialColor == Color);

	return true;
}

bool FInventoryCanBeTaken::RunTest(const FString& Parameters)
{
	LevelScope("/Game/Tests/EmptyTestLevel");

	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World exists", World)) return false;

	const FTransform InitialTransform{FVector{1000.0f}};
	AInventoryItem* InvItem = CreateBlueprint<AInventoryItem>(World, InventoryItemBPName, InitialTransform);
	if (!TestNotNull("Inventory item exists", InvItem)) return false;

	constexpr FInventoryData InvData{EInventoryItemType::Cylinder, 13};
	const FLinearColor Color = FLinearColor::Yellow;
	CallFuncByNameWithParams(InvItem, "SetInventoryData", {InvData.ToString(), Color.ToString()});

	TArray<AActor*> Pawns;
	UGameplayStatics::GetAllActorsOfClass(World, AAutomationTestingCharacter::StaticClass(), Pawns);
	if (!TestTrueExpr(Pawns.Num() == 1)) return false;

	const auto Character = Cast<AAutomationTestingCharacter>(Pawns[0]);
	if (!TestNotNull("TPSCharacter exists", Character)) return false;

	const auto InvComp = Character->FindComponentByClass<UInventoryComponent>();
	if (!TestNotNull("InvComp exists", InvComp)) return false;
	TestTrueExpr(InvComp->GetInventoryAmountByType(InvData.Type) == 0);

	Character->SetActorLocation(InitialTransform.GetLocation());

	TestTrueExpr(InvComp->GetInventoryAmountByType(InvData.Type) == InvData.Score);
	TestTrueExpr(!IsValid(InvItem));

	TArray<AActor*> InvItems;
	UGameplayStatics::GetAllActorsOfClass(World, AInventoryItem::StaticClass(), InvItems);
	TestTrueExpr(InvItems.Num() == 0);

	return true;
}


bool FMeshForEveryInventoryItemShouldExist::RunTest(const FString& Parameters)
{
	LevelScope("/Game/Tests/EmptyTestLevel");

	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World exists", World)) return false;

	ENUM_LOOP_START(EInventoryItemType, Element)

		const FTransform InitialTransform{FVector{1000.0f}};
		AInventoryItem* InvItem = CreateBlueprint<AInventoryItem>(World, InventoryItemBPName, InitialTransform);
		if (!TestNotNull("Inventory item exists", InvItem)) return false;

		constexpr FInventoryData InvData{EInventoryItemType::Cylinder, 13};
		const FLinearColor Color = FLinearColor::Yellow;
		CallFuncByNameWithParams(InvItem, "SetInventoryData", {InvData.ToString(), Color.ToString()});

		const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
		if (!TestNotNull("Static mesh component exists", StaticMeshComp)) return false;

		const FString MeshMessage = FString::Printf(TEXT("Static mesh for %s exists"), *UEnum::GetValueAsString(Element));
		TestNotNull(*MeshMessage, StaticMeshComp->GetStaticMesh().Get());

	ENUM_LOOP_END

	return true;
}


#endif
