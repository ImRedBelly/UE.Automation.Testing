// Fill out your copyright notice in the Description page of Project Settings.

#include "Science/ScienceFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogFibonacci, All, All);

int32 UScienceFunctionLibrary::Fibonacci(int32 Value)
{
	if (Value < 0)
	{
		UE_LOG(LogFibonacci, Error, TEXT("Invalid input for Fibonacci: %i"), Value);
	}
	return Value <= 1 ? Value : Fibonacci(Value - 1) + Fibonacci(Value - 2);
}
