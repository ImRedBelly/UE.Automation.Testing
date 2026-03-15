#if WITH_AUTOMATION_TESTS

#include "ScienceFunctionLibraryTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ScienceFunctionLibraryTests.h"

#include "TestUtils.h"
#include "Science/ScienceFunctionLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciSimple, "TPSGame.Science.Fibonacci.Simple",
                                 EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags_ApplicationContextMask);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciStress, "TPSGame.Science.Fibonacci.Stress",
                                 EAutomationTestFlags::StressFilter |
                                 EAutomationTestFlags_ApplicationContextMask);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciLogHasError, "TPSGame.Science.Fibonacci.LogHasError",
                                 EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags_ApplicationContextMask);

bool FFibonacciSimple::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci simple testing");

	// 0 1 1 2 3 5 8 ...

	TestTrueExpr(UScienceFunctionLibrary::Fibonacci(0) == 0);
	TestTrueExpr(UScienceFunctionLibrary::Fibonacci(1) == 1);
	TestTrueExpr(UScienceFunctionLibrary::Fibonacci(2) == 1);
	TestTrueExpr(UScienceFunctionLibrary::Fibonacci(3) == 2);
	TestTrueExpr(UScienceFunctionLibrary::Fibonacci(4) == 3);

	const TArray<TestGame::TestPayload<int32, int32>> TestData
	{
		{0, 0},
		{1, 1},
		{2, 1},
		{3, 2},
		{4, 3},
		{5, 5},
		{6, 8},
	};

	for (const auto Data : TestData)
	{
		const FString InfoString = FString::Printf(TEXT("Fibonacci(%i) != %i"), Data.TestValue, Data.ExpectedValue);
		TestEqual(InfoString, UScienceFunctionLibrary::Fibonacci(Data.TestValue), Data.ExpectedValue);
	}

	return true;
}

bool FFibonacciStress::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci stress testing");

	int32 PrevPrevValue = 0;
	int32 PrevValue = 1;
	for (int32 i = 2; i < 40; ++i)
	{
		const int32 CurrentValue = UScienceFunctionLibrary::Fibonacci(i);
		TestTrueExpr(CurrentValue == PrevPrevValue + PrevValue);

		PrevPrevValue = PrevValue;
		PrevValue = CurrentValue;
	}


	return true;
}

bool FFibonacciLogHasError::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci negative number on input produces error");

	AddExpectedError("Invalid input for Fibonacci", EAutomationExpectedErrorFlags::MatchType::Contains, 1);
	UScienceFunctionLibrary::Fibonacci(-10);

	return true;
}

#endif
