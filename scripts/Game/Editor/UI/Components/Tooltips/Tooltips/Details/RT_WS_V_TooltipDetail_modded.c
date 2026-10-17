[BaseContainerProps(), BaseContainerCustomTitleField("m_sDisplayName")]
modded class RT_WaveSpawnerGroupsTooltipDetail : SCR_EntityTooltipDetail
{
	//------------------------------------------------------------------------------------------------
	override bool CreateDetail(SCR_EditableEntityComponent entity, Widget parent, TextWidget label, bool setFrameslot = true)
	{
		RT_WavesSpawnerEntity_Vehicles spawner = RT_WavesSpawnerEntity_Vehicles.Cast(entity.GetOwner());
		if (spawner) 
			m_sDisplayName = "#RT_WS_V-Editor_TooltipDetail_Groups_Name";
		
		return super.CreateDetail(entity, parent, label, setFrameslot);
	}
	
//	//------------------------------------------------------------------------------------------------
//	override bool InitDetail(SCR_EditableEntityComponent entity, Widget widget)
//	{	
//		RT_WavesSpawnerEntity_Vehicles spawner = RT_WavesSpawnerEntity_Vehicles.Cast(entity.GetOwner());
//		if (spawner) return false;
//			
//		return super.InitDetail(entity, widget);
//	}
}

[BaseContainerProps(), BaseContainerCustomTitleField("m_sDisplayName")]
modded class RT_WaveSpawnerUseVehiclesTooltipDetail : SCR_EntityTooltipDetail
{
	//------------------------------------------------------------------------------------------------
	override bool InitDetail(SCR_EditableEntityComponent entity, Widget widget)
	{	
		RT_WavesSpawnerEntity_Vehicles spawner = RT_WavesSpawnerEntity_Vehicles.Cast(entity.GetOwner());
		if (spawner) return false;
			
		return super.InitDetail(entity, widget);
	}
}