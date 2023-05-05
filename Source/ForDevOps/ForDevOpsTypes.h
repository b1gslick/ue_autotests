#pragma once

#include "CoreMinimal.h"
#include "ForDevOpsTypes.generated.h"

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{
    SPHERE = 0 UMETA(DisplayName = "MY COOL SPHERE"),
    CUBE UMETA(DisplayName = "MY COOL CUBE"),
    CYLINDER,
    CONE
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