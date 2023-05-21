// This is My project

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "ForDevOps/Tests/InventoryComponentTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ForDevOps/Components/InventoryComponent.h"
#include "ForDevOps/ForDevOpsTypes.h"
#include "TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentCouldBeCreated, "DevOpsGame.Components.Inventory.ComponentCouldBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FItemScoresShouldBeZerosByDefault, "DevOpsGame.Components.Inventory.ItemScoresShouldBeZerosByDefault",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNegativeScoreShouldntBeAdded, "DevOpsGame.Components.Inventory.NegativeScoreShouldntBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPositiveScoreShouldBeAdded, "DevOpsGame.Components.Inventory.PositiveScoreShouldBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScoreMoreThanLimitCanBeAdded, "DevOpsGame.Components.Inventory.ScoreMoreThanLimitCanBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

using namespace ForDevOps::Test;

namespace
{
class UInventoryComponentTestable : public UInventoryComponent
{
public:
    void SetLimits(const TMap<EInventoryItemType, int32>& Limits) { InventoryLimits = Limits; }
};

TMap<EInventoryItemType, int32> InitLimits(UInventoryComponentTestable* InvComp, int32 LimitValue)
{
    TMap<EInventoryItemType, int32> InvLimits;
    ENUM_LOOP_START(EInventoryItemType, EElem)
    InvLimits.Add(EElem, LimitValue);
    ENUM_LOOP_END
    InvComp->SetLimits(InvLimits);
    return InvLimits;
}

}  // namespace

bool FComponentCouldBeCreated::RunTest(const FString& Parameters)
{
    const UInventoryComponent* SUT_InventoryComponent = NewObject<UInventoryComponent>();
    if (!TestNotNull("Inventory component exist", SUT_InventoryComponent)) return false;
    return true;
}

bool FItemScoresShouldBeZerosByDefault::RunTest(const FString& Parameters)
{
    const UInventoryComponent* SUT_InventoryComponent = NewObject<UInventoryComponent>();
    if (!TestNotNull("Inventory component exist", SUT_InventoryComponent)) return false;

    // ENUM_LOOP_START(EInventoryItemType, EElem)
    // TestTrueExpr(SUT_InventoryComponent->GetInventoryAmountByType(EElem) == 0);
    // ENUM_LOOP_END

    ForEach<EInventoryItemType>([&](EInventoryItemType EElem)  //
        { TestTrueExpr(SUT_InventoryComponent->GetInventoryAmountByType(EElem) == 0); });

    return true;
}

bool FNegativeScoreShouldntBeAdded::RunTest(const FString& Parameters)
{
    UInventoryComponentTestable* SUT_InventoryComponent = NewObject<UInventoryComponentTestable>();
    if (!TestNotNull("Inventory component exist", SUT_InventoryComponent)) return false;

    InitLimits(SUT_InventoryComponent, 100);

    const int32 NegativeScoreAmountt = -5;
    const int32 InitialScoreAmount = 10;
    ENUM_LOOP_START(EInventoryItemType, EElem)
    TestTrueExpr(SUT_InventoryComponent->TryToAddItem({EElem, InitialScoreAmount}));
    TestTrueExpr(SUT_InventoryComponent->GetInventoryAmountByType(EElem) == InitialScoreAmount);
    TestTrueExpr(!SUT_InventoryComponent->TryToAddItem({EElem, NegativeScoreAmountt}));
    TestTrueExpr(SUT_InventoryComponent->GetInventoryAmountByType(EElem) == InitialScoreAmount);
    ENUM_LOOP_END
    return true;
}

bool FPositiveScoreShouldBeAdded::RunTest(const FString& Parameters)
{
    UInventoryComponentTestable* SUT_InventoryComponent = NewObject<UInventoryComponentTestable>();
    if (!TestNotNull("Inventory component exist", SUT_InventoryComponent)) return false;

    const int32 ScoreLimit = 100;

    const auto InvLimits = InitLimits(SUT_InventoryComponent, 100);

    ENUM_LOOP_START(EInventoryItemType, EElem)
    for (int32 i = 0; i < InvLimits[EElem]; ++i)
    {
        TestTrueExpr(SUT_InventoryComponent->TryToAddItem({EElem, 1}));
        TestTrueExpr(SUT_InventoryComponent->GetInventoryAmountByType(EElem) == i + 1);
    }
    ENUM_LOOP_END
    return true;
}

bool FScoreMoreThanLimitCanBeAdded::RunTest(const FString& Parameters)
{
    UInventoryComponentTestable* SUT_InventoryComponent = NewObject<UInventoryComponentTestable>();
    if (!TestNotNull("Inventory component exist", SUT_InventoryComponent)) return false;
    const int32 ScoreLimit = 100;
    InitLimits(SUT_InventoryComponent, ScoreLimit);

    TestTrueExpr(SUT_InventoryComponent->TryToAddItem({EInventoryItemType::SPHERE, 10}));
    TestTrueExpr(SUT_InventoryComponent->GetInventoryAmountByType(EInventoryItemType::SPHERE) == 10);

    TestTrueExpr(!SUT_InventoryComponent->TryToAddItem({EInventoryItemType::SPHERE, ScoreLimit + 1}));
    TestTrueExpr(SUT_InventoryComponent->GetInventoryAmountByType(EInventoryItemType::SPHERE) == 10);

    return true;
}

#endif
