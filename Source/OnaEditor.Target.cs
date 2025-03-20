

using UnrealBuildTool;
using System.Collections.Generic;

public class OnaEditorTarget : TargetRules
{
	public OnaEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Ona" } );
	}
}
