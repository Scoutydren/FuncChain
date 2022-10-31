// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionMesh.h"
#include "ProceduralMeshComponent.h"

AFunctionMesh::AFunctionMesh()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	ProceduralMesh->SetupAttachment(GetRootComponent());
}

void AFunctionMesh::Generate(FunctionType functionType, int identifier, float a, float b, float c, float d, float lowerBound, float upperBound)
{
	switch (functionType)
	{
	case FunctionType::LINEAR:
		GenerateLinearFunction(identifier, a, b, c, d, lowerBound, upperBound);
		break;
	case FunctionType::SINE:
		break;
	default:
		break;
	}
	//GenerateTestTriangle(); 
	//GenerateLinearFunction(); 
	//GenerateSinFunction(); 
}

void AFunctionMesh::Delete(int identifier)
{

}

void AFunctionMesh::BeginPlay()
{
	Super::BeginPlay();

	//GenerateTestTriangle(); 
	//GenerateLinearFunction(); 
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

void AFunctionMesh::GenerateLinearFunction(int identifier, float a, float b, float c, float d, float lowerBound, float upperBound)
{
	// y = a(x - c) + d
	// This is the function we want to use instead
	// For sine, we want to use y = asin(b * x - c) + d

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f %f"), lowerBound, upperBound));
	}

	for (float i = lowerBound; i < upperBound + 1; i += 1) {
		int stride = 1; 
		float x1 = stride * i;
		float z1 = a * (x1 - c) + d;
		float x2 = stride * (i + 1);
		float z2 = a * (x2 - c) + d;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f %f"), x1, z1));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f %f"), x2, z2));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("------------------------")));
		}

		float depth = 5.f; 

		// I made it -depth and +depth since the ball is on y=0
		vertices.Add(FVector(x1, -depth, z1));
		vertices.Add(FVector(x2, -depth, z2));
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

	// How do you update/remove a function? I thought it could be taken care of by indentifier but it didn't work for me
	ProceduralMesh->CreateMeshSection_LinearColor(identifier, vertices, Triangles, normals, UV0, vertexColors, tangents, true);

	/*
	// Parameters, will expose later 
	float e = 1.f;
	float f = 0.f;

	int numSegments = 100;
	for (int i = 0; i < numSegments; i += 1) {
		int stride = 1;
		float x1 = stride * i;
		float z1 = e * x1 + f;
		float x2 = stride * (i + 1);
		float z2 = e * x2 + f;

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

	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);*/
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

