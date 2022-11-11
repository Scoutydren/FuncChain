// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FunctionMesh.generated.h"

class UProceduralMeshComponent;

UENUM(BlueprintType)
enum class FunctionType : uint8
{
	EMPTY UMETA(DisplayName = "Empty"),
	LINEAR UMETA(DisplayName = "Linear"),
	QUADRATIC UMETA(DisplayName = "Quadratic"),
	EXPONENTIAL UMETA(DisplayName = "Exponential"),
	LOGARITHMIC UMETA(DisplayName = "Logarithmic"),
	SINE UMETA(DisplayName = "Sine"),
};

UCLASS()
class FUNCCHAIN_API AFunctionMesh : public AActor
{
	GENERATED_BODY()
	
public:	

	const float upperX = 100;
	const float upperY = 100;
	const float lowerX = -100;
	const float lowerY = -100;

	UPROPERTY(BlueprintReadWrite)
	UProceduralMeshComponent* ProceduralMesh;

	AFunctionMesh();

	UFUNCTION(BlueprintCallable)
	void Generate(FunctionType functionType, int identifier, float a, float b, float c, float d, float lowerBound, float upperBound);

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	void GenerateTestTriangle(); 

	void GenerateLinearFunction(int identifier, float a, float b, float c, float d, float lowerBound, float upperBound);

	void GenerateQuadraticFunction(int identifier, float a, float b, float c, float d, float lowerBound, float upperBound);

	void GenerateExponentialFunction(int identifier, float a, float b, float c, float d, float lowerBound, float upperBound);

	void GenerateLogarithmicFunction(int identifier, float a, float b, float c, float d, float lowerBound, float upperBound);

	void GenerateSineFunction(int identifier, float a, float b, float c, float d, float lowerBound, float upperBound);

	bool IsOutOfBounds(float x, float y);
	
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	TArray<FVector> vertices;

	UPROPERTY(EditAnywhere)
	TArray<int32> Triangles;
	
	/*UPROPERTY(EditAnywhere)
	TArray<FVector> normals;
	
	UPROPERTY(EditAnywhere)
	TArray<FVector2D> UV0;
	
	UPROPERTY(EditAnywhere)
	TArray<FProcMeshTangent> tangents;
	
	UPROPERTY(EditAnywhere)
	TArray<FLinearColor> vertexColors;*/
};