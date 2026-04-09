//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class RT_CF_ChangeFaceContextAction : SCR_BaseContextAction
{	
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		if (!hoveredEntity || !SCR_ChimeraCharacter.Cast(hoveredEntity.GetOwner())) return false;

		return true;
	}
	
	override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		if (!hoveredEntity || !SCR_ChimeraCharacter.Cast(hoveredEntity.GetOwner())) return false;

		return true;
	}
	
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags, int param = -1)
	{		
		if (GetGame().GetMenuManager().FindMenuByPreset(ChimeraMenuPreset.FaceEditorBrowserDialog)) return; // already opened	
						
		MenuBase menuBase = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.FaceEditorBrowserDialog);
		RT_CF_FaceEditorBrowserDialogUI menu = RT_CF_FaceEditorBrowserDialogUI.Cast(menuBase);
		
		if (!menu) return;
		
		menu.m_Editor.SetSelectedEntity(hoveredEntity.GetOwner());
		
		GetGame().GetCallqueue().CallLater(SetupPreviewEntityLater, 300, false, menu.m_Editor, hoveredEntity.GetOwner());
		
		if (SCR_MenuHelper.GetOnMenuClose()) SCR_MenuHelper.GetOnMenuClose().Insert(onChooseFaceMenuClose);
	}
	
	protected void SetupPreviewEntityLater(RT_CF_FacesBrowserEditorUIComponent editor, IEntity ent)
	{				
		VisualIdentity currentFace = RT_CF_Utils.GetVisualIdentity(ent);
		
		editor.SetPreviewHead(new RT_CF_EditableEntityUIInfo(SCR_UIInfo.CreateInfo((string)currentFace.GetHead()), visual: currentFace));
		editor.FocusItemByName(currentFace.GetHead());
	}
	
	protected void onChooseFaceMenuClose(ChimeraMenuBase pMenu)
	{		
		RT_CF_FaceEditorBrowserDialogUI menu = RT_CF_FaceEditorBrowserDialogUI.Cast(pMenu);		
		
		if (!menu) return;
		
		if (SCR_MenuHelper.GetOnMenuClose()) SCR_MenuHelper.GetOnMenuClose().Remove(onChooseFaceMenuClose);
				
		if (!menu.m_Editor || !menu.m_Editor.GetSelectedEntity()) return;
		
		VisualIdentity currentFace = menu.m_Editor.m_CurrentFace;		
		SCR_ChimeraCharacter entity = SCR_ChimeraCharacter.Cast(menu.m_Editor.GetSelectedEntity());
		
		if (!currentFace || !entity) return;
		
		//entity.SaveVisualToServer_O(currentFace.GetHead(), currentFace.GetBody());		
		
		
		SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!pc) return;
		
		ResourceName currentCamo = menu.m_Editor.m_CurrentCamo;
		
		if (currentCamo)
		{
			VisualIdentity newCurrentFace = VisualIdentity.Cast(currentFace.Clone());
			newCurrentFace.SetHead(currentCamo);
			newCurrentFace.SetBody(currentFace.GetBody());
			
			currentFace = newCurrentFace;
		}
		
		pc.SaveEntityVisualToServer_O(entity, currentFace.GetHead(), currentFace.GetBody());
	}
};