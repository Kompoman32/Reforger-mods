class RT_CF_ChangeFaceActionComponent: ScriptedUserAction
{		
	override bool CanBePerformedScript(IEntity user)
	{
		PlayerController pc = GetGame().GetPlayerController();
		
		if (!pc || !SCR_ChimeraCharacter.Cast(user)) {
			m_sCannotPerformReason = "#RT-Face_Wardrobe_Action_No_PlayerController";
			return false;
		}
		
//		SCR_PlayerFactionAffiliationComponent fac = SCR_PlayerFactionAffiliationComponent.Cast(pc.FindComponent(SCR_PlayerFactionAffiliationComponent));
//		
//		if (!fac) {
//			m_sCannotPerformReason = "#RT-Face_Wardrobe_Action_No_Faction";
//			return false;
//		}
//				
//		SCR_Faction faction = SCR_Faction.Cast(fac.GetAffiliatedFaction());
//		
//		if (!faction) {
//			m_sCannotPerformReason = "#RT-Face_Wardrobe_Action_No_Faction";
//			return false;
//		}
		
		if (GetGame().GetMenuManager().FindMenuByPreset(ChimeraMenuPreset.FaceEditorBrowserDialog)) 
		{
			m_sCannotPerformReason = "#RT-Face_Wardrobe_Action_Dialog_Already_Opened";
			return false;
		}
		
		return true;
	}
	
	override bool HasLocalEffectOnlyScript() {return true; }
	
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) {			
		if (GetGame().GetMenuManager().FindMenuByPreset(ChimeraMenuPreset.FaceEditorBrowserDialog)) return; // already opened	
				
		MenuBase a = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.FaceEditorBrowserDialog);
		a.SetActionContext("EditorBrowserContext");
		RT_CF_FaceEditorBrowserDialogUI menu = RT_CF_FaceEditorBrowserDialogUI.Cast(a);
		
		if (!menu) return;
		
		menu.m_Editor.SetSelectedEntity(pUserEntity);
		
		GetGame().GetCallqueue().CallLater(SetupPreviewEntityLater, 100, false, menu.m_Editor, pUserEntity);
		
		if (SCR_MenuHelper.GetOnMenuClose()) SCR_MenuHelper.GetOnMenuClose().Insert(onChooseFaceMenuClose);
	}
	
	protected void SetupPreviewEntityLater(RT_CF_FacesBrowserEditorUIComponent editor, IEntity ent)
	{				
		VisualIdentity currentFace = RT_CF_Utils.GetVisualIdentity(ent);
		
		if (currentFace)
		{
			VisualIdentity newCurrentFace = VisualIdentity.Cast(currentFace.Clone());
			newCurrentFace.SetHead(currentFace.GetHead());
			newCurrentFace.SetBody(currentFace.GetBody());
			
			currentFace = newCurrentFace;
		}
		
		editor.SetPreviewHead(new RT_CF_EditableEntityUIInfo(SCR_UIInfo.CreateInfo((string)currentFace.GetHead()), visual: currentFace));
		editor.FocusItemByName(currentFace.GetHead());
	}
	
	protected void onChooseFaceMenuClose(ChimeraMenuBase pMenu)
	{		
		RT_CF_FaceEditorBrowserDialogUI menu = RT_CF_FaceEditorBrowserDialogUI.Cast(pMenu);		
				
		if (!menu) return;
		
		if (SCR_MenuHelper.GetOnMenuClose()) SCR_MenuHelper.GetOnMenuClose().Remove(onChooseFaceMenuClose);
		
		if (!menu.m_Editor) return;
		
		VisualIdentity currentFace = menu.m_Editor.m_CurrentFace;		
		if (!currentFace) return;
		
		SCR_ChimeraCharacter playerEntity = SCR_ChimeraCharacter.Cast(GetGame().GetPlayerController().GetControlledEntity());
		
		if (!playerEntity) return;

		ResourceName currentCamo = menu.m_Editor.m_CurrentCamo;
		
		if (currentCamo)
		{
			VisualIdentity newCurrentFace = VisualIdentity.Cast(currentFace.Clone());
			newCurrentFace.SetHead(currentCamo);
			newCurrentFace.SetBody(currentFace.GetBody());
			
			currentFace = newCurrentFace;
		}
		
		SCR_PlayerData.SaveCurrentUserVisual(currentFace);
		
		playerEntity.SaveVisualToServer_O(currentFace.GetHead(), currentFace.GetBody());		
	}
}
