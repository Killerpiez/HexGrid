// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "ProceduralMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGrid::AGrid()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DefaultSceneRoot"));
	DefaultSceneRoot->SetupAttachment(RootComponent);


	LineMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("LineMesh"));
	LineMesh->SetupAttachment(DefaultSceneRoot);

	SelectionMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("SelectionMesh"));
	SelectionMesh->SetupAttachment(DefaultSceneRoot);
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

	DefaultSceneRoot->SetVisibility(false);
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int32 i = 0; i < HexagonPoints.Num(); i++)
	{
		DrawDebugLine(GetWorld(), HexagonPoints[i].Location, HexagonPoints[i].Location + FVector(0.0f, 0.0f, 10000.0), FColor::Red, true, -1.0f, 1, 3.0f);
	}
}

void AGrid::SetHexInformation()
{
	FVector TempLocation;
	float TempX, TempY;
	int32 HexIndexCount = 0;

	for (int32 i = 0; i < NumRows; i++)
	{
		TempX = i * 1.5 * Radius;
		TempY = i * Radius * sqrt(3) * 0.5f;
		TempLocation = FVector(TempX, TempY, 0.0f);

		for (int32 j = 0; j < NumColumns; j++)
		{
			TempX = Radius * 1.5;
			TempY = Radius * sqrt(3) * -0.5f;
			TempLocation = TempLocation + FVector(TempX, TempY, 0.0f);

			TempLocation = TempLocation + FVector(0.0f, 0.0f, GetZOffset(TempLocation));
			UE_LOG(LogTemp, Warning, TEXT("Location: %f, %f, %f"), TempLocation.X, TempLocation.Y, TempLocation.Z);
			AddHexData(TempLocation, HexIndexCount);
			SetLinkID(HexIndexCount, i, j);
			HexIndexCount++;

		}
	}
}

float AGrid::GetZOffset(FVector Location)
{
	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, LocalToWorldTransform(Location), LocalToWorldTransform(Location) - FVector(0.0f, 0.0f, 50000.0f), ECC_Visibility))
	{
		// Finds the ground and then adds 20cm so that we are just over the ground
		return Location.Z - HitResult.Distance + 20.0f;
	}

	// UE_LOG(LogTemp, Warning, TEXT("Could not find ground. Make sure Grid is the highest Actor in the world space"));
	return 0.0f;
}

FVector AGrid::LocalToWorldTransform(FVector LocalLocation)
{	
	return DefaultSceneRoot->GetComponentTransform().GetLocation();
}

void AGrid::AddHexData(FVector Location, int32 HexIndex)
{
	HexagonPoints[HexIndex].Location = Location;
	HexagonPoints[HexIndex].ID = HexIndex;
}

void AGrid::SetLinkID(int32 ID, int32 CurrentRow, int32 CurrentColumn)
{
	// TODO Refactor this, a lot of repeating code here
	TArray<int32> TempArray;

	// Set Small corner links (the first and last indices)
	if (CurrentRow == 0 && CurrentColumn == 0)
	{
		TempArray.Add(ID + 1);
		TempArray.Add(NumColumns + ID);

		HexagonPoints[ID].Links.Append(TempArray);
		TempArray.Empty();
	}
	else if (CurrentRow == NumRows - 1 && CurrentColumn == NumColumns - 1)
	{
		TempArray.Add(ID - 1);
		TempArray.Add(ID - NumColumns);

		HexagonPoints[ID].Links.Append(TempArray);
		TempArray.Empty();
	}

	// Set Corner Links (the hex opposite corner of the first and last indice)
	if (CurrentRow == NumRows - 1 && CurrentColumn == 0)
	{
		TempArray.Add(ID + 1);
		TempArray.Add(ID - NumColumns);
		TempArray.Add(ID - NumColumns + 1);

		HexagonPoints[ID].Links.Append(TempArray);
		TempArray.Empty();
	}
	else if (CurrentRow == 0 && CurrentColumn == NumColumns - 1)
	{
		TempArray.Add(ID - 1);
		TempArray.Add(ID + NumColumns);
		TempArray.Add(ID + NumColumns - 1);

		HexagonPoints[ID].Links.Append(TempArray);
		TempArray.Empty();
	}

	// Set the edges
	if (CurrentRow == 0 && !(CurrentColumn == 0 || CurrentColumn == NumColumns - 1))
	{
		TempArray.Add(ID - 1);
		TempArray.Add(ID + 1);
		TempArray.Add(ID + NumColumns);
		TempArray.Add(ID + NumColumns - 1);

		HexagonPoints[ID].Links.Append(TempArray);
		TempArray.Empty();
	}
	else if (CurrentRow == NumRows - 1 && !(CurrentColumn == 0 || CurrentColumn == NumColumns - 1))
	{
		TempArray.Add(ID - 1);
		TempArray.Add(ID + 1);
		TempArray.Add(ID - NumColumns);
		TempArray.Add(ID - NumColumns + 1);

		HexagonPoints[ID].Links.Append(TempArray);
		TempArray.Empty();
	}
	else if (CurrentColumn == 0 && !(CurrentRow == 0 || CurrentRow == NumRows - 1))
	{
		TempArray.Add(ID + 1);
		TempArray.Add(ID + NumColumns);
		TempArray.Add(ID - NumColumns);
		TempArray.Add(ID - NumColumns + 1);

		HexagonPoints[ID].Links.Append(TempArray);
		TempArray.Empty();
	}
	else if (CurrentColumn == NumColumns - 1 && !(CurrentRow == 0 || CurrentRow == NumRows - 1))
	{
		TempArray.Add(ID - 1);
		TempArray.Add(ID + NumColumns);
		TempArray.Add(ID - NumColumns);
		TempArray.Add(ID + NumColumns - 1);

		HexagonPoints[ID].Links.Append(TempArray);
		TempArray.Empty();
	}

	// Set centers
	if (CurrentRow != 0 && CurrentRow != NumRows - 1 && CurrentColumn != 0 && CurrentColumn != NumColumns - 1)
	{
		TempArray.Add(ID + 1);
		TempArray.Add(ID - 1);
		TempArray.Add(ID + NumColumns);
		TempArray.Add(ID - NumColumns);
		TempArray.Add(ID + NumColumns - 1);
		TempArray.Add(ID - NumColumns + 1);

		HexagonPoints[ID].Links.Append(TempArray);
		TempArray.Empty();
	}
}

void AGrid::FindVerticesAndTriangles()
{
	TArray<FVector> TempLineStartList, TempLineEndList;
	TArray<int32> AddLines;
	FVector TempLocation, TempLineStart, TempLineEnd;
	FRotator HexRotator = FRotator(0.0f, 0.0f, 60.0f);
	float TempX, TempY, TempZ;

	for (int32 i = HexagonPoints.Num() - 1; i >= 0; i--)
	{
		TempZ = 4 ^ 5;
		TempX = HexagonPoints[i].Location.X + (Radius * .5);
		TempY = HexagonPoints[i].Location.Y - sqrt(FMath::Square(Radius) - FMath::Square(Radius * 0.5f));
		TempLocation = FVector(TempX, TempY, 0.0f);

		for (int32 j = 0; j < 6; j++)
		{
			if (CheckDistance(TempLocation, HexagonPoints[i], TempLineStart, TempLineEnd))
			{
				TempLineStartList.Add(TempLineStart);
				TempLineEndList.Add(TempLineEnd);
			}

			TempLineEnd.Z = 0.0f;
			TempLocation = TempLineEnd;
		}

		if (TempLineStartList.Num() == 6)
		{
			AddLines = MakeLinesList(HexagonPoints[i].ID);

			for (int32 L = 0; L < AddLines.Num(); L++)
			{
				CreateLines(TempLineStartList[L], TempLineEndList[L], LineThickness, LineVertices, LineTriangles, false);
			}
		}
		else
		{
			for (int32 k = HexagonPoints[i].Links.Num() - 1; k >= 0; k--)
			{
				HexagonPoints[k].Links.Remove(HexagonPoints[i].ID);
			}

			RemoveIndex.Add(HexagonPoints[i].ID);
		}

		TempLineEndList.Empty();
		TempLineStartList.Empty();
		AddLines.Empty();
	}
}

bool AGrid::CheckDistance(FVector Location, FHexGridData Hex, FVector& TempLineStartOUT, FVector& TempLineEndOUT)
{
	FRotator HexRotator = FRotator(0.0f, 0.0f, 60.0f);
	FVector VectorOne, VectorTwo;
	float Distance;

	VectorOne = Location + FVector(0.0f, 0.0f, GetZOffset(Location));
	VectorTwo = HexRotator.RotateVector(Location - Hex.Location) + Hex.Location;
	VectorTwo.Z = 0.0f;
	VectorTwo = VectorTwo + FVector(0.0f, 0.0f, GetZOffset(VectorTwo));
	
	Distance = FVector::Distance(VectorOne, VectorTwo);

	if (Distance >= Radius * 1.2f)
	{
		TempLineStartOUT = VectorOne;
		TempLineEndOUT = VectorTwo;
		return true;
	}

	return false;
}

TArray<int32> AGrid::MakeLinesList(int32 CurrentID)
{
	TArray<int32> TempArray, LineList;

	TempArray.Empty();
	TempArray.Add(3);
	TempArray.Add(4);
	TempArray.Add(5);
	LineList.Append(TempArray);

	if (CurrentID == HexagonPoints.Num() - 1)
	{
		TempArray.Empty();
		TempArray.Add(0);
		TempArray.Add(1);
		TempArray.Add(2);
		LineList.Append(TempArray);
	}
	else if (CurrentID > HexagonPoints.Num() - 1 - NumColumns)
	{
		TempArray.Empty();
		TempArray.Add(1);
		TempArray.Add(2);
		LineList.Append(TempArray);
	}
	else if (CurrentID - ((CurrentID / NumColumns) * NumColumns) == 0)
	{
		TempArray.Empty();
		TempArray.Add(2);
		LineList.Append(TempArray);
	}
	else if (CurrentID - ((CurrentID / NumColumns) * NumColumns) == NumColumns - 1)
	{
		TempArray.Empty();
		TempArray.Add(0);
		LineList.Append(TempArray);
	}

	if (RemoveIndex.Contains(CurrentID + 1))
	{
		TempArray.Empty();
		TempArray.Add(0);
		LineList.Append(TempArray);
	}
	else if (RemoveIndex.Contains(CurrentID + NumColumns))
	{
		TempArray.Empty();
		TempArray.Add(1);
		LineList.Append(TempArray);
	}
	else if (RemoveIndex.Contains(CurrentID + NumColumns - 1) && CurrentID - ((CurrentID / NumColumns) * NumColumns) != 0)
	{
		TempArray.Empty();
		TempArray.Add(2);
		LineList.Append(TempArray);
	}

	return LineList;
}

void AGrid::CreateLines(FVector StartLocation, FVector EndLocation, float Thickness, TArray<FVector>& VerticesOUT, TArray<int32>& TrianglesOUT, bool IsSelection)
{
	TArray<int32> TempIntArray;
	TArray<FVector> TempVectorArray;
	FVector ThicknessDirection;
	float HalfThickness;

	HalfThickness = Thickness / 2;

	ThicknessDirection = EndLocation - StartLocation;
	ThicknessDirection.Normalize();
	ThicknessDirection = FVector::CrossProduct(ThicknessDirection, FVector(0.0f, 0.0f, 1.0f));

	TempIntArray.Add(VerticesOUT.Num() + 2);
	TempIntArray.Add(VerticesOUT.Num() + 1);
	TempIntArray.Add(VerticesOUT.Num() + 0);
	TempIntArray.Add(VerticesOUT.Num() + 2);
	TempIntArray.Add(VerticesOUT.Num() + 3);
	TempIntArray.Add(VerticesOUT.Num() + 1);

	TrianglesOUT.Append(TempIntArray);

	TempVectorArray.Add(StartLocation + (ThicknessDirection * CheckType(IsSelection, HalfThickness)));
	TempVectorArray.Add(EndLocation + (ThicknessDirection * HalfThickness));
	TempVectorArray.Add(StartLocation - (ThicknessDirection * CheckType(IsSelection, HalfThickness)));
	TempVectorArray.Add(EndLocation - (ThicknessDirection * HalfThickness));

	VerticesOUT.Append(TempVectorArray);
}

float AGrid::CheckType(bool Type, float HalfThickness)
{
	if (Type)
	{
		return 0.0f;
	}

	return HalfThickness;
}

void AGrid::RemoveHexPoints()
{
	for (int32 i = 0; i < RemoveIndex.Num(); i++)
	{
		if (HexagonPoints.IsValidIndex(RemoveIndex[i]))
		{
			HexagonPoints.RemoveAt(RemoveIndex[i]);
		}
	}

	RemoveIndex.Empty();
}

void AGrid::CreateGrid()
{
	LineMesh->CreateMeshSection(0, LineVertices, LineTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	LineMesh->SetMaterial(0, MI_Line);
}

void AGrid::CreateSelectionMesh()
{
	TArray<FVector> SelectionVertices;
	TArray<int32> SelectionTriangles;
	FVector TempLocation;
	float TempX;

	TempX = sqrt(FMath::Square(Radius) - FMath::Square(Radius * 0.5f));
	TempLocation = FVector(TempX, 0.0f, 0.0f);

	for (int32 i = 0; i < 6; i++)
	{
		TempLocation = TempLocation.RotateAngleAxis(60.0f, FVector(0.0f, 0.0f, 1.0f));
		TempLocation.Z = 0.0f;
		CreateLines(FVector(0.0f, 0.0f, 0.0f), TempLocation, Radius, SelectionVertices, SelectionTriangles, true);
	}

	SelectionMesh->SetVisibility(false, false);
	SelectionMesh->CreateMeshSection(0, SelectionVertices, SelectionTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	SelectionMesh->SetMaterial(0, MI_SelectionValid);
}

// This function is called in the blueprint construction script. This is where all the magic begins though
void AGrid::RefreshGrid(UMaterialInstanceDynamic* UpdatedLineMI, UMaterialInstanceDynamic* UpdatedSelectionValid, UMaterialInstanceDynamic* UpdatedSelectionInvalid)
{
	HexagonPoints.SetNum(NumRows * NumColumns);

	MI_Line = UpdatedLineMI;
	MI_SelectionValid = UpdatedSelectionValid;
	MI_SelectionInValid = UpdatedSelectionInvalid;

	// Gets and sets the location, and the LinkID for each hexagon
	SetHexInformation();

	// Calculates and sets our vertex and triangle arrays so we know where to draw each line
	//FindVerticesAndTriangles();

	// Remove the Hex Points that no longer exist based on terrain calculations
	//RemoveHexPoints();

	// Finally we create the grid with remaining vertices and triangles
	//CreateGrid();

	// With a much simplier function, we create 6 triangles for the hex mesh similar to how we created the grid
	//CreateSelectionMesh();
}