// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionMesh.h"
#include "ProceduralMeshComponent.h"

#include <cmath>

AFunctionMesh::AFunctionMesh()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	ProceduralMesh->SetupAttachment(GetRootComponent());
}

void AFunctionMesh::Generate(FunctionType functionType, int identifier, float a, float b, float c, float d, float lowerBound, float upperBound)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), identifier));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), ProceduralMesh->GetNumSections()));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), ProceduralMesh->GetNumSections()));*/

	switch (functionType)
	{
	case FunctionType::LINEAR:
		GenerateLinearFunction(identifier, a, b, c, d, lowerBound, upperBound);
		break;
	case FunctionType::QUADRATIC:
		GenerateQuadraticFunction(identifier, a, b, c, d, lowerBound, upperBound);
		break;
	case FunctionType::EXPONENTIAL:
		GenerateExponentialFunction(identifier, a, b, c, d, lowerBound, upperBound);
		break;
	case FunctionType::LOGARITHMIC:
		GenerateLogarithmicFunction(identifier, a, b, c, d, lowerBound, upperBound);
		break;
	case FunctionType::SINE:
		GenerateSineFunction(identifier, a, b, c, d, lowerBound, upperBound);
		break;
	default:
		break;
	}
	//GenerateTestTriangle(); 
	//GenerateLinearFunction(); 
	//GenerateSinFunction(); 
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

	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f %f"), lowerBound, upperBound));
	}*/

	float stride = 1;
	float depth = 5.f;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	for (float i = lowerBound; i < upperBound + 1; i += stride) {
		float x1 = i;
		float z1 = a * (x1 - c) + d;
		float x2 = i + 1;
		float z2 = a * (x2 - c) + d;

		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f %f"), x1, z1));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f %f"), x2, z2));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("------------------------")));
		}*/


		// I made it -depth and +depth since the ball is on y=0
		FVector p1 = FVector(x1, -depth, z1);
		FVector p2 = FVector(x2, -depth, z2);
		FVector p3 = FVector(x1, depth, z1);
		FVector p4 = FVector(x2, depth, z2);

		vertices.Add(p1);
		vertices.Add(p2);
		vertices.Add(p3);
		vertices.Add(p4); 

		Triangles.Add(0 + 4 * (i - lowerBound) / stride);
		Triangles.Add(2 + 4 * (i - lowerBound) / stride);
		Triangles.Add(1 + 4 * (i - lowerBound) / stride);
		Triangles.Add(1 + 4 * (i - lowerBound) / stride);
		Triangles.Add(2 + 4 * (i - lowerBound) / stride);
		Triangles.Add(3 + 4 * (i - lowerBound) / stride);

		//Assume normals are the same for all points on one subdivided square
		FVector n = FVector::CrossProduct(p1, p2).GetSafeNormal();

		for (int j = 0; j < 4; j++) {
			normals.Add(n);
		}

		vertexColors.Add(FLinearColor(0, 0, 0, 1.0));
		vertexColors.Add(FLinearColor(0, 0, 0, 1.0));
		vertexColors.Add(FLinearColor(0, 0, 0, 1.0));
		vertexColors.Add(FLinearColor(0, 0, 0, 1.0));
	}

	// How do you update/remove a function? I thought it could be taken care of by indentifier but it didn't work for me
	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
	
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

void AFunctionMesh::GenerateQuadraticFunction(int identifier, float a, float b, float c, float d, float lowerBound, float upperBound)
{
	// y = a * (b * (x - c)) ^ 2 + d

	float stride = 0.1;
	float depth = 5.f;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	for (float i = lowerBound; i < upperBound + 1; i += stride) {
		float x1 = i;
		float z1 = a * pow(b * (x1 - c), 2) + d;
		float x2 = i + 1;
		float z2 = a * pow(b * (x2 - c), 2) + d;

		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f %f"), x1, z1));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f %f"), x2, z2));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("------------------------")));
		}*/

		FVector p1 = FVector(x1, -depth, z1);
		FVector p2 = FVector(x2, -depth, z2);
		FVector p3 = FVector(x1, depth, z1);
		FVector p4 = FVector(x2, depth, z2);

		vertices.Add(p1);
		vertices.Add(p2);
		vertices.Add(p3);
		vertices.Add(p4);

		Triangles.Add(0 + 4 * (i - lowerBound) / stride);
		Triangles.Add(2 + 4 * (i - lowerBound) / stride);
		Triangles.Add(1 + 4 * (i - lowerBound) / stride);
		Triangles.Add(1 + 4 * (i - lowerBound) / stride);
		Triangles.Add(2 + 4 * (i - lowerBound) / stride);
		Triangles.Add(3 + 4 * (i - lowerBound) / stride);

		//Assume normals are the same for all points on one subdivided square
		FVector n = FVector::CrossProduct(p1, p2).GetSafeNormal();

		for (int j = 0; j < 4; j++) {
			normals.Add(n);
		}

	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
}

void AFunctionMesh::GenerateExponentialFunction(int identifier, float a, float b, float c, float d, float lowerBound, float upperBound)
{
	// y = a * e ^ (b * (x - c)) + d

	float stride = 0.1;
	float depth = 5.f;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	for (float i = lowerBound; i < upperBound + 1; i += stride) {
		float x1 = i;
		float z1 = a * exp(b * (x1 - c)) + d;
		float x2 = i + 1;
		float z2 = a * exp(b * (x2 - c)) + d;

		FVector p1 = FVector(x1, -depth, z1);
		FVector p2 = FVector(x2, -depth, z2);
		FVector p3 = FVector(x1, depth, z1);
		FVector p4 = FVector(x2, depth, z2);

		vertices.Add(p1);
		vertices.Add(p2);
		vertices.Add(p3);
		vertices.Add(p4);

		Triangles.Add(0 + 4 * (i - lowerBound) / stride);
		Triangles.Add(2 + 4 * (i - lowerBound) / stride);
		Triangles.Add(1 + 4 * (i - lowerBound) / stride);
		Triangles.Add(1 + 4 * (i - lowerBound) / stride);
		Triangles.Add(2 + 4 * (i - lowerBound) / stride);
		Triangles.Add(3 + 4 * (i - lowerBound) / stride);

		//Assume normals are the same for all points on one subdivided square
		FVector n = FVector::CrossProduct(p1, p2).GetSafeNormal();

		for (int j = 0; j < 4; j++) {
			normals.Add(n);
		}

	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
}

void AFunctionMesh::GenerateLogarithmicFunction(int identifier, float a, float b, float c, float d, float lowerBound, float upperBound)
{
	// y = a * e ^ (b * (x - c)) + d

	float stride = 0.1;
	float depth = 5.f;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	// Makes sure bounds are in range of domain
	float adjustedLowerBound = std::max(lowerBound, c + stride);

	for (float i = adjustedLowerBound; i < upperBound + 1; i += stride) {
		float x1 = i;
		float z1 = a * log(b * (x1 - c)) + d;
		float x2 = i + 1;
		float z2 = a * log(b * (x2 - c)) + d;

		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f %f"), x1, z1));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f %f"), x2, z2));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("------------------------")));
		}*/

		FVector p1 = FVector(x1, -depth, z1);
		FVector p2 = FVector(x2, -depth, z2);
		FVector p3 = FVector(x1, depth, z1);
		FVector p4 = FVector(x2, depth, z2);

		vertices.Add(p1);
		vertices.Add(p2);
		vertices.Add(p3);
		vertices.Add(p4);

		Triangles.Add(0 + 4 * (i - adjustedLowerBound) / stride);
		Triangles.Add(2 + 4 * (i - adjustedLowerBound) / stride);
		Triangles.Add(1 + 4 * (i - adjustedLowerBound) / stride);
		Triangles.Add(1 + 4 * (i - adjustedLowerBound) / stride);
		Triangles.Add(2 + 4 * (i - adjustedLowerBound) / stride);
		Triangles.Add(3 + 4 * (i - adjustedLowerBound) / stride);

		//Assume normals are the same for all points on one subdivided square
		FVector n = FVector::CrossProduct(p1, p2).GetSafeNormal();

		for (int j = 0; j < 4; j++) {
			normals.Add(n);
		}
	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
}

void AFunctionMesh::GenerateSineFunction(int identifier, float a, float b, float c, float d, float lowerBound, float upperBound)
{
	// y = a * sin(b * (x - c)) + d

	float stride = 0.1;
	float depth = 5.f;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	for (float i = lowerBound; i < upperBound + 1; i += stride) {
		float x1 = i;
		float z1 = a * sin(b * (x1 - c)) + d;
		float x2 = i + 1;
		float z2 = a * sin(b * (x2 - c)) + d;

		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f %f"), x1, z1));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f %f"), x2, z2));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("------------------------")));
		}*/

		FVector p1 = FVector(x1, -depth, z1);
		FVector p2 = FVector(x2, -depth, z2);
		FVector p3 = FVector(x1, depth, z1);
		FVector p4 = FVector(x2, depth, z2);

		vertices.Add(p1);
		vertices.Add(p2);
		vertices.Add(p3);
		vertices.Add(p4);

		Triangles.Add(0 + 4 * (i - lowerBound) / stride);
		Triangles.Add(2 + 4 * (i - lowerBound) / stride);
		Triangles.Add(1 + 4 * (i - lowerBound) / stride);
		Triangles.Add(1 + 4 * (i - lowerBound) / stride);
		Triangles.Add(2 + 4 * (i - lowerBound) / stride);
		Triangles.Add(3 + 4 * (i - lowerBound) / stride);

		FVector n = FVector::CrossProduct(p1, p2).GetSafeNormal();

		for (int j = 0; j < 4; j++) {
			normals.Add(n);
		}
	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
}

void AFunctionMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

