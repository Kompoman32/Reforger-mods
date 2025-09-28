//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class RT_OpenInventoryContextAction : SCR_SelectedEntitiesContextAction
{
	[Attribute(desc: "Display config, only header is used, labels are filled by action")]
	protected ref SCR_EditorContentBrowserDisplayConfig m_ContentBrowserConfig;
	
	override bool CanBeShown(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
	{		
		if (selectedEntity.GetEntityType() == EEditableEntityType.CHARACTER)
		{
			return true;
		}

		return false;
	}
	
	override bool CanBePerformed(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
	{
		SCR_DamageManagerComponent damageComponent = SCR_DamageManagerComponent.Cast(selectedEntity.GetOwner().FindComponent(SCR_DamageManagerComponent));
		
		if (!damageComponent || damageComponent.GetState() == EDamageState.DESTROYED)
			return false;

		return true;
	}
	
	override void Perform(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition)
	{		
		SCR_ContentBrowserEditorComponent contentBrowserManager = SCR_ContentBrowserEditorComponent.Cast(SCR_ContentBrowserEditorComponent.GetInstance(SCR_ContentBrowserEditorComponent, true));
		if (!selectedEntity || !contentBrowserManager)
		{
			return;
		}
		
		contentBrowserManager.m_EditInvEntity = selectedEntity.GetOwnerScripted();
		
		SCR_EditableEntityUIInfo uiInfo = SCR_EditableEntityUIInfo.Cast(selectedEntity.GetInfo());
		SCR_EditorContentBrowserSaveStateData resultState = new SCR_EditorContentBrowserSaveStateData();
		
		contentBrowserManager.OpenBrowserState(resultState, m_ContentBrowserConfig);	
	}
};
