#if WITH_AUTOMATION_TESTS

#include "ForDevOps/Tests/TestUtils.h"

namespace ForDevOps
{
namespace Test
{
UWorld* GetTestGameWorld()
{

    const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
    for (const FWorldContext& Context : WorldContexts)
    {
        if (((Context.WorldType == EWorldType::PIE) || Context.WorldType == EWorldType::Game) && Context.World())
        {
            return Context.World();
        }
    }

    return nullptr;
};

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
{
    if (!Object) return;

    FString Command = FString::Printf(TEXT("%s"), *FuncName);
    for (const auto Param : Params)
    {
        Command.Append(" ").Append(Param);
    }

    FOutputDeviceNull OutputDeviceNull;
    Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);

    // FString::Printf(TEXT("X=%3.3f Y=%3.3f Z=%3.3f"), X, Y, Z); toString
    //"SetTestData 123 345.34 \"My cool string\" Yes ";
    // first args name function than variables separator space
}

}  // namespace Test
}  // namespace ForDevOps

#endif
