// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionMesh.h"
#include "ProceduralMeshComponent.h"


AFunctionMesh::AFunctionMesh()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	ProceduralMesh->SetupAttachment(GetRootComponent());
}

void AFunctionMesh::BeginPlay()
{
	Super::BeginPlay();

	//GenerateTestTriangle(); 
	GenerateLinearFunction(); 
	//GenerateSinFunction(); 
}

void AFunctionMesh::GenerateTestTriangle()
{
	/*vertices.Add(FVector(0, 0, 0));
	vertices.Add(FVector(0, 100, 0));
	vertices.Add(FVector(100, 100, 0));

	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);*/

	TArray<FVector> normals;
	normals.Add(FVector(1, 0, 0));
	normals.Add(FVector(1, 0, 0));
	normals.Add(FVector(1, 0, 0));

	TArray<FVector2D> UV0;
	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(10, 0));
	UV0.Add(FVector2D(0, 10));

	TArray<FProcMeshTangent> tangents;
	tangents.Add(FProcMeshTangent(0, 1, 0));
	tangents.Add(FProcMeshTangent(0, 1, 0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	TArray<FLinearColor> vertexColors;
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));

	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
}

void AFunctionMesh::GenerateLinearFunction()
{
	// y = ax + b
	
	// Parameters, will expose later 
	float a = 1.f;
	float b = 0.f; 

	int numSegments = 100; 
	for (int i = 0; i < numSegments; i += 1) {
		int stride = 1; 
		float x1 = stride * i;
		float z1 = a * x1 + b; 
		float x2 = stride * (i + 1); 
		float z2 = a * x2 + b; 

		float depth = 10.f; 

		vertices.Add(FVector(x1, 0, z1));
		vertices.Add(FVector(x2, 0, z2));
		vertices.Add(FVector(x1, depth, z1));
		vertices.Add(FVector(x2, depth, z2)); 

		Triangles.Add(0 + 4 * i);
		Triangles.Add(2 + 4 * i);
		Triangles.Add(1 + 4 * i);
		Triangles.Add(1 + 4 * i);
		Triangles.Add(2 + 4 * i);
		Triangles.Add(3 + 4 * i);
	}

	TArray<FVector> normals;

	TArray<FVector2D> UV0;

	TArray<FProcMeshTangent> tangents;

	TArray<FLinearColor> vertexColors;

	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
}

void AFunctionMesh::GenerateSinFunction()
{
	// y = sin(a * x) + b 

	// Parameters, will expose later 
	float a = 3.f;
	float b = 0.5f;

	int numSegments = 100;
	for (int i = 0; i < numSegments; i += 1) {
		float stride = 0.1;
		float x1 = stride * i;
		float z1 = sin(a * x1) + b;
		float x2 = stride * (i + 1);
		float z2 = sin(a * x2) + b;

		float depth = 10.f;

		vertices.Add(FVector(x1, 0, z1));
		vertices.Add(FVector(x2, 0, z2));
		vertices.Add(FVector(x1, depth, z1));
		vertices.Add(FVector(x2, depth, z2));

		Triangles.Add(0 + 4 * i);
		Triangles.Add(2 + 4 * i);
		Triangles.Add(1 + 4 * i);
		Triangles.Add(1 + 4 * i);
		Triangles.Add(2 + 4 * i);
		Triangles.Add(3 + 4 * i);
	}

	TArray<FVector> normals;

	TArray<FVector2D> UV0;

	TArray<FProcMeshTangent> tangents;

	TArray<FLinearColor> vertexColors;

	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
}

void AFunctionMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

