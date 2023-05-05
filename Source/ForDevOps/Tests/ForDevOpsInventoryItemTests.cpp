// This is My project

#if WITH_AUTOMATION_TESTS

#include "ForDevOps/Tests/ForDevOpsInventoryItemTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ForDevOps/Items/InventoryItem.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ForDevOps/Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ForDevOps/ForDevOpsCharacter.h"
#include "ForDevOps/Components/InventoryComponent.h"

DEFINE_LOG_CATEGORY_STATIC(TestLog, All, All);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeCreated, "DevOpsGame.Items.Inventory.CppActorsCantBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly, "DevOpsGame.Items.Inventory.BlueprintShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryDataShouldBeSetupCorrectly, "DevOpsGame.Items.Inventory.InventoryDataShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryCanBetaken, "DevOpsGame.Items.Inventory.InventoryCanBetaken",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

namespace
{
constexpr char* InventoryItemBPName = "/Script/Engine.Blueprint'/Game/Inventory/BP_InventoryItem.BP_InventoryItem'";
constexpr char* InventoryItemBPTestName = "/Script/Engine.Blueprint'/Game/Tests/BP_Test_InventoryItem_Child.BP_Test_InventoryItem_Child'";

}  // namespace

using namespace ForDevOps::Test;

bool FCppActorCantBeCreated::RunTest(const FString& Parameters)
{
    const FString ExpectedWarningMsg =
        FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *AInventoryItem::StaticClass()->GetName());

    AddExpectedError(ExpectedWarningMsg, EAutomationExpectedErrorFlags::Exact);
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* SUT_World = GetTestGameWorld();
    if (!TestNotNull("Inventory item exists", SUT_World)) return false;

    const FTransform InitialTransform(FVector{1000.0f});
    const AInventoryItem* SUT_InvItem = SUT_World->SpawnActor<AInventoryItem>(AInventoryItem::StaticClass(), InitialTransform);

    if (!TestNull("Inventory item not exists", SUT_InvItem)) return false;

    return true;
}

bool FBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* SUT_World = GetTestGameWorld();
    if (!TestNotNull("Inventory item not exists", SUT_World)) return false;

    const FTransform InitialTransform(FVector{1000.0f});
    const AInventoryItem* SUT_InvItem = CreateBlueprint<AInventoryItem>(SUT_World, InventoryItemBPName, InitialTransform);
    if (!TestNotNull("Inventory item exists", SUT_InvItem)) return false;
    // const AInventoryItem* SUT_InvItem = SUT_World->SpawnActor<AInventoryItem>(AInventoryItem::StaticClass(), InitialTransform); load
    // class

    const auto SUT_CollisionComp = SUT_InvItem->FindComponentByClass<USphereComponent>();
    if (!TestNotNull("Sphere component exists", SUT_CollisionComp)) return false;

    TestTrueExpr(SUT_CollisionComp->GetUnscaledSphereRadius() >= 30.0f);
    TestTrueExpr(SUT_CollisionComp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);

    TestTrueExpr(SUT_CollisionComp->GetGenerateOverlapEvents());
    TestTrueExpr(SUT_InvItem->GetRootComponent() == SUT_CollisionComp);

    ENUM_LOOP_START(ECollisionChannel, EElem)
    if (EElem != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(SUT_CollisionComp->GetCollisionResponseToChannel(EElem) == ECollisionResponse::ECR_Overlap);
    }
    ENUM_LOOP_END

    const auto SUT_TextRenderComp = SUT_InvItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("Text component exists", SUT_TextRenderComp)) return false;

    const auto SUT_StaticMeshComp = SUT_InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static mesh component exists", SUT_StaticMeshComp)) return false;

    TestTrueExpr(SUT_StaticMeshComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision);

    return true;
}

bool FInventoryDataShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* SUT_World = GetTestGameWorld();
    if (!TestNotNull("WORLD  exists", SUT_World)) return false;

    const FTransform InitialTransform(FVector{1000.0f});
    AInventoryItem* SUT_InvItem = CreateBlueprint<AInventoryItem>(SUT_World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", SUT_InvItem)) return false;

    /* FVector Vector{120.f, 130.0f, 140.0f};
   CallFuncByNameWithParams(SUT_InvItem, "SetTestData",
        {                                     //
            FString::FromInt(123),            //
            FString::SanitizeFloat(345.45f),  //
            FString("\"My cool string\""),    //
            FString("Yes"),                   //
            FString::Printf(TEXT("(X=%f,Y=%f,Z=%f)"), Vector.X, Vector.Y, Vector.Z)

        });
    */

    const int32 Score = 13;
    const FInventoryData SUT_InvData{EInventoryItemType::CYLINDER, Score};
    const FLinearColor Color = FLinearColor::Yellow;

    CallFuncByNameWithParams(SUT_InvItem, "SetInventoryData",
        {
            SUT_InvData.ToString(),  //
            Color.ToString()         //
        });

    const auto SUT_TextRenderComp = SUT_InvItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("Text component exists", SUT_TextRenderComp)) return false;

    // UE_LOG(TestLog, Error, TEXT("SUT_TextRenderComp : %s"), SUT_TextRenderComp->Text.ToString());
    // Test property from TextRenderComponent
    TestTrueExpr(SUT_TextRenderComp->Text.ToString().Equals(FString::FromInt(Score)));
    // UE_LOG(TestLog, Error, TEXT("TextRenderColor: %s"), SUT_TextRenderComp->TextRenderColor.ToString());
    TestTrueExpr(SUT_TextRenderComp->TextRenderColor == Color.ToFColor(true));

    const auto SUT_StaticMeshComp = SUT_InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static mesh component exists", SUT_StaticMeshComp)) return false;

    const auto Material = SUT_StaticMeshComp->GetMaterial(0);
    if (!TestNotNull("Material exists", Material)) return false;

    FLinearColor MaterialColor;
    Material->GetVectorParameterValue(FHashedMaterialParameterInfo{"Color"}, MaterialColor);

    // Test Static mesh
    TestTrueExpr(MaterialColor == Color);

    return true;
}

bool FInventoryCanBetaken::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* SUT_World = GetTestGameWorld();
    if (!TestNotNull("WORLD  exists", SUT_World)) return false;

    const FTransform InitialTransform(FVector{1000.0f});
    AInventoryItem* SUT_InvItem = CreateBlueprint<AInventoryItem>(SUT_World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", SUT_InvItem)) return false;

    const int32 Score = 13;
    const EInventoryItemType Type = EInventoryItemType::CYLINDER;
    const FInventoryData SUT_InvData{Type, Score};
    const FLinearColor Color = FLinearColor::Yellow;

    CallFuncByNameWithParams(SUT_InvItem, "SetInventoryData",
        {
            SUT_InvData.ToString(),  //
            Color.ToString()         //
        });

    TArray<AActor*> Pawns;
    UGameplayStatics::GetAllActorsOfClass(SUT_World, AForDevOpsCharacter::StaticClass(), Pawns);

    if (!TestTrueExpr(Pawns.Num() == 1)) return false;

    const auto SUT_Character = Cast<AForDevOpsCharacter>(Pawns[0]);
    if (!TestNotNull("Character exists", SUT_Character)) return false;

    // check initial inventory
    const auto SUT_InvCom = SUT_Character->FindComponentByClass<UInventoryComponent>();  //
    if (!TestNotNull("Inv comp exists", SUT_InvCom)) return false;
    TestTrueExpr(SUT_InvCom->GetInventoryAmountByType(Type) == 0);

    // Character takes item..
    SUT_Character->SetActorLocation(InitialTransform.GetLocation());

    TestTrueExpr(SUT_InvCom->GetInventoryAmountByType(Type) == Score);
    TestTrueExpr(!IsValid(SUT_InvItem));

    TArray<AActor*> InvItems;
    UGameplayStatics::GetAllActorsOfClass(SUT_World, AInventoryItem::StaticClass(), InvItems);
    if (!TestTrueExpr(InvItems.Num() == 0)) return false;

    return true;
}
#endif
