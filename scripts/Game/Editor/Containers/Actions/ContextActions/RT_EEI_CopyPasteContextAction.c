modded enum ENotification
{
	ACTION_PASTE_INVENTORY_WRONG_FORMAT //!< Pasted loadout has wrong format and can't be applied.
}

[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class RT_EEI_CopyContextAction : SCR_BaseContextAction
{
	static string CopiedLoadout = string.Empty;
	
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{		
		if (!hoveredEntity) return false;
		
		if (hoveredEntity.GetEntityType() != EEditableEntityType.CHARACTER) return false;

		return true;
	}
	
	override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		SCR_DamageManagerComponent damageComponent = SCR_DamageManagerComponent.Cast(hoveredEntity.GetOwner().FindComponent(SCR_DamageManagerComponent));
		
		if (!damageComponent || damageComponent.GetState() == EDamageState.DESTROYED)
			return false;

		return true;
	}
	
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags, int param = -1)
	{	
		IEntity entity = hoveredEntity.GetOwner();		
		
		SCR_JsonSaveContext saveContext = new SCR_JsonSaveContext();
		if (!SCR_PlayerArsenalLoadout.ReadLoadoutString(entity, saveContext)) {
			CopiedLoadout = string.Empty;
			return;
		}
		
		// Console local save / PC clipboard save
		if (GetGame().IsPlatformGameConsole()) {
			CopiedLoadout = saveContext.SaveToString();
		} else {
			System.ExportToClipboard(saveContext.SaveToString());
		}
	}
}

[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class RT_EEI_PasteContextAction : SCR_SelectedEntitiesContextAction
{	
	string m_PasteContextString;
	ref JsonLoadContext m_PasteContext;
	
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{				
		// Console local save / PC clipboard save
		if (GetGame().IsPlatformGameConsole()) {
			if (!RT_EEI_CopyContextAction.CopiedLoadout) return false;
		} else {
			if (!System.ImportFromClipboard()) return false;
		}	

		return super.CanBeShown(hoveredEntity, selectedEntities, cursorWorldPosition, flags);
	}
	
	override bool CanBeShown(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
	{
		if (!GetGame().GetPlayerController()) return false;		
		if (!selectedEntity) return false;
		if (selectedEntity.GetEntityType() != EEditableEntityType.CHARACTER) return false;

		return super.CanBeShown(selectedEntity, cursorWorldPosition, flags);
	}
	
	override bool CanBePerformed(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
	{
		SCR_DamageManagerComponent damageComponent = SCR_DamageManagerComponent.Cast(selectedEntity.GetOwner().FindComponent(SCR_DamageManagerComponent));
		
		if (!damageComponent || damageComponent.GetState() == EDamageState.DESTROYED)
			return false;	
			
		return true;
	}
	
	override bool InitPerform()
	{
		string loadout;
		
		// Console local save / PC clipboard save
		if (GetGame().IsPlatformGameConsole()) {
			loadout = RT_EEI_CopyContextAction.CopiedLoadout;
		} else {
			loadout = System.ImportFromClipboard();
		}	
		
		JsonLoadContext ctx = new JsonLoadContext();		
		if (!ctx.LoadFromString(loadout)) 
		{
			GetGame().GetCallqueue().Remove(ShowWarningDebounced);
			GetGame().GetCallqueue().CallLater(ShowWarningDebounced, 100, false);
			m_PasteContext = null;
			m_PasteContextString = string.Empty;
			return false;
		}
		
		m_PasteContext = ctx;
		m_PasteContextString = loadout;
		
		return true;
	}
	
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags, int param = -1)
	{	
		if (!InitPerform()) return;
		if (!m_PasteContext) return;
		
		set<SCR_EditableEntityComponent> filteredSelectedEntities = new set<SCR_EditableEntityComponent>();
		
		foreach	(SCR_EditableEntityComponent entity : selectedEntities)
		{
			if (CanBePerformed(entity, cursorWorldPosition, 0))
			{
				filteredSelectedEntities.Insert(entity);
			}
		}
		
		array<RplId> selectedEntityIds = {};
		SerializeEntities(filteredSelectedEntities, selectedEntityIds);
		
		SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		
		pc.RT_EEI_PasteLoadoutToEntities(selectedEntityIds, m_PasteContextString);
	}
	
	protected void SerializeEntities(notnull set<SCR_EditableEntityComponent> selectedEntities, out array<RplId> selectedEntityIds)
	{
		foreach (SCR_EditableEntityComponent selectedEntity : selectedEntities)
		{
			if (!selectedEntity)
				continue;
			
			RplId selectedEntityId = Replication.FindId(selectedEntity);
			if (selectedEntityId == -1)
				continue;
			
			selectedEntityIds.Insert(selectedEntityId);
		}
	}
	
	protected void ShowWarningDebounced()
	{
		SCR_NotificationsComponent.SendLocal(ENotification.ACTION_PASTE_INVENTORY_WRONG_FORMAT);
	}
}