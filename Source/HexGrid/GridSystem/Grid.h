// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

USTRUCT(BlueprintType)
struct FHexGridData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HexGridData, meta = (AllowPrivateAccess = "true"))
		FVector Location = FVector(0.0f, 0.0f, 0.0f);

	// Links is how we establish pathfinding. Each hex created is given a unique ID and then told which ID's its linked to.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HexGridData, meta = (AllowPrivateAccess = "true"))
		TArray<int32> Links;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HexGridData, meta = (AllowPrivateAccess = "true"))
		int32 ID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HexGridData, meta = (AllowPrivateAccess = "true"))
		bool Occupied = false;
};

UCLASS()
class HEXGRID_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	

	AGrid();

protected:

	virtual void BeginPlay() override;

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UProceduralMeshComponent* LineMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UProceduralMeshComponent* SelectionMesh;

	// User defined variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = User, meta = (AllowPrivateAccess = "true"))
		int32 NumColumns = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = User, meta = (AllowPrivateAccess = "true"))
		int32 NumRows = 5;

	// 53.5999cm radius is approx a 1 square meter hex grid size. Radius here is defined as distance from center of hex perpendicular to the side
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = User, meta = (AllowPrivateAccess = "true"))
		float Radius = 53.599f;

	// Refresh variable responsible for real time refresh while moving the grid. Check false if you're experiencing massive frame drops while moving it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Refresh, meta = (AllowPrivateAccess = "true"))
		bool Refresh = false;

	// Other variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ClassDefault, meta = (AllowPrivateAccess = "true"))
		float LineThickness = 2.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ClassDefault, meta = (AllowPrivateAccess = "true"))
		float LineOpacity = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ClassDefault, meta = (AllowPrivateAccess = "true"))
		float SelectionOpacity = 0.25f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ClassDefault, meta = (AllowPrivateAccess = "true"))
		struct FLinearColor LineColor = FLinearColor(0.0, 0.0f, 0.0f, 1.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ClassDefault, meta = (AllowPrivateAccess = "true"))
		struct FLinearColor SelectionColorValid = FLinearColor(.008745f, .58333f, 0.0f, 1.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ClassDefault, meta = (AllowPrivateAccess = "true"))
		struct FLinearColor SelectionColorInvalid = FLinearColor(.526042f, .007333f, 0.0f, 1.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ClassDefault, meta = (AllowPrivateAccess = "true"))
		class UMaterialInstanceDynamic* MI_SelectionValid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ClassDefault, meta = (AllowPrivateAccess = "true"))
		class UMaterialInstanceDynamic* MI_SelectionInValid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ClassDefault, meta = (AllowPrivateAccess = "true"))
		class UMaterialInstanceDynamic* MI_Line;

public:	

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SetHexInformation();

	UFUNCTION()
		float GetZOffset(FVector Location);

	UFUNCTION()
		FVector LocalToWorldTransform(FVector LocalLocation);

	UFUNCTION()
		void AddHexData(FVector Location, int32 HexIndex);

	UFUNCTION()
		void SetLinkID(int32 ID, int32 CurrentRow, int32 CurrentColumn);

	UFUNCTION()
		void FindVerticesAndTriangles();

	// Checks to make sure our lines are not too far apart based on the Z axis. This is how we make sure the terrain is not too steep
	UFUNCTION()
		bool CheckDistance(FVector Location, FHexGridData Hex, FVector& TempLineStartOUT, FVector& TempLineEndOUT);

	// Finalizes our list of the grid to generate the lines
	UFUNCTION()
		void CreateLines(FVector StartLocation, FVector EndLocation, float Thickness, TArray<FVector>& VerticesOUT, TArray<int32>& TrianglesOUT, bool IsSelection);

	// Simple function that checks if the mesh to generate is going to need a thickness
	UFUNCTION()
		float CheckType(bool Type, float HalfThickness);

	UFUNCTION()
		TArray<int32> MakeLinesList(int32 CurrentID);

	UFUNCTION()
		void RemoveHexPoints();

	UFUNCTION()
		void CreateGrid();

	UFUNCTION()
		void CreateSelectionMesh();

	UFUNCTION(BlueprintCallable)
		void RefreshGrid(UMaterialInstanceDynamic* UpdatedLineMI, UMaterialInstanceDynamic* UpdatedSelectionValid, UMaterialInstanceDynamic* UpdatedSelectionInvalid);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ClassDefault, meta = (AllowPrivateAccess = "true"))
		TArray<FHexGridData> HexagonPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ClassDefault, meta = (AllowPrivateAccess = "true"))
		TArray<FVector> LineVertices;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ClassDefault, meta = (AllowPrivateAccess = "true"))
		TArray<int32> LineTriangles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ClassDefault, meta = (AllowPrivateAccess = "true"))
		TArray<int32> RemoveIndex;
};