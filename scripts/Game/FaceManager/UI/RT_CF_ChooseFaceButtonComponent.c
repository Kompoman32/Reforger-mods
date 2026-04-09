class RT_CF_ChooseFaceButtonComponent : SCR_ScriptedWidgetComponent
{
	[Attribute("LoadoutSelector")]
	protected string m_sLoadoutUIHandler;
	
	protected SCR_ButtonImageComponent m_ChooseFaceButton;
	
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		
		m_ChooseFaceButton = SCR_ButtonImageComponent.Cast(w.FindHandler(SCR_ButtonImageComponent));
		
		if (m_ChooseFaceButton)
		{	
			m_ChooseFaceButton.m_OnClicked.Insert(OnOpenChooseFaceMenu);
		}
	}
	
	override void HandlerDeattached(Widget w)
	{
		if (m_ChooseFaceButton)
		{	
			m_ChooseFaceButton.m_OnClicked.Remove(OnOpenChooseFaceMenu);
		}
	}
	
//	override void OnMenuOpen()
//	{
//		super.OnMenuOpen();
//		
//		m_ChooseFaceButton = SCR_ButtonImageComponent.GetButtonImage("ChooseFaceButton", GetRootWidget());
//		if (m_ChooseFaceButton)
//		{	
//			m_ChooseFaceButton.m_OnClicked.Insert(OnOpenChooseFaceMenu);
//		}
//		
//		LoadSavedFace();
//	}
	
//	override void OnMenuHide()
//	{
//		super.OnMenuHide();
//
//		if (m_ChooseFaceButton)
//		{	
//			m_ChooseFaceButton.m_OnClicked.Remove(OnOpenChooseFaceMenu);
//		}
//	}
	
	protected void OnOpenChooseFaceMenu()
	{
		if (GetGame().GetMenuManager().FindMenuByPreset(ChimeraMenuPreset.FaceEditorBrowserDialog)) return; // already opened			
			
		MenuBase a = GetGame().GetMenuManager().OpenDialog(ChimeraMenuPreset.FaceEditorBrowserDialog);
		RT_CF_FaceEditorBrowserDialogUI menu = RT_CF_FaceEditorBrowserDialogUI.Cast(a);
		
		if (!menu) return;
		
		if (SCR_MenuHelper.GetOnMenuClose()) SCR_MenuHelper.GetOnMenuClose().Insert(onChooseFaceMenuClose);
	}

	
	protected void onChooseFaceMenuClose(ChimeraMenuBase pMenu)
	{		
		RT_CF_FaceEditorBrowserDialogUI menu = RT_CF_FaceEditorBrowserDialogUI.Cast(pMenu);		
		if (!menu) return;
		
		if (SCR_MenuHelper.GetOnMenuClose()) SCR_MenuHelper.GetOnMenuClose().Remove(onChooseFaceMenuClose);
		
		SCR_LoadoutRequestUIComponent loadoutRequestUIHandler = GetLoadoutRequestHandler();
		
		if (!menu.m_Editor || !loadoutRequestUIHandler) return;
		
		VisualIdentity currentFace = menu.m_Editor.m_CurrentFace;		
		
		if (!currentFace) return;
		
		IEntity previewEntity = loadoutRequestUIHandler.GetPreviewedEntity();
		
		if (!previewEntity) return;
		
		ResourceName currentCamo = menu.m_Editor.m_CurrentCamo;
		
		if (currentCamo)
		{
			VisualIdentity newCurrentFace = VisualIdentity.Cast(currentFace.Clone());
			newCurrentFace.SetHead(currentCamo);
			newCurrentFace.SetBody(currentFace.GetBody());
			
			currentFace = newCurrentFace;
		}
		
				
		RT_CF_Utils.SavePlayerIdentity(previewEntity, currentFace);		
		loadoutRequestUIHandler.ResetLoadoutPreview();
		
		SCR_PlayerData.SaveCurrentUserVisual(currentFace);
	}
	
	protected void LoadSavedFace()
	{
		SCR_LoadoutRequestUIComponent loadoutRequestUIHandler = GetLoadoutRequestHandler();
		
		if (!loadoutRequestUIHandler) return;
		
		IEntity previewEntity = loadoutRequestUIHandler.GetPreviewedEntity();
				
		if (!previewEntity) return;
		VisualIdentity currentFace = SCR_PlayerData.GetCurrentUserVisual(previewEntity);
		
		RT_CF_Utils.SavePlayerIdentity(previewEntity, currentFace);		
		loadoutRequestUIHandler.ResetLoadoutPreview();
	}
	
	//------------------------------------------------------------------------------------------------
	SCR_LoadoutRequestUIComponent GetLoadoutRequestHandler()
	{
		Widget parent = m_wRoot;
		
		while (parent.GetParent())
		{
			parent = parent.GetParent();
		}
		
		Widget tmp = parent.FindAnyWidget(m_sLoadoutUIHandler);
		if (!tmp)
			return null;

		return SCR_LoadoutRequestUIComponent.Cast(tmp.FindHandler(SCR_LoadoutRequestUIComponent));
	}
}