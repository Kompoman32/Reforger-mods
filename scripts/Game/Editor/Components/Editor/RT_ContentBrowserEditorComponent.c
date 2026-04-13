
modded class SCR_ContentBrowserEditorComponent : SCR_BaseEditorComponent 
{
	IEntity m_EditInvEntity;
	
	override bool OpenBrowserState(SCR_EditorContentBrowserSaveStateData saveState, SCR_EditorContentBrowserDisplayConfig displayConfig = null)
	{
		bool result = super.OpenBrowserState(saveState, displayConfig);
		
		if (!result) return false;
		
		vector transform[4] = { "0 0 0", "0 0 0", "0 0 0", "0 0 0" };
		SCR_EditorPreviewParams params = SCR_EditorPreviewParams.CreateParams(transform, verticalMode: EEditorTransformVertical.SEA);

	
		m_PlacingManager.m_EditInvEntity = m_EditInvEntity;
		m_PlacingManager.SetInstantPlacing(params);
		
		return true;
	}
	
	override void OnMenuClosed()
	{
		m_PlacingManager.m_EditInvEntity = null;
		m_EditInvEntity = null;
		super.OnMenuClosed();
	}
}

modded class SCR_PlacingEditorComponent : SCR_BaseEditorComponent
{
	IEntity m_EditInvEntity = null;
	
	override bool SetSelectedPrefab(ResourceName prefab = "", bool onConfirm = false, bool showBudgetMaxNotification = true, set<SCR_EditableEntityComponent> recipients = null)
	{
		if (m_EditInvEntity)
		{
			SCR_EditorModeEntity mode = SCR_EditorModeEntity.Cast(SCR_EditorModeEntity.GetInstance());
			
			RplIdentity identity = Replication.FindOwner(RplComponent.Cast(mode.FindComponent(RplComponent)).Id());
			
			mode.CreateEntityForEditInventoryOnServer(m_EditInvEntity, prefab, identity);
			
			m_EditInvEntity = null;
			
			
			SetInstantPlacing(null);
			m_StatesManager.SetSafeDialog(true);
			m_SelectedPrefab = prefab;
			m_Recipients = recipients;
			return true;
		}
		
		return super.SetSelectedPrefab(prefab, onConfirm, showBudgetMaxNotification, recipients);
	}
}


modded class SCR_EditorModeEntity : SCR_EditorBaseEntity
{
	ref map<RplIdentity, RplId> m_ArsenalEntititesForEditInventory = new map<RplIdentity, RplId>();
	
	EntityID m_EditInvSelectedEntityId;
	RplId m_EditInvWaitingEntityId;
	WorldTimestamp m_EditInvWaitingStart;
	
	void CreateEntityForEditInventoryOnServer(IEntity selectedEntity, ResourceName prefab, RplIdentity pIdentity)
	{
		Rpc(Rpc_AskCreateEntityOnServer, selectedEntity.GetID(), prefab, pIdentity);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void Rpc_AskCreateEntityOnServer(EntityID pSelectedEntityId , ResourceName pPrefab, RplIdentity pIdentity)
	{		
		IEntity entity = GetGame().SpawnEntityPrefabEx(pPrefab, false, GetGame().GetWorld(), null);
		
		SCR_EditableEntityComponent eec = SCR_EditableEntityComponent.Cast(entity.FindComponent(SCR_EditableEntityComponent));
		
		if (eec)
			eec.SetEntityFlag(EEditableEntityFlag.NON_INTERACTIVE, true);					
		
		if (!entity) return;
		
		RplComponent rplComp = RplComponent.Cast(entity.FindComponent(RplComponent));
		rplComp.Give(pIdentity);
		
		RplId entityId = rplComp.Id();
		m_ArsenalEntititesForEditInventory.Set(pIdentity, entityId);		

		Rpc(Rpc_DoCreateEntityOnServerResponce, pSelectedEntityId, entityId);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void Rpc_DoCreateEntityOnServerResponce(EntityID pEntityId, RplId pArsenalEntityId)
	{
		m_EditInvSelectedEntityId = pEntityId;
		m_EditInvWaitingEntityId = pArsenalEntityId;
		m_EditInvWaitingStart = ChimeraWorld.CastFrom(GetGame().GetWorld()).GetLocalTimestamp();
		
		GetGame().GetCallqueue().CallLater(WaitToOpenBeaconMenu, 300, true);		
	}
	
	protected void WaitToOpenBeaconMenu() 
	{
		WorldTimestamp timeNow_s = ChimeraWorld.CastFrom(GetGame().GetWorld()).GetLocalTimestamp();
		
		if (!m_EditInvWaitingEntityId || !m_EditInvWaitingStart || timeNow_s.DiffSeconds(m_EditInvWaitingStart) > 10)
		{
			m_EditInvSelectedEntityId = null;
			m_EditInvWaitingEntityId = null;
			m_EditInvWaitingStart = null;
			
			GetGame().GetCallqueue().Remove(WaitToOpenBeaconMenu);
			return;
		}		
		
		RplComponent arsRplComp = RplComponent.Cast(Replication.FindItem(m_EditInvWaitingEntityId));
		
		if (!arsRplComp) return;
		
		IEntity arsenalEntity = arsRplComp.GetEntity();
		
		if (!arsenalEntity) return;		
		
		IEntity entity = GetGame().GetWorld().FindEntityByID(m_EditInvSelectedEntityId);
		
		GetGame().GetCallqueue().Remove(WaitToOpenBeaconMenu);
		
		if (entity) {
			OpenBaconMenu(entity, arsenalEntity);	
		};
		
		m_EditInvSelectedEntityId = null;
		m_EditInvWaitingEntityId = null;
		m_EditInvWaitingStart = null;
	}
	
	void OpenBaconMenu(IEntity selectedEntity, IEntity arsenalEntity)
	{		
		if (!selectedEntity || !arsenalEntity) return;
		
		MenuManager menuManager = GetGame().GetMenuManager();
		Bacon_GunBuilderUI menu = Bacon_GunBuilderUI.Cast(menuManager.OpenMenu(ChimeraMenuPreset.Bacon_GunBuilderUI));
		
		menu.Init(arsenalEntity, selectedEntity);
		
		RplId aresenalIdForReplicationFind = null;
		RplComponent rplComp = RplComponent.Cast(arsenalEntity.FindComponent(RplComponent));
		
		if (rplComp)
		{
			aresenalIdForReplicationFind = rplComp.Id();
			GetGame().GetCallqueue().CallLater(RemoveArsenalEntity, 1000, true, aresenalIdForReplicationFind)
		}
	}
	
	protected void RemoveArsenalEntity(RplId aresenalIdForReplicationFind)
	{
		if (Bacon_GunBuilderUI.m_Instance) return;
		
		Rpc(Rpc_AskRemoveArsenalEntity_S, aresenalIdForReplicationFind);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void Rpc_AskRemoveArsenalEntity_S(RplId aresenalIdForReplicationFind)
	{		
		RplComponent arsRplComp = RplComponent.Cast(Replication.FindItem(aresenalIdForReplicationFind));
		
		if (!arsRplComp) return;
		
		IEntity arsenalEntity = arsRplComp.GetEntity();
		
		if (!arsenalEntity) return;
		
		RplComponent.DeleteRplEntity(arsenalEntity, false);
	}
}
