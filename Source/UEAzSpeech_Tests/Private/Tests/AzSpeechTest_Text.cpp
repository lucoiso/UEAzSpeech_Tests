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

#include "AzSpeech/TextToStreamAsync.h"
#include "AzSpeech/TextToVoiceAsync.h"
#include "AzSpeech/TextToWavAsync.h"

#if WITH_DEV_AUTOMATION_TESTS

static const FString TextToConvert = "Hello World!";
static const FString DefaultLanguage = "en-US";
static const FString DefaultVoice = "en-US-AriaNeural";

// static const FString DefaultLanguage = "en-US";
// static const FString DefaultVoice = "en-US-AriaNeural";

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTextToStreamDefaultTest, "AzSpeechTests.Synthesizer.Default.TextToStream", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)
bool FTextToStreamDefaultTest::RunTest(const FString& Parameters)
{
	GLog->Log("Running AzSpeech Test: " + GetTestName());

	bool TestResult = true; 
	UTextToStreamAsync* const TestTask = UTextToStreamAsync::TextToStream(nullptr, TextToConvert);

	// Check if the task is valid
	{
		TestResult = TestResult && TestNotNull(TEXT("The created task that we'll use in all below test cases need to be valid"), TestTask);
	}

	// Check if cached SSML data is equal to the given Text
	{
		const FString Expected = FString(TextToConvert);
		TestResult = TestResult && TestEqual(TEXT("The cached text needs to be equal to what we passed on construction"), TestTask->TextToConvert, Expected);
	}

	// Check if cached LanguageId is equal to what we passed
	{
		TestResult = TestResult && TestEqual(TEXT("The cached language needs to be equal to what we passed on construction"), TestTask->LanguageID, DefaultLanguage);
	}

	// Check if cached Voice Name is equal to what we passed
	{
		TestResult = TestResult && TestEqual(TEXT("The cached voice needs to be equal to what we passed on construction"), TestTask->VoiceName, DefaultVoice);
	}

	// Check if the output buffer has data
	{
		const std::string TextToConvert_STD = TCHAR_TO_UTF8(*TestTask->TextToConvert);
		const std::string Language_STD = TCHAR_TO_UTF8(*TestTask->LanguageID);
		const std::string Voice_STD = TCHAR_TO_UTF8(*TestTask->VoiceName);
		
		const std::vector<uint8_t> OutputBuffer = TestTask->DoAzureTaskWork_Internal(TextToConvert_STD, Language_STD, Voice_STD);
		TestResult = TestResult && TestTrue(TEXT("After the task work, the output buffer can't be empty"), !OutputBuffer.empty());
	}

	return TestResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTextToStreamAutoTest, "AzSpeechTests.Synthesizer.Auto.TextToStream", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)
bool FTextToStreamAutoTest::RunTest(const FString& Parameters)
{
	GLog->Log("Running AzSpeech Test: " + GetTestName());

	bool TestResult = true;
	UTextToStreamAsync* const TestTask = UTextToStreamAsync::TextToStream(nullptr, TextToConvert, "Auto", FString());

	// Check if the task is valid
	{
		TestResult = TestResult && TestNotNull(TEXT("The created task that we'll use in all below test cases need to be valid"), TestTask);
	}

	// Check if cached SSML data is equal to the given Text
	{
		const FString Expected = FString(TextToConvert);
		TestResult = TestResult && TestEqual(TEXT("The cached text needs to be equal to what we passed on construction"), TestTask->TextToConvert, Expected);
	}

	// Check if cached LanguageId is equal to what we passed
	{
		TestResult = TestResult && TestEqual(TEXT("The cached language needs to be equal to Auto"), TestTask->LanguageID, "Auto");
	}

	// Check if the output buffer has data
	{
		const std::string TextToConvert_STD = TCHAR_TO_UTF8(*TestTask->TextToConvert);
		const std::string Language_STD = TCHAR_TO_UTF8(*TestTask->LanguageID);

		const std::vector<uint8_t> OutputBuffer = TestTask->DoAzureTaskWork_Internal(TextToConvert_STD, Language_STD, std::string());
		TestResult = TestResult && TestTrue(TEXT("After the task work, the output buffer can't be empty"), !OutputBuffer.empty());
	}

	return TestResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTextToVoiceDefaultTest, "AzSpeechTests.Synthesizer.Default.TextToVoice", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)
bool FTextToVoiceDefaultTest::RunTest(const FString& Parameters)
{
	GLog->Log("Running AzSpeech Test: " + GetTestName());

	bool TestResult = true;
	UTextToVoiceAsync* const TestTask = UTextToVoiceAsync::TextToVoice(nullptr, TextToConvert);

	// Check if the task is valid
	{
		TestResult = TestResult && TestNotNull(TEXT("The created task that we'll use in all below test cases need to be valid"), TestTask);
	}

	// Check if cached SSML data is equal to the given Text
	{
		const FString Expected = FString(TextToConvert);
		TestResult = TestResult && TestEqual(TEXT("The cached text needs to be equal to what we passed on construction"), TestTask->TextToConvert, Expected);
	}

	// Check if cached LanguageId is equal to what we passed
	{
		TestResult = TestResult && TestEqual(TEXT("The cached language needs to be equal to what we passed on construction"), TestTask->LanguageID, DefaultLanguage);
	}

	// Check if cached Voice Name is equal to what we passed
	{
		TestResult = TestResult && TestEqual(TEXT("The cached voice needs to be equal to what we passed on construction"), TestTask->VoiceName, DefaultVoice);
	}

	// Check if the output result is valid
	{
		const std::string TextToConvert_STD = TCHAR_TO_UTF8(*TestTask->TextToConvert);
		const std::string Language_STD = TCHAR_TO_UTF8(*TestTask->LanguageID);
		const std::string Voice_STD = TCHAR_TO_UTF8(*TestTask->VoiceName);

		const bool OutputResult = TestTask->DoAzureTaskWork_Internal(TextToConvert_STD, Language_STD, Voice_STD);
		TestResult = TestResult && TestTrue(TEXT("After the task work, the output result can't be false"), !OutputResult);
	}

	return TestResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTextToVoiceAutoTest, "AzSpeechTests.Synthesizer.Auto.TextToVoice", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)
bool FTextToVoiceAutoTest::RunTest(const FString& Parameters)
{
	GLog->Log("Running AzSpeech Test: " + GetTestName());

	bool TestResult = true;
	UTextToVoiceAsync* const TestTask = UTextToVoiceAsync::TextToVoice(nullptr, TextToConvert, "Auto", FString());

	// Check if the task is valid
	{
		TestResult = TestResult && TestNotNull(TEXT("The created task that we'll use in all below test cases need to be valid"), TestTask);
	}

	// Check if cached SSML data is equal to the given Text
	{
		const FString Expected = FString(TextToConvert);
		TestResult = TestResult && TestEqual(TEXT("The cached text needs to be equal to what we passed on construction"), TestTask->TextToConvert, Expected);
	}

	// Check if cached LanguageId is equal to what we passed
	{
		TestResult = TestResult && TestEqual(TEXT("The cached language needs to be equal to Auto"), TestTask->LanguageID, "Auto");
	}

	// Check if the output result is valid
	{
		const std::string TextToConvert_STD = TCHAR_TO_UTF8(*TestTask->TextToConvert);
		const std::string Language_STD = TCHAR_TO_UTF8(*TestTask->LanguageID);

		const bool OutputResult = TestTask->DoAzureTaskWork_Internal(TextToConvert_STD, Language_STD, std::string());
		TestResult = TestResult && TestTrue(TEXT("After the task work, the output result can't be false"), !OutputResult);
	}

	return TestResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTextToWavDefaultTest, "AzSpeechTests.Synthesizer.Default.TextToWav", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)
bool FTextToWavDefaultTest::RunTest(const FString& Parameters)
{
	GLog->Log("Running AzSpeech Test: " + GetTestName());

	bool TestResult = true;
	UTextToWavAsync* const TestTask = UTextToWavAsync::TextToWav(nullptr, TextToConvert, FPaths::AutomationDir(), TEXT("AzSpeech_TestAudioFile"));

	// Check if the task is valid
	{
		TestResult = TestResult && TestNotNull(TEXT("The created task that we'll use in all below test cases need to be valid"), TestTask);

		if (!TestResult)
		{
			return false;
		}
	}

	// Check if cached SSML data is equal to the given Text
	{
		const FString Expected = FString(TextToConvert);
		TestResult = TestResult && TestEqual(TEXT("The cached text needs to be equal to what we passed on construction"), TestTask->TextToConvert, Expected);
	}

	// Check if cached LanguageId is equal to what we passed
	{
		TestResult = TestResult && TestEqual(TEXT("The cached language needs to be equal to what we passed on construction"), TestTask->LanguageID, DefaultLanguage);
	}

	// Check if cached Voice Name is equal to what we passed
	{
		TestResult = TestResult && TestEqual(TEXT("The cached voice needs to be equal to what we passed on construction"), TestTask->VoiceName, DefaultVoice);
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
		const std::string TextToConvert_STD = TCHAR_TO_UTF8(*TestTask->TextToConvert);
		const std::string Language_STD = TCHAR_TO_UTF8(*TestTask->LanguageID);
		const std::string Voice_STD = TCHAR_TO_UTF8(*TestTask->VoiceName);
		const std::string FilePath_STD = TCHAR_TO_UTF8(*FilePath_UE);
		
		const bool OutputResult = TestTask->DoAzureTaskWork_Internal(TextToConvert_STD, Language_STD, Voice_STD, FilePath_STD);

		TestResult = TestResult && TestTrue(TEXT("After the task work, the output result can't be false"), !OutputResult);
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTextToWavAutoTest, "AzSpeechTests.Synthesizer.Auto.TextToWav", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)
bool FTextToWavAutoTest::RunTest(const FString& Parameters)
{
	GLog->Log("Running AzSpeech Test: " + GetTestName());

	bool TestResult = true;
	UTextToWavAsync* const TestTask = UTextToWavAsync::TextToWav(nullptr, TextToConvert, FPaths::AutomationDir(), TEXT("AzSpeech_TestAudioFile"), "Auto", FString());

	// Check if the task is valid
	{
		TestResult = TestResult && TestNotNull(TEXT("The created task that we'll use in all below test cases need to be valid"), TestTask);
	}

	// Check if cached SSML data is equal to the given Text
	{
		const FString Expected = FString(TextToConvert);
		TestResult = TestResult && TestEqual(TEXT("The cached text needs to be equal to what we passed on construction"), TestTask->TextToConvert, Expected);
	}

	// Check if cached LanguageId is equal to what we passed
	{
		TestResult = TestResult && TestEqual(TEXT("The cached language needs to be equal to Auto"), TestTask->LanguageID, "Auto");
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
		const std::string TextToConvert_STD = TCHAR_TO_UTF8(*TestTask->TextToConvert);
		const std::string Language_STD = TCHAR_TO_UTF8(*TestTask->LanguageID);		
		const std::string FilePath_STD = TCHAR_TO_UTF8(*FilePath_UE);
		
		const bool OutputResult = TestTask->DoAzureTaskWork_Internal(TextToConvert_STD, Language_STD, std::string(), FilePath_STD);
		
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