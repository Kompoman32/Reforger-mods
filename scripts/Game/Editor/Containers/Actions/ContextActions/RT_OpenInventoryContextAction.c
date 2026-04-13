modded enum ENotification
{
	ACTION_NEED_CONTROLLED_ENTITY //!< Action need placed controlled entity and cannot be executed
}

//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class RT_OpenInventoryContextAction : SCR_BaseContextAction
{
	[Attribute(desc: "Display config, only header is used, labels are filled by action")]
	protected ref SCR_EditorContentBrowserDisplayConfig m_ContentBrowserConfig;
	
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{		
		if (!hoveredEntity) return false;
		
		if (hoveredEntity.GetEntityType() == EEditableEntityType.CHARACTER) return true;

		return false;
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
		PlayerController pc = GetGame().GetPlayerController();
		if (!pc) {
			return;
		};
		
		if (!GetGame().GetPlayerManager().GetPlayerControlledEntity(pc.GetPlayerId()))
		{
			SCR_NotificationsComponent.SendLocal(ENotification.ACTION_NEED_CONTROLLED_ENTITY);
			return;
		}
		
		SCR_ContentBrowserEditorComponent contentBrowserManager = SCR_ContentBrowserEditorComponent.Cast(SCR_ContentBrowserEditorComponent.GetInstance(SCR_ContentBrowserEditorComponent, true));
		if (!hoveredEntity || !contentBrowserManager)
		{
			return;
		}
		
		contentBrowserManager.m_EditInvEntity = hoveredEntity.GetOwnerScripted();
		
		SCR_EditableEntityUIInfo uiInfo = SCR_EditableEntityUIInfo.Cast(hoveredEntity.GetInfo());
		SCR_EditorContentBrowserSaveStateData resultState = new SCR_EditorContentBrowserSaveStateData();
		
		contentBrowserManager.OpenBrowserState(resultState, m_ContentBrowserConfig);	
	}
};
