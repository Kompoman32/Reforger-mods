class RT_CF_FaceEditorBrowserDialogUI : MenuRootBase {
	const string WIDGET_BUTTON_CLOSE = "CloseButton";
	const string WIDGET_BUTTON_SEARCH = "SearchButton";
	const string WIDGET_BUTTON_CAMO = "FocusSwitchButton";
	
	const string WIDGET_SEARCH_EDITBOX = "EditBoxSearch";
	const string WIDGET_CAMO_COMBOBOX = "CamoComboBox";
	
	const string WIDGET_EDITOR_WINDOW = "Window";
	
	protected Widget m_wEditor;
	protected Widget m_wSearchEditBox;
	protected Widget m_wCamoCombobox;
	
	RT_CF_FacesBrowserEditorUIComponent m_Editor;
	SCR_EditBoxComponent m_SearchEditBox;
	
	void CloseSelf()
	{
		GetManager().CloseMenu(this);
	}
	
	Widget GetWidgetByName(string widgetName)
	{
		Widget root = GetRootWidget();
		if (!root) return null;
		
		return root.FindAnyWidget(widgetName);
	}
	
	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		
		Widget rootWidget = GetRootWidget();
		if (!rootWidget) return;
		
		m_wEditor = GetWidgetByName(WIDGET_EDITOR_WINDOW);
		
		if (m_wEditor)
		{
			m_Editor = RT_CF_FacesBrowserEditorUIComponent.Cast(m_wEditor.FindHandler(RT_CF_FacesBrowserEditorUIComponent));
		}
		
		m_wSearchEditBox = GetWidgetByName(WIDGET_SEARCH_EDITBOX);

		if (m_wSearchEditBox)
			m_SearchEditBox = SCR_EditBoxComponent.Cast(m_wSearchEditBox.FindHandler(SCR_EditBoxComponent));

		if (m_SearchEditBox)
		{
			m_SearchEditBox.m_OnConfirm.Insert(OnSearchConfirmed);
		}
		
		m_wCamoCombobox = GetWidgetByName(WIDGET_CAMO_COMBOBOX);
		
		ScriptInvoker onClose = ButtonActionComponent.GetOnAction(rootWidget, WIDGET_BUTTON_CLOSE);
		if (onClose) onClose.Insert(CloseSelf);
		
		ScriptInvoker onSearchButton = ButtonActionComponent.GetOnAction(rootWidget, WIDGET_BUTTON_SEARCH);
		if (onSearchButton) onSearchButton.Insert(FocusSearch);
		
		ScriptInvoker onCamoButton = ButtonActionComponent.GetOnAction(rootWidget, WIDGET_BUTTON_CAMO);
		if (onCamoButton) onCamoButton.Insert(FocusCamo);				
	}	
	
	protected void FocusSearch()
	{
		if (!m_SearchEditBox) return;
	
		GetGame().GetWorkspace().SetFocusedWidget(m_SearchEditBox.m_wEditBox);
	}
	
	protected void OnSearchConfirmed(SCR_EditBoxComponent editBox, string text)
	{
		if (!m_Editor) return;
		
		GetGame().GetCallqueue().CallLater(SetFocusWidgetLater, 100, false);
	}
	
	protected void SetFocusWidgetLater()
	{
		m_Editor.SetFocusToCurrent();
	}
	
	protected void FocusCamo()
	{
		if (!m_wCamoCombobox) return;
	
		GetGame().GetWorkspace().SetFocusedWidget(m_wCamoCombobox);
	}
}