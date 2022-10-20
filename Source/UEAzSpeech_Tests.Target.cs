// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech_Tests

using UnrealBuildTool;
using System.Collections.Generic;

public class UEAzSpeech_TestsTarget : TargetRules
{
	public UEAzSpeech_TestsTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "UEAzSpeech_Tests" } );
	}
}
