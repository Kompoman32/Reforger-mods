//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class RT_WS_AddUnitsToCustomSpawnerContextAction : SCR_SelectedEntitiesContextAction
{	
	override bool CanBeShown(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
	{
		RT_WavesSpawnerEntity entity = RT_WavesSpawnerEntity.Cast(selectedEntity.GetOwner());
		
		if (!entity) return false;

		return true;
	}
	
	override bool CanBePerformed(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
	{		
		return true;
	}
	
	override void Perform(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition)
	{		
		RT_WavesSpawnerEntity entity = RT_WavesSpawnerEntity.Cast(selectedEntity.GetOwner());
		
		if (!entity) return;
		
		entity.AddNearbyUnits();
	}
};