// This is My project

#if WITH_AUTOMATION_TESTS

#include "ForDevOps/Tests/ForDevOpsCharacterTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ForDevOps/Tests/TestUtils.h"
#include "Engine/World.h"
#include "ForDevOps/ForDevOpsCharacter.h"
#include "ForDevOps/ForDevOpsTypes.h"
#include "Engine/DamageEvents.h"
#include "Tests/AutomationCommon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogDevOpsCharacterTests, All, All);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthMightBeChangedWithDamage, "DevOpsGame.Character.HealthMightBeChangedWithDamage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandSimpleWait, "DevOpsGame.LatentCommand.LatentCommandSimpleWait",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandSimpleLog, "DevOpsGame.LatentCommand.LatentCommandSimpleLog",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandOpenCloseMap, "DevOpsGame.LatentCommand.LatentCommandOpenCloseMap",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanBeKilled, "DevOpsGame.Character.CharacterCanBeKilled",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoHealShouldRestoreHealth, "DevOpsGame.Character.AutoHealShouldRestoreHealth",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace ForDevOps::Test;

namespace
{
constexpr char* CharacterTestBPName = "/Script/Engine.Blueprint'/Game/Tests/BP_Test_TopDownCharacter_Child.BP_Test_TopDownCharacter_Child'";

}  // namespace

bool FHealthMightBeChangedWithDamage::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* SUT_World = GetTestGameWorld();
    if (!TestNotNull("WORLD  exists", SUT_World)) return false;

    const FTransform InitialTransform{FVector{0.0f, -240.0f, 110.0f}};
    AForDevOpsCharacter* SUT_Character = CreateBlueprintDeferred<AForDevOpsCharacter>(
        SUT_World, CharacterTestBPName, InitialTransform);  // deferred fucniton which spawn lazy character
    if (!TestNotNull("Character exists", SUT_Character)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;

    CallFuncByNameWithParams(SUT_Character, "SetHealthData", {HealthData.ToString()});

    SUT_Character->FinishSpawning(InitialTransform);  // call spawning for start begin play

    const float DamageAmount = 10.0f;
    TestEqual("Health is full", SUT_Character->GetHealthPercent(), 1.0f);
    SUT_Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Health was decreaded", SUT_Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    return true;
}

bool FLatentCommandSimpleWait::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(3.0f));
    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(ForDevOpsLatentLogCommand, FString, LogMessage);
bool ForDevOpsLatentLogCommand::Update()
{
    UE_LOG(LogDevOpsCharacterTests, Display, TEXT("%s"), *LogMessage);
    return true;
}

bool FLatentCommandSimpleLog::RunTest(const FString& Parameters)
{
    UE_LOG(LogDevOpsCharacterTests, Display, TEXT("log 1"))
    ADD_LATENT_AUTOMATION_COMMAND(ForDevOpsLatentLogCommand("Latent log 1"));
    UE_LOG(LogDevOpsCharacterTests, Display, TEXT("log 2"))
    ADD_LATENT_AUTOMATION_COMMAND(ForDevOpsLatentLogCommand("Latent log 2"));
    UE_LOG(LogDevOpsCharacterTests, Display, TEXT("log 3"))
    return true;
}

bool FLatentCommandOpenCloseMap::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/EmptyTestLevel");  // create not anonymys ojbect for close map after return from function

    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FCharacterDestroyedLatentCommand, AForDevOpsCharacter*, Character, float, LifeSpan);
bool FCharacterDestroyedLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= LifeSpan)
    {
        if (IsValid(Character))
        {
            UE_LOG(LogDevOpsCharacterTests, Error, TEXT("Character wasn't destroyed"));
        }
        return true;
    }
    return false;
}

bool FCharacterCanBeKilled::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* SUT_World = GetTestGameWorld();
    if (!TestNotNull("WORLD  exists", SUT_World)) return false;

    const FTransform InitialTransform{FVector{0.0f, -240.0f, 110.0f}};
    AForDevOpsCharacter* SUT_Character = CreateBlueprintDeferred<AForDevOpsCharacter>(
        SUT_World, CharacterTestBPName, InitialTransform);  // deferred fucniton which spawn lazy character
    if (!TestNotNull("Character exists", SUT_Character)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;
    HealthData.LifeSpan = 1.5f;

    CallFuncByNameWithParams(SUT_Character, "SetHealthData", {HealthData.ToString()});

    SUT_Character->FinishSpawning(InitialTransform);  // call spawning for start begin play

    TestEqual("Health is full", SUT_Character->GetHealthPercent(), 1.0f);

    const auto KillingDamageAmount = HealthData.MaxHealth;
    SUT_Character->TakeDamage(KillingDamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Health is empty", SUT_Character->GetHealthPercent(), 0.0f);
    TestTrueExpr(SUT_Character->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None);

    ENUM_LOOP_START(ECollisionChannel, EElem)
    if (EElem != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(SUT_Character->GetCapsuleComponent()->GetCollisionResponseToChannel(EElem) == ECollisionResponse::ECR_Ignore);
    }
    ENUM_LOOP_END
    TestTrueExpr(SUT_Character->GetMesh()->GetCollisionEnabled() == ECollisionEnabled::QueryAndPhysics);
    TestTrueExpr(SUT_Character->GetMesh()->IsSimulatingPhysics());
    TestTrueExpr(FMath::IsNearlyEqual(SUT_Character->GetLifeSpan(), HealthData.LifeSpan));

    // ADD_LATENT_AUTOMATION_COMMAND(FCharacterDestroyedLatentCommand(SUT_Character, HealthData.LifeSpan));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [SUT_Character]()
        {
            if (IsValid(SUT_Character))
            {
                UE_LOG(LogDevOpsCharacterTests, Error, TEXT("Character wasn't destroyed"));
            }
        },
        HealthData.LifeSpan));

    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FAutoHealtCheckLatentCommand, AForDevOpsCharacter*, Character, float, HealingDuration);
bool FAutoHealtCheckLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= HealingDuration)
    {
        if (!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.0f))
        {
            UE_LOG(LogDevOpsCharacterTests, Error, TEXT("Health is not full"));
        }
        return true;
    }
    return false;
}

bool FAutoHealShouldRestoreHealth::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* SUT_World = GetTestGameWorld();
    if (!TestNotNull("WORLD  exists", SUT_World)) return false;

    const FTransform InitialTransform{FVector{0.0f, -240.0f, 110.0f}};
    AForDevOpsCharacter* SUT_Character = CreateBlueprintDeferred<AForDevOpsCharacter>(
        SUT_World, CharacterTestBPName, InitialTransform);  // deferred fucniton which spawn lazy character
    if (!TestNotNull("Character exists", SUT_Character)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 200.0f;
    HealthData.HealModifier = 5.0f;
    HealthData.HealRate = 0.5f;

    CallFuncByNameWithParams(SUT_Character, "SetHealthData", {HealthData.ToString()});

    SUT_Character->FinishSpawning(InitialTransform);  // call spawning for start begin play

    const float DamageAmount = 20.0f;
    TestEqual("Health is full", SUT_Character->GetHealthPercent(), 1.0f);
    SUT_Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Health was decreaded", SUT_Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    const float HealthDiff = HealthData.MaxHealth * (1.0f - SUT_Character->GetHealthPercent());
    const float HealingDuration = HealthData.HealRate * HealthDiff / HealthData.HealModifier;
    // ADD_LATENT_AUTOMATION_COMMAND(FAutoHealtCheckLatentCommand(SUT_Character, HealingDuration));

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [SUT_Character]()
        {
            if (!FMath::IsNearlyEqual(SUT_Character->GetHealthPercent(), 1.0f))
            {
                UE_LOG(LogDevOpsCharacterTests, Error, TEXT("Health is not full"));
            }
        },
        HealingDuration));

    return true;
}

#endif
