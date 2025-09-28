class RT_ChangeFaceActionComponent: ScriptedUserAction
{		
	override bool CanBePerformedScript(IEntity user)
	{
		PlayerController pc = GetGame().GetPlayerController();
		
		if (!pc) {
			m_sCannotPerformReason = "No PlayerController";
			return false;
		}
		
		SCR_PlayerFactionAffiliationComponent fac = SCR_PlayerFactionAffiliationComponent.Cast(pc.FindComponent(SCR_PlayerFactionAffiliationComponent));
		
		if (!fac) {
			m_sCannotPerformReason = "No SCR_PlayerFactionAffiliationComponent";
			return false;
		}
				
		SCR_Faction faction = SCR_Faction.Cast(fac.GetAffiliatedFaction());
		
		if (!faction) {
			m_sCannotPerformReason = "No SCR_Faction on player";
			return false;
		}
		
		if (GetGame().GetMenuManager().FindMenuByPreset(ChimeraMenuPreset.FaceEditorBrowserDialog)) 
		{
			m_sCannotPerformReason = "Dialog  is already open";
			return false;
		}
		
		return true;
	}
	
	override bool HasLocalEffectOnlyScript() {return true; }
	
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) {			
		if (GetGame().GetMenuManager().FindMenuByPreset(ChimeraMenuPreset.FaceEditorBrowserDialog)) return; // already opened	
				
		MenuBase a = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.FaceEditorBrowserDialog);
		RT_FaceEditorBrowserDialogUI menu = RT_FaceEditorBrowserDialogUI.Cast(a);
		
		if (!menu) return;
		
		ScriptInvokerMenu onMenuClose = SCR_MenuHelper.GetOnMenuClose();

		if (onMenuClose) onMenuClose.Insert(onChooseFaceMenuClose);
	}
	
	protected void onChooseFaceMenuClose(ChimeraMenuBase pMenu)
	{		
		RT_FaceEditorBrowserDialogUI menu = RT_FaceEditorBrowserDialogUI.Cast(pMenu);		
				
		if (!menu || !menu.m_Editor) return;
		
		VisualIdentity currentFace = menu.m_Editor.m_CurrentFace;		
		if (!currentFace) return;
		
		SCR_PlayerData.SaveCurrentUserVisual(currentFace);
		
		
		SCR_ChimeraCharacter playerEntity = SCR_ChimeraCharacter.Cast(GetGame().GetPlayerController().GetControlledEntity());
		
		if (!playerEntity) return;
		
		playerEntity.SaveVisualToServer_O(currentFace.GetHead(), currentFace.GetBody());		
	}
}
