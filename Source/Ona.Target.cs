

using UnrealBuildTool;
using System.Collections.Generic;

public class OnaTarget : TargetRules
{
	public OnaTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Ona" } );
	}
}
