#if WITH_AUTOMATION_TESTS
#include "Tests/TestUtils.h"
#include "Misc/OutputDeviceNull.h"

namespace TestGame::Test
{
	UWorld* GetTestGameWorld()
	{
		const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
		for (const FWorldContext& Context : WorldContexts)
		{
			if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game) && Context.World())
			{
				return Context.World();
			}
		}

		return nullptr;
	}


	void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
	{
		if (!Object) return;
		FString Command = FuncName;
		for (auto Param : Params)
		{
			Command.Append(" ").Append(Param);
		}

		FOutputDeviceNull OutputDeviceNull;
		Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
	}
}

#endif
