// Max Entities
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
modded class RT_WS_WavesSpawner_Attribute_MaxEntities: SCR_BaseValueListEditorAttribute
{	
	bool m_bIsVehicleSpawnerAttribute = false;
	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		RT_WavesSpawnerEntity_Vehicles entity = RT_WavesSpawnerEntity_Vehicles.Cast(RT_WS_AttributeUtils.GetEntityFromAttribute(item));
		if (!!entity != !!m_bIsVehicleSpawnerAttribute) return null;
		
		return super.ReadVariable(item, manager);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		RT_WavesSpawnerEntity_Vehicles entity = RT_WavesSpawnerEntity_Vehicles.Cast(RT_WS_AttributeUtils.GetEntityFromAttribute(item));
		if (!!entity != !!m_bIsVehicleSpawnerAttribute) return;
		
		super.WriteVariable(item, var, manager, playerID);
	}
}

// Max Entities
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_WS_V_WavesSpawner_Attribute_MaxEntities: RT_WS_WavesSpawner_Attribute_MaxEntities
{
	void RT_WS_V_WavesSpawner_Attribute_MaxEntities()
	{
		m_bIsVehicleSpawnerAttribute = true;	
	}	
}

// Delay
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
modded class RT_WS_WavesSpawner_Attribute_Delay: SCR_BaseValueListEditorAttribute
{
	bool m_bIsVehicleSpawnerAttribute = false;
	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		RT_WavesSpawnerEntity_Vehicles entity = RT_WavesSpawnerEntity_Vehicles.Cast(RT_WS_AttributeUtils.GetEntityFromAttribute(item));
		if (!!entity != !!m_bIsVehicleSpawnerAttribute) return null;
		
		return super.ReadVariable(item, manager);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		RT_WavesSpawnerEntity_Vehicles entity = RT_WavesSpawnerEntity_Vehicles.Cast(RT_WS_AttributeUtils.GetEntityFromAttribute(item));
		if (!!entity != !!m_bIsVehicleSpawnerAttribute) return;
		
		super.WriteVariable(item, var, manager, playerID);
	}
}

// Delay
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_WS_V_WavesSpawner_Attribute_Delay: RT_WS_WavesSpawner_Attribute_Delay
{
	void RT_WS_V_WavesSpawner_Attribute_Delay()
	{
		m_bIsVehicleSpawnerAttribute = true;	
	}	
}

// Min In Group
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
modded class RT_WS_WavesSpawner_Attribute_MinInGroup: SCR_BaseValueListEditorAttribute
{
	bool m_bIsVehicleSpawnerAttribute = false;
	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		RT_WavesSpawnerEntity_Vehicles entity = RT_WavesSpawnerEntity_Vehicles.Cast(RT_WS_AttributeUtils.GetEntityFromAttribute(item));
		if (!!entity != !!m_bIsVehicleSpawnerAttribute) return null;
		
		return super.ReadVariable(item, manager);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		RT_WavesSpawnerEntity_Vehicles entity = RT_WavesSpawnerEntity_Vehicles.Cast(RT_WS_AttributeUtils.GetEntityFromAttribute(item));
		if (!!entity != !!m_bIsVehicleSpawnerAttribute) return;
		
		super.WriteVariable(item, var, manager, playerID);
	}
}

// Max In Group
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
modded class RT_WS_WavesSpawner_Attribute_MaxInGroup: SCR_BaseValueListEditorAttribute
{
	bool m_bIsVehicleSpawnerAttribute = false;
	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		RT_WavesSpawnerEntity_Vehicles entity = RT_WavesSpawnerEntity_Vehicles.Cast(RT_WS_AttributeUtils.GetEntityFromAttribute(item));
		if (!!entity != !!m_bIsVehicleSpawnerAttribute) return null;
		
		return super.ReadVariable(item, manager);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		RT_WavesSpawnerEntity_Vehicles entity = RT_WavesSpawnerEntity_Vehicles.Cast(RT_WS_AttributeUtils.GetEntityFromAttribute(item));
		if (!!entity != !!m_bIsVehicleSpawnerAttribute) return;
		
		
		super.WriteVariable(item, var, manager, playerID);
	}
}

// UseVehicles
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
modded class RT_WS_WavesSpawner_Attribute_UseVehicles: SCR_BaseEditorAttribute
{	
	bool m_bIsVehicleSpawnerAttribute = false;

	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		RT_WavesSpawnerEntity_Vehicles entity = RT_WavesSpawnerEntity_Vehicles.Cast(RT_WS_AttributeUtils.GetEntityFromAttribute(item));
		if (!!entity != !!m_bIsVehicleSpawnerAttribute) return null;
		
		return super.ReadVariable(item, manager);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		RT_WavesSpawnerEntity_Vehicles entity = RT_WavesSpawnerEntity_Vehicles.Cast(RT_WS_AttributeUtils.GetEntityFromAttribute(item));
		if (!!entity != !!m_bIsVehicleSpawnerAttribute) return;
		
		super.WriteVariable(item, var, manager, playerID);
	}
}

// Units
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
modded class RT_WS_WavesSpawner_Attribute_Units: SCR_BaseEditorAttribute
{	
	bool m_bIsVehicleSpawnerAttribute = false;
	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		RT_WavesSpawnerEntity_Vehicles entity = RT_WavesSpawnerEntity_Vehicles.Cast(RT_WS_AttributeUtils.GetEntityFromAttribute(item));
		if (!!entity != !!m_bIsVehicleSpawnerAttribute) return null;
		
		return super.ReadVariable(item, manager);
	}
}

// Units
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_WS_V_WavesSpawner_Attribute_Units: RT_WS_WavesSpawner_Attribute_Units
{
	void RT_WS_V_WavesSpawner_Attribute_Units()
	{
		m_bIsVehicleSpawnerAttribute = true;	
	}
}

// Radius In Spawner
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
modded class RT_WS_WavesSpawner_Attribute_Radius: SCR_BaseValueListEditorAttribute
{
	bool m_bIsVehicleSpawnerAttribute = false;

	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		RT_WavesSpawnerEntity_Vehicles entity = RT_WavesSpawnerEntity_Vehicles.Cast(RT_WS_AttributeUtils.GetEntityFromAttribute(item));
		if (!!entity != !!m_bIsVehicleSpawnerAttribute) return null;
		
		return super.ReadVariable(item, manager);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		RT_WavesSpawnerEntity_Vehicles entity = RT_WavesSpawnerEntity_Vehicles.Cast(RT_WS_AttributeUtils.GetEntityFromAttribute(item));
		if (!!entity != !!m_bIsVehicleSpawnerAttribute) return;
		
		super.WriteVariable(item, var, manager, playerID);
	}
}

// Radius In Spawner
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_WS_V_WavesSpawner_Attribute_Radius: RT_WS_WavesSpawner_Attribute_Radius
{
	void RT_WS_V_WavesSpawner_Attribute_Radius()
	{
		m_bIsVehicleSpawnerAttribute = true;	
	}
}
