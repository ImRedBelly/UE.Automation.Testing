#if WITH_AUTOMATION_TESTS

#include "SandboxTests.h"
#include "CoreMinimal.h"
#include "TestUtils.h"
#include "Misc/AutomationTest.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "TPSGame.Math.MaxInt",
                                 EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags_ApplicationContextMask);


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, "TPSGame.Math.Sqrt",
                                 EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags::HighPriority |
                                 EAutomationTestFlags_ApplicationContextMask);

bool FMathMaxInt::RunTest(const FString& Parameters)
{
	AddInfo("Max [int] func testing");

	typedef TArray<TestGame::TestPayload<TInterval<int32>, int32>> MaxIntervalPayload;
	const MaxIntervalPayload TestData
	{
		{{13, 25}, 25},
		{{25, 25}, 25},
		{{0, 123}, 123},
		{{0, 0}, 0},
		{{-2345, 0}, 0},
		{{-45, -69}, -45},
		{{-9, -9}, -9},
		{{-78, 34}, 34},
	};

	for (const auto Data : TestData)
	{
		TestTrueExpr(FMath::Max(Data.TestValue.Min, Data.TestValue.Max) == Data.ExpectedValue);
	}

	TestTrue("2 different positive numbers", FMath::Max(13, 25) == 25);
	TestEqual("2 equal positive numbers", FMath::Max(25, 25), 25);
	TestTrueExpr(FMath::Max(0, 123) == 123);
	TestTrue("2 zeroes", FMath::Max(0, 0) == 0);
	TestTrue("Negative number and zero", FMath::Max(-2345, 0) == 0);
	TestTrue("2 different negative numbers", FMath::Max(-45, -67) == -45);
	TestTrue("2 equal negative numbers", FMath::Max(-9, -9) == -9);
	TestTrue("Positive vs. negative number", FMath::Max(-78, 34) == 34);

	return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
	// sqrt(3) = 1.73205...

	AddInfo("Sqrt func testing");

	typedef TArray<TestGame::TestPayload<float, float>> SqrtPayload;
	const SqrtPayload TestData
	{
		{4.0f, 2.0f},
		{3.0f, 1.7f, 0.1f},
		{3.0f, 1.73f, 0.01f},
		{3.0f, 1.73205f, 1.e-5f},
	};
	for (auto Data : TestData)
	{
		const bool IsEqual = FMath::IsNearlyEqual(FMath::Sqrt(Data.TestValue), Data.ExpectedValue, Data.Tolerance);
		TestTrueExpr(IsEqual);
	}

	TestEqual("Sqrt(4) [0]", FMath::Sqrt(4.0f), 2.0f);
	// TestEqual("Sqrt(3) [1]", FMath::Sqrt(3.0f), 1.7f);
	TestEqual("Sqrt(3) [2]", FMath::Sqrt(3.0f), 1.7f, 0.1f);
	TestEqual("Sqrt(3) [3]", FMath::Sqrt(3.0f), 1.73f, 0.01f);
	TestEqual("Sqrt(3) [4]", FMath::Sqrt(3.0f), 1.73205f, 1.e-5f);

	return true;
}

#endif
