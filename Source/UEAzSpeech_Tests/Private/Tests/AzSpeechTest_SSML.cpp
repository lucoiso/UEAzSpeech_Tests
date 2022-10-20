// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech_Tests

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"

#include "AzSpeech/AzSpeechHelper.h"

#include "AzSpeech/SSMLToStreamAsync.h"
#include "AzSpeech/SSMLToVoiceAsync.h"
#include "AzSpeech/SSMLToWavAsync.h"

#if WITH_DEV_AUTOMATION_TESTS

static const FString RawSSML = "<speak version='1.0' xml:lang='en-US'><voice name='en-US-AriaNeural'>Hello World!</voice></speak>";

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSSMLToStreamTest, "AzSpeech.Synthesizer.SSML.SSMLToStream", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)
bool FSSMLToStreamTest::RunTest(const FString& Parameters)
{
	GLog->Log("Running AzSpeech Test: " + GetTestName());

	bool TestResult = true;
	USSMLToStreamAsync* const TestTask = USSMLToStreamAsync::SSMLToStream(nullptr, RawSSML);
	
	// Check if the task is valid
	{
		TestResult = TestResult && TestNotNull(TEXT("The created task that we'll use in all below test cases need to be valid"), TestTask);
	}

	// Check if cached SSML data is equal to the given SSML
	{
		const FString Expected = FString(RawSSML);
		TestResult = TestResult && TestEqual(TEXT("The generated FString needs to contain the given converted SSML content"), TestTask->SSMLString, Expected);
	}

	// Check if the output buffer has data
	{
		const std::string SSMLString_STD = TCHAR_TO_UTF8(*TestTask->SSMLString);
		const std::vector<uint8_t> OutputBuffer = TestTask->DoAzureTaskWork_Internal(SSMLString_STD);
		TestResult = TestResult && TestTrue(TEXT("After the task work, the output buffer can't be empty"), !OutputBuffer.empty());
	}

	return TestResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSSMLToVoiceTest, "AzSpeech.Synthesizer.SSML.SSMLToVoice", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)
bool FSSMLToVoiceTest::RunTest(const FString& Parameters)
{
	GLog->Log("Running AzSpeech Test: " + GetTestName());

	bool TestResult = true;	
	USSMLToVoiceAsync* const TestTask = USSMLToVoiceAsync::SSMLToVoice(nullptr, RawSSML);

	// Check if the task is valid
	{
		TestResult = TestResult && TestNotNull(TEXT("The created task that we'll use in all below test cases need to be valid"), TestTask);
	}

	// Check if cached SSML data is equal to the given SSML
	{
		const FString Expected = FString(RawSSML);
		TestResult = TestResult && TestEqual(TEXT("The generated FString needs to contain the given converted SSML content"), TestTask->SSMLString, Expected);
	}

	// Check if the output buffer has data
	{
		const std::string SSMLString_STD = TCHAR_TO_UTF8(*TestTask->SSMLString);
		const bool OutputResult = TestTask->DoAzureTaskWork_Internal(SSMLString_STD);
		TestResult = TestResult && TestTrue(TEXT("After the task work, the output result can't be false"), !OutputResult);
	}

	return TestResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSSMLToWavTest, "AzSpeech.Synthesizer.SSML.SSMLToWav", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)
bool FSSMLToWavTest::RunTest(const FString& Parameters)
{
	GLog->Log("Running AzSpeech Test: " + GetTestName());

	bool TestResult = true;	
	USSMLToWavAsync* const TestTask = USSMLToWavAsync::SSMLToWav(nullptr, RawSSML, FPaths::AutomationDir(), TEXT("AzSpeech_TestAudioFile"));

	// Check if the task is valid
	{
		TestResult = TestResult && TestNotNull(TEXT("The created task that we'll use in all below test cases need to be valid"), TestTask);

		if (!TestResult)
		{
			return false;
		}
	}

	// Check if cached SSML data is equal to the given SSML
	{
		const FString Expected = FString(RawSSML);
		TestResult = TestResult && TestEqual(TEXT("The generated FString needs to contain the given converted SSML content"), TestTask->SSMLString, Expected);
	}
	
	if (const bool bDirectoryCreated = UAzSpeechHelper::CreateNewDirectory(TestTask->FilePath))
	{
		TestResult = TestResult && TestTrue(TEXT("The directory must exist"), bDirectoryCreated);

		if (!TestResult)
		{
			return false;
		}
	}

	// Generated .wav file path
	const FString FilePath_UE = UAzSpeechHelper::QualifyWAVFileName(TestTask->FilePath, TestTask->FileName);
	
	// Check if the output buffer has data
	{
		const std::string SSMLString_STD = TCHAR_TO_UTF8(*TestTask->SSMLString);
		const std::string FilePath_STD = TCHAR_TO_UTF8(*FilePath_UE);		
		const bool OutputResult = TestTask->DoAzureTaskWork_Internal(SSMLString_STD, FilePath_STD);
		
		TestResult = TestResult && TestTrue(TEXT("After the task work, the output result can't be false"), OutputResult);
	}

	// Check the generated file
	{
		// Check if the generated file exists
		const bool bFileExists = FPaths::FileExists(FilePath_UE);
		TestResult = TestResult && bFileExists;
		
		if (bFileExists)
		{
			// Check the generated file size
			{
				const int64 FileSize = IFileManager::Get().FileSize(*FilePath_UE);
				TestResult = TestResult && TestTrue(TEXT("After the task work, the output file size can't be 0"), FileSize > 0);
			}

			// Delete the generated file
			{
				TestResult = TestResult && TestTrue(TEXT("After the task work, we need to delete the generated file"), IFileManager::Get().Delete(*FilePath_UE));
			}
		}
	}

	return TestResult;
}

#endif //WITH_DEV_AUTOMATION_TESTS