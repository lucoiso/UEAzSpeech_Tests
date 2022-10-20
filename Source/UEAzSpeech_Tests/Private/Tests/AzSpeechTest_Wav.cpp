// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech_Tests

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"

#include "AzSpeech/AzSpeechHelper.h"
#include "AzSpeech/AzSpeechSettings.h"

#include "AzSpeech/WavToTextAsync.h"
#include "AzSpeech/TextToWavAsync.h"

#if WITH_DEV_AUTOMATION_TESTS

static const FString DefaultLanguage = "en-US";
static const FString DefaultVoice = "en-US-AriaNeural";
static const FString TextToConvert = "Hello World";

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWavToTextDefaultTest, "AzSpeech.Recognizer.Default.WavToText", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)
bool FWavToTextDefaultTest::RunTest(const FString& Parameters)
{
	GLog->Log("Running AzSpeech Test: " + GetTestName());

	bool TestResult = true;
	UWavToTextAsync* const TestTask = UWavToTextAsync::WavToText(nullptr, FPaths::AutomationDir(), TEXT("AzSpeech_TestAudioFile"));
	
	// Check if the task is valid
	{
		TestResult = TestResult && TestNotNull(TEXT("The created task that we'll use in all below test cases need to be valid"), TestTask);
	}

	// Check if cached LanguageId is equal to what we passed
	{
		TestResult = TestResult && TestEqual(TEXT("The cached language needs to be equal to what we passed on construction"), TestTask->LanguageID, DefaultLanguage);
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

	// Generate the .wav file (using another azspeech task)
	{
		UTextToWavAsync* const WavTask = UTextToWavAsync::TextToWav(nullptr, TextToConvert, FPaths::AutomationDir(), TEXT("AzSpeech_TestAudioFile"));

		const std::string TextToConvert_STD = TCHAR_TO_UTF8(*TextToConvert);
		const std::string Language_STD = TCHAR_TO_UTF8(*TestTask->LanguageID);
		const std::string Voice_STD = TCHAR_TO_UTF8(*DefaultVoice);
		const std::string FilePath_STD = TCHAR_TO_UTF8(*FilePath_UE);
		
		WavTask->DoAzureTaskWork_Internal(TextToConvert_STD, Language_STD, std::string(), FilePath_STD);
	}

	// Check if the generated file exists
	const bool bFileExists = FPaths::FileExists(FilePath_UE);
	
	// Check the generated file
	{
		TestResult = TestResult && bFileExists;
		if (bFileExists)
		{
			// Check the generated file size
			{
				const int64 FileSize = IFileManager::Get().FileSize(*FilePath_UE);
				TestResult = TestResult && TestTrue(TEXT("After the task work, the output file size can't be 0"), FileSize > 0);
			}
		}
	}

	// Check if the output result has data
	{
		const std::string FilePath_STD = TCHAR_TO_UTF8(*FilePath_UE);
		const std::string Language_STD = TCHAR_TO_UTF8(*TestTask->LanguageID);
		
		const std::string OutputResult = TestTask->DoAzureTaskWork_Internal(FilePath_STD, Language_STD);

		TestResult = TestResult && TestTrue(TEXT("After the task work, the output result can't be empty"), !OutputResult.empty());

		const FString OutputResult_UE = UTF8_TO_TCHAR(OutputResult.c_str());
		TestResult = TestResult && TestEqual(TEXT("After the task work, the output result needs to be equal to the text used to generate the wav file"), OutputResult_UE, TextToConvert);
	}

	if (bFileExists)
	{
		// Delete the generated file
		{
			TestResult = TestResult && TestTrue(TEXT("After the task work, we need to delete the generated file"), IFileManager::Get().Delete(*FilePath_UE));
		}
	}

	return TestResult;
}

#endif //WITH_DEV_AUTOMATION_TESTS