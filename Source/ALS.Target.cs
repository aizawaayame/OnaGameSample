

using UnrealBuildTool;
using System.Collections.Generic;

public class ALSTarget : TargetRules
{
	public ALSTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "ALS" } );
	}
}
