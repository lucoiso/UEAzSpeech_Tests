// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech_Tests

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"

#include "AzSpeech/AzSpeechHelper.h"

#include "AzSpeech/VoiceToTextAsync.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVoiceToTextDefaultTest, "AzSpeech.Recognizer.Default.VoiceToText", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)
bool FVoiceToTextDefaultTest::RunTest(const FString& Parameters)
{
	GLog->Log("Running AzSpeech Test: " + GetTestName());

	bool TestResult = true;
	UVoiceToTextAsync* const TestTask = UVoiceToTextAsync::VoiceToText(nullptr);
	
	// Check if the task is valid
	{
		TestResult = TestResult && TestNotNull(TEXT("The created task that we'll use in all below test cases need to be valid"), TestTask);

		if (!TestResult)
		{
			return false;
		}
	}

	// To do
	return TestResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVoiceToTextAutoTest, "AzSpeech.Recognizer.Auto.VoiceToText", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)
bool FVoiceToTextAutoTest::RunTest(const FString& Parameters)
{
	GLog->Log("Running AzSpeech Test: " + GetTestName());

	bool TestResult = true;
	UVoiceToTextAsync* const TestTask = UVoiceToTextAsync::VoiceToText(nullptr, "Auto");

	// Check if the task is valid
	{
		TestResult = TestResult && TestNotNull(TEXT("The created task that we'll use in all below test cases need to be valid"), TestTask);

		if (!TestResult)
		{
			return false;
		}
	}
	
	// To do
	return TestResult;
}

#endif //WITH_DEV_AUTOMATION_TESTS