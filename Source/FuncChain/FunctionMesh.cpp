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

	float stride = 1;
	float width = 0.1f;
	float depth = 3.f;
	int skipped = 0;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	for (float i = lowerBound; i < upperBound; i += stride) {
		float x1 = i;
		float x2 = i + stride;

		// f(x)
		float z1 = a * (x1 - c) + d;
		float z2 = a * (x2 - c) + d;

		if (IsOutOfBounds(x1, z1) || IsOutOfBounds(x2, z2)) {
			skipped += 1;
			continue;
		}

		// f'(x) = a
		float dz1 = a;
		float dz2 = a;

		// Get unit vector of tangent
		FVector tangent1 = FVector(1, 0, dz1);
		FVector tangent2 = FVector(1, 0, dz2);
		tangent1.Normalize();
		tangent2.Normalize();

		// Get perpendicular vector
		FVector left = tangent1.RotateAngleAxis(-90, FVector(0, 1, 0));
		FVector right = tangent1.RotateAngleAxis(90, FVector(0, 1, 0));

		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f %f"), x1, z1));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f %f"), x2, z2));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("------------------------")));
		}*/

		FVector p1 = FVector(x1, -depth, z1) + left * width;
		FVector p2 = FVector(x2, -depth, z2) + left * width;
		FVector p3 = FVector(x1, depth, z1) + left * width;
		FVector p4 = FVector(x2, depth, z2) + left * width;
		FVector p5 = FVector(x1, -depth, z1) + right * width;
		FVector p6 = FVector(x2, -depth, z2) + right * width;
		FVector p7 = FVector(x1, depth, z1) + right * width;
		FVector p8 = FVector(x2, depth, z2) + right * width;;

		vertices.Add(p1);
		vertices.Add(p2);
		vertices.Add(p3);
		vertices.Add(p4);
		vertices.Add(p5);
		vertices.Add(p6);
		vertices.Add(p7);
		vertices.Add(p8);

		// Use right hand rule to figure out which side of triangle is visible
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));

		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));

		////Assume normals are the same for all points on one subdivided square
		//FVector n = FVector::CrossProduct(p1, p2).GetSafeNormal();

		//for (int j = 0; j < 4; j++) {
		//	normals.Add(n);
		//}
	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
}

void AFunctionMesh::GenerateQuadraticFunction(int identifier, float a, float b, float c, float d, float lowerBound, float upperBound)
{
	// y = a * (b * (x - c)) ^ 2 + d

	float stride = 0.1;
	float width = 0.1f;
	float depth = 3.f;
	int skipped = 0;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	for (float i = lowerBound; i < upperBound; i += stride) {
		float x1 = i;
		float x2 = i + stride;

		// f(x) = ab^2 * (x^2 - 2cx + c^2) + d
		float z1 = a * pow(b * (x1 - c), 2) + d;
		float z2 = a * pow(b * (x2 - c), 2) + d;

		if (IsOutOfBounds(x1, z1) || IsOutOfBounds(x2, z2)) {
			skipped += 1;
			continue;
		}

		// f'(x) = ab^2 (2x - 2c)
		float dz1 = a * pow(b, 2) * (2 * x1 - 2 * c);
		float dz2 = a * pow(b, 2) * (2 * x2 - 2 * c);

		// Get unit vector of tangent
		FVector tangent1 = FVector(1, 0, dz1);
		FVector tangent2 = FVector(1, 0, dz2);
		tangent1.Normalize();
		tangent2.Normalize();

		// Get perpendicular vector
		FVector left = tangent1.RotateAngleAxis(-90, FVector(0, 1, 0));
		FVector right = tangent1.RotateAngleAxis(90, FVector(0, 1, 0));

		FVector p1 = FVector(x1, -depth, z1) + left * width;
		FVector p2 = FVector(x2, -depth, z2) + left * width;
		FVector p3 = FVector(x1, depth, z1) + left * width;
		FVector p4 = FVector(x2, depth, z2) + left * width;
		FVector p5 = FVector(x1, -depth, z1) + right * width;
		FVector p6 = FVector(x2, -depth, z2) + right * width;
		FVector p7 = FVector(x1, depth, z1) + right * width;
		FVector p8 = FVector(x2, depth, z2) + right * width;;

		vertices.Add(p1);
		vertices.Add(p2);
		vertices.Add(p3);
		vertices.Add(p4);
		vertices.Add(p5);
		vertices.Add(p6);
		vertices.Add(p7);
		vertices.Add(p8);

		// Use right hand rule to figure out which side of triangle is visible
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));

		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));

		////Assume normals are the same for all points on one subdivided square
		//FVector n = FVector::CrossProduct(p1, p2).GetSafeNormal();

		//for (int j = 0; j < 4; j++) {
		//	normals.Add(n);
		//}
	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
}

void AFunctionMesh::GenerateExponentialFunction(int identifier, float a, float b, float c, float d, float lowerBound, float upperBound)
{
	// y = a * e ^ (b * (x - c)) + d

	float stride = 0.1;
	float width = 0.1f;
	float depth = 3.f;
	int skipped = 0;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	for (float i = lowerBound; i < upperBound; i += stride) {
		float x1 = i;
		float x2 = i + stride;

		// f(x)
		float z1 = a * exp(b * (x1 - c)) + d;
		float z2 = a * exp(b * (x2 - c)) + d;

		if (IsOutOfBounds(x1, z1) || IsOutOfBounds(x2, z2)) {
			skipped += 1;
			continue;
		}

		// f'(x) = ab * e^{b * (x-c)}
		float dz1 = a * b * exp(b * (x1 - c));
		float dz2 = a * b * exp(b * (x2 - c));

		// Get unit vector of tangent
		FVector tangent1 = FVector(1, 0, dz1);
		FVector tangent2 = FVector(1, 0, dz2);
		tangent1.Normalize();
		tangent2.Normalize();

		// Get perpendicular vector
		FVector left = tangent1.RotateAngleAxis(-90, FVector(0, 1, 0));
		FVector right = tangent1.RotateAngleAxis(90, FVector(0, 1, 0));

		FVector p1 = FVector(x1, -depth, z1) + left * width;
		FVector p2 = FVector(x2, -depth, z2) + left * width;
		FVector p3 = FVector(x1, depth, z1) + left * width;
		FVector p4 = FVector(x2, depth, z2) + left * width;
		FVector p5 = FVector(x1, -depth, z1) + right * width;
		FVector p6 = FVector(x2, -depth, z2) + right * width;
		FVector p7 = FVector(x1, depth, z1) + right * width;
		FVector p8 = FVector(x2, depth, z2) + right * width;;

		vertices.Add(p1);
		vertices.Add(p2);
		vertices.Add(p3);
		vertices.Add(p4);
		vertices.Add(p5);
		vertices.Add(p6);
		vertices.Add(p7);
		vertices.Add(p8);

		// Use right hand rule to figure out which side of triangle is visible
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));

		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));

		////Assume normals are the same for all points on one subdivided square
		//FVector n = FVector::CrossProduct(p1, p2).GetSafeNormal();

		//for (int j = 0; j < 4; j++) {
		//	normals.Add(n);
		//}
	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
}

void AFunctionMesh::GenerateLogarithmicFunction(int identifier, float a, float b, float c, float d, float lowerBound, float upperBound)
{
	// y = a * ln (b * (x - c)) + d

	float stride = 0.1;
	float width = 0.1f;
	float depth = 3.f;
	int skipped = 0;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	// Makes sure bounds are in range of domain
	int direction = b >= 0 ? 1 : -1;
	float adjustedLowerBound = lowerBound;
	if (direction == 1) {
		adjustedLowerBound = std::max(lowerBound, c + stride);
	}
	float adjustedUpperBound = upperBound;
	if (direction == -1) {
		adjustedUpperBound = std::min(upperBound, c - stride);
	}

	for (float i = adjustedLowerBound; i < adjustedUpperBound; i += stride) {
		float x1 = i;
		float x2 = i + stride;

		// f(x)
		float z1 = a * log(b * (x1 - c)) + d;
		float z2 = a * log(b * (x2 - c)) + d;

		if (IsOutOfBounds(x1, z1) || IsOutOfBounds(x2, z2)) {
			skipped += 1;
			continue;
		}

		// f'(x) = a / (x - c)
		float dz1 = a / (x1 - c);
		float dz2 = a / (x2 - c);

		// Get unit vector of tangent
		FVector tangent1 = FVector(1, 0, dz1);
		FVector tangent2 = FVector(1, 0, dz2);
		tangent1.Normalize();
		tangent2.Normalize();

		// Get perpendicular vector
		FVector left = tangent1.RotateAngleAxis(-90, FVector(0, 1, 0));
		FVector right = tangent1.RotateAngleAxis(90, FVector(0, 1, 0));

		FVector p1 = FVector(x1, -depth, z1) + left * width;
		FVector p2 = FVector(x2, -depth, z2) + left * width;
		FVector p3 = FVector(x1, depth, z1) + left * width;
		FVector p4 = FVector(x2, depth, z2) + left * width;
		FVector p5 = FVector(x1, -depth, z1) + right * width;
		FVector p6 = FVector(x2, -depth, z2) + right * width;
		FVector p7 = FVector(x1, depth, z1) + right * width;
		FVector p8 = FVector(x2, depth, z2) + right * width;;

		vertices.Add(p1);
		vertices.Add(p2);
		vertices.Add(p3);
		vertices.Add(p4);
		vertices.Add(p5);
		vertices.Add(p6);
		vertices.Add(p7);
		vertices.Add(p8);

		// Use right hand rule to figure out which side of triangle is visible
		Triangles.Add(0 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));

		Triangles.Add(1 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - adjustedLowerBound) / stride - skipped));

		////Assume normals are the same for all points on one subdivided square
		//FVector n = FVector::CrossProduct(p1, p2).GetSafeNormal();

		//for (int j = 0; j < 4; j++) {
		//	normals.Add(n);
		//}
	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
}

void AFunctionMesh::GenerateSineFunction(int identifier, float a, float b, float c, float d, float lowerBound, float upperBound)
{
	// y = a * sin(b * (x - c)) + d

	float stride = 0.1;
	float width = 0.1f;
	float depth = 3.f;
	int skipped = 0;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	for (float i = lowerBound; i < upperBound; i += stride) {
		float x1 = i;
		float x2 = i + stride;

		// f(x)
		float z1 = a * sin(b * (x1 - c)) + d;
		float z2 = a * sin(b * (x2 - c)) + d;

		if (IsOutOfBounds(x1, z1) || IsOutOfBounds(x2, z2)) {
			skipped += 1;
			continue;
		}

		// f'(x) = ab * cos(b * (x-c))
		float dz1 = a * b * cos(b * (x1 - c));
		float dz2 = a * b * cos(b * (x2 - c));

		// Get unit vector of tangent
		FVector tangent1 = FVector(1, 0, dz1);
		FVector tangent2 = FVector(1, 0, dz2);
		tangent1.Normalize();
		tangent2.Normalize();

		// Get perpendicular vector
		FVector left = tangent1.RotateAngleAxis(-90, FVector(0, 1, 0));
		FVector right = tangent1.RotateAngleAxis(90, FVector(0, 1, 0));

		FVector p1 = FVector(x1, -depth, z1) + left * width;
		FVector p2 = FVector(x2, -depth, z2) + left * width;
		FVector p3 = FVector(x1, depth, z1) + left * width;
		FVector p4 = FVector(x2, depth, z2) + left * width;
		FVector p5 = FVector(x1, -depth, z1) + right * width;
		FVector p6 = FVector(x2, -depth, z2) + right * width;
		FVector p7 = FVector(x1, depth, z1) + right * width;
		FVector p8 = FVector(x2, depth, z2) + right * width;;

		vertices.Add(p1);
		vertices.Add(p2);
		vertices.Add(p3);
		vertices.Add(p4);
		vertices.Add(p5);
		vertices.Add(p6);
		vertices.Add(p7);
		vertices.Add(p8);

		// Use right hand rule to figure out which side of triangle is visible
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));

		Triangles.Add(1 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(5 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(3 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(7 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(6 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(4 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(2 + 8 * std::round((i - lowerBound) / stride - skipped));
		Triangles.Add(0 + 8 * std::round((i - lowerBound) / stride - skipped));

		/*FVector n = FVector::CrossProduct(p1, p2).GetSafeNormal();

		for (int j = 0; j < 4; j++) {
			normals.Add(n);
		}*/
	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
}

bool AFunctionMesh::IsOutOfBounds(float x, float y) {
	return x > upperX || y > upperY || x < lowerX || y < lowerY;
}

void AFunctionMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
