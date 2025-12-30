// Enabled
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_WS_WavesSpawner_Attribute_Enabled: SCR_BaseEditorAttribute
{	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{		
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromAttribute(item);
		if (!entity) return null;
		
		return SCR_BaseEditorAttributeVar.CreateBool(entity.GetEnabled());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromAttribute(item);
		if (!entity) return;
		
		entity.SetEnabled(var.GetBool());
	}
}

// Max Entities
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_WS_WavesSpawner_Attribute_MaxEntities: SCR_BaseValueListEditorAttribute
{	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{		
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromAttribute(item);
		if (!entity) return null;
		
		return SCR_BaseEditorAttributeVar.CreateInt(entity.GetMaxUnits());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromAttribute(item);
		if (!entity) return;
		
		entity.SetMaxUnits(var.GetInt());
	}
}

// Delay
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_WS_WavesSpawner_Attribute_Delay: SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromAttribute(item);
		if (!entity) return null;
		
		return SCR_BaseEditorAttributeVar.CreateInt(entity.GetSpawnDelay());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromAttribute(item);
		if (!entity) return;
		
		entity.SetSpawnDelay(var.GetInt());
	}
}

// Min In Group
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_WS_WavesSpawner_Attribute_MinInGroup: SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromAttribute(item);
		if (!entity) return null;
		
		return SCR_BaseEditorAttributeVar.CreateInt(entity.GetMinInGroup());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromAttribute(item);
		if (!entity) return;
		
		entity.SetMinInGroup(var.GetInt());
	}
}

// Max In Group
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_WS_WavesSpawner_Attribute_MaxInGroup: SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromAttribute(item);
		if (!entity) return null;
		
		return SCR_BaseEditorAttributeVar.CreateInt(entity.GetMaxInGroup());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromAttribute(item);
		if (!entity) return;
		
		entity.SetMaxInGroup(var.GetInt());
	}
}

// Units
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_WS_WavesSpawner_Attribute_Units: SCR_BaseEditorAttribute
{	
	ref array<ResourceName> m_aAvailableResourceNames = {};
	ref array<string> m_aAvailableDisplayNames = {};
	
	ref array<string> m_aSelectedResourceNames = {};
	ref array<int> m_aSelectedWeights = {};
	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromAttribute(item);
		
		if (!entity) return null;
		
		m_aAvailableResourceNames = entity.m_aAvailableResourceNames;
		m_aAvailableDisplayNames = entity.m_aAvailableDisplayNames;
		
		m_aSelectedResourceNames.Clear();
		m_aSelectedWeights.Clear();
		
		foreach(Tuple2<string, int> setting: entity.m_aUnitsSettings)
		{
			m_aSelectedResourceNames.Insert(setting.param1);
			m_aSelectedWeights.Insert(setting.param2);
		}
		
		return SCR_BaseEditorAttributeVar.CreateVector(Vector(0,0,0));
	}
	
	override bool GetIsMultiSelect()
	{
		return true;
	}
	
	override bool GetHasConflictingValues()
	{
		return true;
	}
	
	override bool GetIsOverridingValues()
	{
		return true;
	}
}

// Radius In Spawner
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_WS_WavesSpawner_Attribute_Radius: SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromAttribute(item);
		if (!entity) return null;
		
		return SCR_BaseEditorAttributeVar.CreateInt(entity.GetRadius());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromAttribute(item);
		if (!entity) return;
		
		entity.SetRadius(var.GetInt());
	}
}


// Radius In MovePoint
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_WS_MovePoint_Attribute_Radius: SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		RT_WavesSpawner_MovePointEntity entity = RT_WS_AttributeUtils.GetMovePointEntityFromAttribute(item);
		if (!entity) return null;
		
		return SCR_BaseEditorAttributeVar.CreateInt(entity.GetAreaRadius());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		RT_WavesSpawner_MovePointEntity entity = RT_WS_AttributeUtils.GetMovePointEntityFromAttribute(item);
		if (!entity) return;
		
		entity.SetAreaRadius(var.GetInt());
	}
}


[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_WS_WavesSpawner_Attribute_Faction: SCR_BaseFactionEditableAttribute
{		
	protected override bool ValidEntity(GenericEntity entity)
	{
		RT_WavesSpawnerEntity spawner = RT_WavesSpawnerEntity.Cast(entity);		
		return spawner && spawner.m_bFactionLess;
	}
	
	protected override Faction GetFaction(GenericEntity entity)
	{		
		RT_WavesSpawnerEntity spawner = RT_WavesSpawnerEntity.Cast(entity);		
		return spawner.GetFaction();
	}
	
	protected override void SetFaction(GenericEntity entity, Faction faction)
	{
		RT_WavesSpawnerEntity spawner = RT_WavesSpawnerEntity.Cast(entity);
		
		spawner.SetFaction(faction);
	}
};
