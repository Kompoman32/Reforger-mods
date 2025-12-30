class RT_WS_AttributeUtils 
{
	static RT_WavesSpawnerEntity GetEntityFromAttribute(Managed item)
	{
		SCR_EditableEntityComponent comp = SCR_EditableEntityComponent.Cast(item);
		if (!comp) return null;		
		
		return RT_WavesSpawnerEntity.Cast(comp.GetOwner());		
	}
	
	static RT_WavesSpawner_MovePointEntity GetMovePointEntityFromAttribute(Managed item)
	{
		SCR_EditableEntityComponent comp = SCR_EditableEntityComponent.Cast(item);
		if (!comp) return null;		
		
		return RT_WavesSpawner_MovePointEntity.Cast(comp.GetOwner());		
	}
	
	static RT_WavesSpawnerEntity GetEntityFromOwner(Managed item)
	{
		ScriptComponent comp = ScriptComponent.Cast(item);
		if (!comp) return null;		
		
		return RT_WavesSpawnerEntity.Cast(comp.GetOwner());		
	}
}