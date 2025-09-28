class RT_FaceEditorBrowserDialogUI : MenuRootBase {
	const string WIDGET_BUTTON_CLOSE = "CloseButton";
	
	const string WIDGET_EDITOR_WINDOW = "Window";
	RT_FacesBrowserEditorUIComponent m_Editor;
	
	void CloseSelf()
	{
		GetManager().CloseMenu(this);
	}
	
	Widget GetWidgetByName(string widgetName)
	{
		Widget root = GetRootWidget();
		if (!root) return null;
		
		WorkspaceWidget workspace = GetGame().GetWorkspace();
		if (!workspace) return null;
		
		return root.FindAnyWidget(widgetName);
	}
	
	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		
		Widget rootWidget = GetRootWidget();
		if (!rootWidget) return;
		
		Widget w = rootWidget.FindAnyWidget(WIDGET_EDITOR_WINDOW);
		
		if (w)
		{
			m_Editor = RT_FacesBrowserEditorUIComponent.Cast(w.FindHandler(RT_FacesBrowserEditorUIComponent));
		}
		
		ScriptInvoker onClose = ButtonActionComponent.GetOnAction(rootWidget, WIDGET_BUTTON_CLOSE);
		if (onClose) onClose.Insert(CloseSelf);
	}	
}