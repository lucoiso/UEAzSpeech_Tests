// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech_Tests

using UnrealBuildTool;
using System.Collections.Generic;

public class UEAzSpeech_TestsEditorTarget : TargetRules
{
	public UEAzSpeech_TestsEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange( new string[] { "UEAzSpeech_Tests" } );
	}
}
