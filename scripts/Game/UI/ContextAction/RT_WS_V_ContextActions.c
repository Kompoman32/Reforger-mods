//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
modded class RT_WS_AddUnitsToCustomSpawnerContextAction : SCR_SelectedEntitiesContextAction
{	
	bool m_bIsVehicleSpawnerAttribute = false;
	
	override bool CanBeShown(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
	{
		RT_WavesSpawnerEntity_Vehicles entity = RT_WavesSpawnerEntity_Vehicles.Cast(selectedEntity.GetOwner());
		
		if (!!entity != !!m_bIsVehicleSpawnerAttribute) return false;

		return super.CanBeShown(selectedEntity, cursorWorldPosition, flags);
	}
};

//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class RT_WS_V_AddUnitsToCustomSpawnerContextAction : RT_WS_AddUnitsToCustomSpawnerContextAction
{
	void RT_WS_V_AddUnitsToCustomSpawnerContextAction()
	{
		m_bIsVehicleSpawnerAttribute = true;	
	}
};