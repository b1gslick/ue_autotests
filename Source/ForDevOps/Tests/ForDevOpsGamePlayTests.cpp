// This is My project

#if WITH_AUTOMATION_TESTS

#include "ForDevOps/Tests/ForDevOpsGamePlayTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ForDevOps/Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ForDevOps/Items/InventoryItem.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "ForDevOps/Tests/Utils/JsonUtils.h"
#include "ForDevOps/Tests/Utils/InputRecordingUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCanBeTakenOnJump, "DevOpsGame.Gameplay.InventoryItemCanBeTakenOnJump",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCantBeTakenOnJumpIfToHigh, "DevOpsGame.Gameplay.FInventoryItemCantBeTakenOnJumpIfToHigh",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllItemsAreTakenOnMovement, "DevOpsGame.Gameplay.AllItemsAreTakenOnMovement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FAllItemsAreTakenOnRecordingMovement, "DevOpsGame.Gameplay.AllItemsAreTakenOnRecordingMovement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FMapChouldBeLoaded, "DevOpsGame.Gameplay.MapShouldBeLoaded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace ForDevOps::Test;

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, ACharacter*, Character);
bool FJumpLatentCommand::Update()
{
    if (!Character) return true;
    const int32 ActionIndex = GetActionBindingIndexByName(Character->InputComponent, "Jump", EInputEvent::IE_Pressed);
    if (ActionIndex != -1)
    {
        const auto JumpActionBind = Character->InputComponent->GetActionBinding(ActionIndex);
        JumpActionBind.ActionDelegate.Execute(EKeys::SpaceBar);
    }
    return true;
}

bool FInventoryItemCanBeTakenOnJump::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/InventoryTestLevel1");

    UWorld* SUT_World = GetTestGameWorld();
    if (!TestNotNull("WORLD  exists", SUT_World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(SUT_World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(SUT_World, AInventoryItem::StaticClass(), InventoryItems);
    if (!TestEqual("Only one item exists", InventoryItems.Num(), 1)) return false;

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(SUT_World, AInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
        },
        2.0f))

    return true;
}

bool FInventoryItemCantBeTakenOnJumpIfToHigh::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/InventoryTestLevel2");

    UWorld* SUT_World = GetTestGameWorld();
    if (!TestNotNull("WORLD  exists", SUT_World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(SUT_World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(SUT_World, AInventoryItem::StaticClass(), InventoryItems);
    if (!TestEqual("Only one item exists", InventoryItems.Num(), 1)) return false;

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(SUT_World, AInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 1);
        },
        2.0f))

    return true;
}

bool FAllItemsAreTakenOnMovement::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/InventoryTestLevel3");

    UWorld* SUT_World = GetTestGameWorld();
    if (!TestNotNull("WORLD  exists", SUT_World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(SUT_World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(SUT_World, AInventoryItem::StaticClass(), InventoryItems);
    TestTrueExpr(InventoryItems.Num() == 7);

    const int32 MoveForwardIndex = GetAxisBindingIndexByName(Character->InputComponent, "MoveForward");
    TestTrueExpr(MoveForwardIndex != INDEX_NONE);
    const int32 MoveRightIndex = GetAxisBindingIndexByName(Character->InputComponent, "MoveRigth");
    TestTrueExpr(MoveForwardIndex != INDEX_NONE);

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

    ADD_LATENT_AUTOMATION_COMMAND(FForDevOpsUntilLatentCommand(
        [=]() { Character->InputComponent->AxisBindings[MoveForwardIndex].AxisDelegate.Execute(1.0f); }, []() {}, 3.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FForDevOpsUntilLatentCommand(
        [=]() { Character->InputComponent->AxisBindings[MoveRightIndex].AxisDelegate.Execute(1.0f); }, []() {}, 2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(SUT_World, AInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
            return true;
        }))

    return true;
}

class FSimulateMovementLatentCommand : public IAutomationLatentCommand
{
public:
    FSimulateMovementLatentCommand(UWorld* InWorld, UInputComponent* InInputComponent, const TArray<FBindingsData>& InBindingsData)
        : World(InWorld), InputComponent(InInputComponent), BindingData(InBindingsData)
    {
    }

    virtual bool Update() override
    {
        if (!World || !InputComponent) return true;
        if (WorldStartTime == 0.0f)
        {
            WorldStartTime = World->TimeSeconds;
        }

        while (World->TimeSeconds - WorldStartTime >= BindingData[Index].WorldTime)
        {
            for (int32 i = 0; i < InputComponent->AxisBindings.Num(); ++i)
            {
                const float AxisValue = BindingData[Index].AxisValues[i].Value;
                InputComponent->AxisBindings[i].AxisDelegate.Execute(AxisValue);
            } /*
            if (Index > 0)
            {
                for (int32 i = 0; i < InputComponent->GetNumActionBindings(); ++i)
                {
                    const auto ActionValue = BindingData[Index].ActionValues[i];
                    const auto PrevActionValue = BindingData[Index = 1].ActionValues[i];
                    if (ActionValue.State && ActionValue.State != PrevActionValue.State)
                    {
                        InputComponent->GetActionBinding(i).ActionDelegate.Execute(ActionValue.Key);
                    }
                }
            }
            */

            if (++Index >= BindingData.Num()) return true;
        }
        return false;
    }

private:
    const UWorld* World;
    const UInputComponent* InputComponent;
    const TArray<FBindingsData> BindingData;
    int32 Index{0};
    float WorldStartTime{0.0f};
};

void FAllItemsAreTakenOnRecordingMovement::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    struct FTestData
    {
        FString TestName;
        FString MapPath;
        FString JsonName;
    };

    const TArray<FTestData> TestData =  //
        {
            {"MainMap", "/Game/TopDown/Maps/TopDownMap", "CharacterTestInput.json"},            //
            {"CustomMap", "/Game/TopDown/Maps/CustomMap", "CharacterTestInputCustomMap.json"},  //
        };

    for (const auto OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.TestName);
        OutTestCommands.Add(FString::Printf(TEXT("%s,%s"), *OneTestData.MapPath, *OneTestData.JsonName));
    }
}

bool FAllItemsAreTakenOnRecordingMovement::RunTest(const FString& Parameters)
{
    TArray<FString> ParsedParams;
    Parameters.ParseIntoArray(ParsedParams, TEXT(","));
    if (!TestTrue("Map Name and json params should exist", ParsedParams.Num() == 2)) return false;

    const auto Level = LevelScope(ParsedParams[0]);

    UWorld* SUT_World = GetTestGameWorld();
    if (!TestNotNull("WORLD  exists", SUT_World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(SUT_World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(SUT_World, AInventoryItem::StaticClass(), InventoryItems);
    TestTrueExpr(InventoryItems.Num() == 9);

    const FString FileName = GetTestDataDir().Append(ParsedParams[1]);

    FInputData InputData;
    if (!JsonUtils::ReadInputData(FileName, InputData)) return false;
    if (!TestTrue("Input data is not empty", InputData.Bindings.Num() > 0)) return false;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(SUT_World, 0);
    if (!TestNotNull("Player controller exists", PlayerController)) return false;

    Character->SetActorTransform(InputData.InitialTransform);
    PlayerController->SetControlRotation(InputData.InitialTransform.Rotator());

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FSimulateMovementLatentCommand(SUT_World, Character->InputComponent, InputData.Bindings));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(SUT_World, AInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 8);
            return true;
        }))
    return true;
}

void FMapChouldBeLoaded::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    /* const TTuple<FString, int32, bool, float> MyTuple1 = {"unreal", 5, true, 100.0f};
     const auto MyTuple2 = MakeTuple(FString("unity"), 7, false, 50.0f);

     auto str = MyTuple1.Get<0>();
     auto intgr = MyTuple1.Get<1>();
     auto bln = MyTuple1.Get<2>();
     auto flt = MyTuple1.Get<3>();
     UE_LOG(LogTemp, Display, TEXT("Tuple1: %s, %i, %i, %f"), *str, intgr, bln, flt);

     Tie(str, intgr, bln, flt) = MyTuple2;
     UE_LOG(LogTemp, Display, TEXT("Tuple1: %s, %i, %i, %f"), *str, intgr, bln, flt);
     */
    const TArray<TTuple<FString, FString>> TestData =  //
        {
            {"MainMap", "/Game/TopDown/Maps/TopDownMap"},
            {"CustomMap", "/Game/TopDown/Maps/CustomMap"},
        };

    for (const auto OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.Key);
        OutTestCommands.Add(
            OneTestData.Value);  // for 2 parameters concatinate parameters into string FString::Printf(TEXT("%s,%s"), *text, *text)
    }
}

bool FMapChouldBeLoaded::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope(Parameters);
    // ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f)); Not need into production
    return true;
}

#endif
