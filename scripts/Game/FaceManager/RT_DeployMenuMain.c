modded enum ChimeraMenuPreset : ScriptMenuPresetEnum
{
	FaceEditorBrowserDialog
}

modded class SCR_DeployMenuMain : SCR_DeployMenuBase {
	protected SCR_ButtonImageComponent m_ChooseFaceButton;
	
	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		
		m_ChooseFaceButton = SCR_ButtonImageComponent.GetButtonImage("ChooseFaceButton", GetRootWidget());
		if (m_ChooseFaceButton)
		{	
			m_ChooseFaceButton.m_OnClicked.Insert(OnOpenChooseFaceMenu);
		}
		
		LoadSavedFace();
	}
	
	protected void OnOpenChooseFaceMenu()
	{
		if (GetManager().FindMenuByPreset(ChimeraMenuPreset.FaceEditorBrowserDialog)) return; // already opened			
			
		MenuBase a = GetGame().GetMenuManager().OpenDialog(ChimeraMenuPreset.FaceEditorBrowserDialog);
		RT_FaceEditorBrowserDialogUI menu = RT_FaceEditorBrowserDialogUI.Cast(a);
		
		if (!menu) return;
		
		ScriptInvokerMenu onMenuClose = SCR_MenuHelper.GetOnMenuClose();

		if (onMenuClose) onMenuClose.Insert(onChooseFaceMenuClose);
	}

	
	protected void onChooseFaceMenuClose(ChimeraMenuBase pMenu)
	{		
		RT_FaceEditorBrowserDialogUI menu = RT_FaceEditorBrowserDialogUI.Cast(pMenu);		
		
		if (!menu || !m_LoadoutRequestUIHandler) return;
		
		if (!menu.m_Editor) return;
		
		
		VisualIdentity currentFace = menu.m_Editor.m_CurrentFace;		
		
		if (!currentFace) return;
		
		IEntity previewEntity = m_LoadoutRequestUIHandler.GetPreviewedEntity();
		
		if (!previewEntity) return;
		
		RT_CF_Utils.SetIdentity(previewEntity, currentFace);		
		m_LoadoutRequestUIHandler.ResetLoadoutPreview();
		
		SCR_PlayerData.SaveCurrentUserVisual(currentFace);
	}
	
	protected override void OnPlayerLoadoutResponse(SCR_PlayerLoadoutComponent component, int loadoutIndex, bool response)
	{
		super.OnPlayerLoadoutResponse(component, loadoutIndex, response);
		
		LoadSavedFace();
	}
	
	protected void LoadSavedFace()
	{
		IEntity previewEntity = m_LoadoutRequestUIHandler.GetPreviewedEntity();
				
		if (!previewEntity) return;
		
		VisualIdentity currentFace = SCR_PlayerData.GetCurrentUserVisual();
		
		RT_CF_Utils.SetIdentity(previewEntity, currentFace);		
		m_LoadoutRequestUIHandler.ResetLoadoutPreview();
	}
}