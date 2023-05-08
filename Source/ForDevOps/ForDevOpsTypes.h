#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ForDevOpsTypes.generated.h"

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{
    SPHERE = 0 UMETA(DisplayName = "MY COOL SPHERE"),
    CUBE UMETA(DisplayName = "MY COOL CUBE"),
    CYLINDER,
    CONE
};

UCLASS()
class FORDEVOPS_API UForDevOpsLocalizationFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure)
    static FText GetInventoryItemText(EInventoryItemType ItemType)
    {
        switch (ItemType)
        {
            case EInventoryItemType::SPHERE: return NSLOCTEXT("Inventory", "Sphere_loc", "SPHERE");
            case EInventoryItemType::CUBE: return NSLOCTEXT("Inventory", "Cube_loc", "CUBE");
            case EInventoryItemType::CYLINDER: return NSLOCTEXT("Inventory", "Cylinder_loc", "CYLINDER");
            case EInventoryItemType::CONE: return NSLOCTEXT("Inventory", "Cone_loc", "CONE");
        }
        checkNoEntry();
        return FText{};
    }
};

USTRUCT(BlueprintType)
struct FInventoryData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BLueprintReadWrite)
    EInventoryItemType Type;

    UPROPERTY(EditAnywhere, BLueprintReadWrite, meta = (ClampMin = "0"))
    int32 Score;

    FString ToString() const { return FString::Printf(TEXT("(Type=%i,Score=%i)"), Type, Score); }
};

USTRUCT(BlueprintType)
struct FHealthData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1.0"))
    float MaxHealth{100.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1"))
    float HealModifier{10.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", Units = "s"))
    float HealRate{0.5f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", Units = "s"))
    float LifeSpan{5.0f};

    FString ToString() const
    {                                                                                           //
        return FString::Printf(TEXT("(MaxHealth=%f,HealModifier=%f,HealRate=%f,LifeSpan=%f)"),  //
            MaxHealth,                                                                          //
            HealModifier,                                                                       //
            HealRate,                                                                           //
            LifeSpan                                                                            //
        );
    }
};