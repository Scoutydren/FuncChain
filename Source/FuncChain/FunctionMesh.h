// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FunctionMesh.generated.h"

class UProceduralMeshComponent;

UCLASS()
class FUNCCHAIN_API AFunctionMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	AFunctionMesh();

protected:
	virtual void BeginPlay() override;

	void GenerateTestTriangle(); 

	void GenerateLinearFunction(); 

	void GenerateSinFunction();
	
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


public:	
	virtual void Tick(float DeltaTime) override;

private: 
	UProceduralMeshComponent* ProceduralMesh; 
};
