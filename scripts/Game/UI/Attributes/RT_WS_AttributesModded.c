modded class SCR_EntityFactionEditorAttribute : SCR_BaseFactionEditableAttribute
{
	protected override bool ValidEntity(GenericEntity entity)
	{
		RT_WavesSpawnerEntity rtEntity = RT_WavesSpawnerEntity.Cast(entity);
		if (rtEntity) return false;
		
		return super.ValidEntity(entity);
	}
}

modded class SCR_AttributesManagerEditorComponent : SCR_BaseEditorComponent
{
	ref ScriptInvoker m_RT_WS_DelayedUnitsAttributeInit = new ScriptInvoker();
	
	ref RT_WS_WavesSpawner_Attribute_Units m_RT_WS_DelayedUnitsAttribute; // RT_WS_WavesSpawner_Attribute_Units attribute
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected override void StartEditingServer(array<int> itemIds)
	{
		foreach (int id: itemIds)
		{			
			Managed item = Replication.FindItem(id);
			RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromOwner(item);
			
			if (!entity) continue;
			
			array<Managed> editedItems = {};
			editedItems.Insert(item);
			
			array<int> attributesIds = {};
			array<ref SCR_BaseEditorAttributeVar> attributesVars = {};
			array<ref EEditorAttributeMultiSelect> attributesMultiSelect = {};
			
			GetVariables(true, editedItems, attributesIds, attributesVars, attributesMultiSelect);
			
			
			if (attributesIds.Count() == 0) continue;
			
			RT_WS_WavesSpawner_Attribute_Units attribute;
			int unitsAttributeId = -1;
			
			foreach (int attributeId: attributesIds)
			{
				attribute = RT_WS_WavesSpawner_Attribute_Units.Cast(m_PrefabData.GetAttribute(attributeId));
				
				if (attribute) 
				{
					unitsAttributeId = attributeId;
					break;
				}
			}
				
				
			if (attribute)
			{
				Rpc(RT_WS_LoadUnitsAttribute, id, unitsAttributeId, attribute.m_aSelectedResourceNames, attribute.m_aSelectedWeights);

				break;
			}
		}
		
		super.StartEditingServer(itemIds);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RT_WS_LoadUnitsAttribute(int pEntityId, int pAttributeId, notnull array<string> pSelectedResourceNames, notnull array<int> pSelectedWeights)
	{		
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromOwner(Replication.FindItem(pEntityId));
		if (!entity) return;
		
		RT_WS_WavesSpawner_Attribute_Units attribute = RT_WS_WavesSpawner_Attribute_Units.Cast(m_PrefabData.GetAttribute(pAttributeId));
		if (!attribute) return;
		
		attribute.m_aAvailableResourceNames = entity.m_aAvailableResourceNames;
		attribute.m_aAvailableDisplayNames = entity.m_aAvailableDisplayNames;
		
		attribute.m_aSelectedResourceNames = pSelectedResourceNames;
		attribute.m_aSelectedWeights = pSelectedWeights;
		
		m_RT_WS_DelayedUnitsAttribute = attribute;
		m_RT_WS_DelayedUnitsAttributeInit.Invoke(m_RT_WS_DelayedUnitsAttribute)
	}	
	
	override void ConfirmEditing()
	{
		//--- Not an owner, ignore
		if (!IsOwner())
			return;
		
		//--- Not editing, ignore
		if (!m_aEditedItems)
			return;
		
		RT_WS_WavesSpawner_Attribute_Units unitsAttribute;
		
		foreach (SCR_BaseEditorAttribute attribute: m_aEditedAttributes)
		{
			unitsAttribute = RT_WS_WavesSpawner_Attribute_Units.Cast(attribute);
			
			if (unitsAttribute) break;
		}
		
		if (unitsAttribute)
		{					
			int entityId = -1;
			
			
			foreach (Managed item: m_aEditedItems)
			{
				RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromOwner(item);
				
				if (item) {
					entityId = Replication.FindId(item);
				}					
			}
			
			Rpc(RT_WS_SendUnitsData, entityId, m_PrefabData.FindAttribute(unitsAttribute), unitsAttribute.m_aSelectedResourceNames, unitsAttribute.m_aSelectedWeights);
		}

		super.ConfirmEditing();
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RT_WS_SendUnitsData(int pEntityId, int attributeId, notnull array<string> pSelectedResourceNames, notnull array<int> pSelectedWeights)
	{
		RT_WavesSpawnerEntity entity = RT_WS_AttributeUtils.GetEntityFromOwner(Replication.FindItem(pEntityId));
		if (!entity) return;
		
		RT_WS_WavesSpawner_Attribute_Units unitsAttribute = RT_WS_WavesSpawner_Attribute_Units.Cast(m_PrefabData.GetAttribute(attributeId));
		
		if (!unitsAttribute) return;
		
		unitsAttribute.m_aSelectedResourceNames = pSelectedResourceNames;
		unitsAttribute.m_aSelectedWeights = pSelectedWeights;
		
		entity.SetSelectedUnits(pSelectedResourceNames, pSelectedWeights)
	}
}