#pragma once

#if WITH_AUTOMATION_TESTS
//#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "Tests/AutomationCommon.h"
#include "Misc/OutputDeviceNull.h"

namespace ForDevOps
{
namespace Test
{

template <typename Type1, typename Type2>
struct TestPayload
{
    Type1 TestValue;
    Type2 ExpectedValue;
};

#define ENUM_LOOP_START(TYPE, EnumElem)                                        \
    for (int32 index = 0; index < StaticEnum<TYPE>()->NumEnums() - 1; ++index) \
    {                                                                          \
        const auto EnumElem = static_cast<TYPE>(index);
#define ENUM_LOOP_END }

template <typename T>
T* CreateBlueprint(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
    // load blueprint
    const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);

    return (World && Blueprint) ? World->SpawnActor<T>(Blueprint->GeneratedClass, Transform)
                                : nullptr;  // Spawn blueprint if pointer is not null, else return null
}

class LevelScope
{
public:
    LevelScope(const FString& MapName) { AutomationOpenMap(MapName); }
    ~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
};

UWorld* GetTestGameWorld();

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params);

}  // namespace Test
}  // namespace ForDevOps

#endif