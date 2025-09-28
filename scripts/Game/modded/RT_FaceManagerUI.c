modded class SCR_LoadoutRequestUIComponent : SCR_DeployRequestUIBaseComponent
{
	protected SCR_ButtonImageComponent m_ChooseFaceButton;
	
	
	IEntity GetPreviewedEntity()
	{
		return m_PreviewedEntity;
	}
	
	void ResetLoadoutPreview()
	{
		m_PreviewComp.GetPreviewManagerEntity().SetPreviewItem(m_PreviewComp.GetItemPreviewWidget(), GetPreviewedEntity(), null, true);
	}
	
	protected override void SetLoadoutPreview(SCR_BasePlayerLoadout loadout)
	{
		super.SetLoadoutPreview(loadout);
		
		VisualIdentity currentFace = SCR_PlayerData.GetCurrentUserVisual();
		
		RT_CF_Utils.SetIdentity(m_PreviewedEntity, currentFace);
		ResetLoadoutPreview();
	}
	
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		
		m_ChooseFaceButton = SCR_ButtonImageComponent.GetButtonImage("ChooseFaceButton", w);
	}
	
	protected override void OnListExpand(SCR_DeployRequestUIBaseComponent component, bool expanded)
	{
		super.OnListExpand(component, expanded);
		
		if (component != this)
		{
			expanded = false;
		}
		
		m_ChooseFaceButton.SetVisible(expanded, false);
	}
}


modded class SCR_ContextMenuButtonEditorUIComponent : ScriptedWidgetComponent
{
	ref RT_EditableEntityUIInfo m_FaceInfo;
	
	void SetData(RT_EditableEntityUIInfo info, string shortcutAction, bool canBePerformed)
	{
		m_FaceInfo = info;
		super.SetData(info, shortcutAction, canBePerformed);
	}
	
	void SetIconVisible(bool show)
	{
		m_ButtonIconWidget.SetVisible(show);
	}
	
	void SetShortcutVisible(bool show)
	{
		m_ShortcutTextWidget.SetVisible(show);
	}

}