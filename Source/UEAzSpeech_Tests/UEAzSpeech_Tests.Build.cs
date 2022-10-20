// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech_Tests

using UnrealBuildTool;

public class UEAzSpeech_Tests : ModuleRules
{
	public UEAzSpeech_Tests(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"UnrealEd",
				"AzSpeech"
			});
	}
}
