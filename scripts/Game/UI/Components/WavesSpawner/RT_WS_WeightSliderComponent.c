class RT_WS_WeightSliderComponent : SCR_SliderComponent
{
	protected bool m_bHasActionListeners = false;
	
	override bool OnFocus(Widget w, int x, int y) {		
		if (GetGame().GetInputManager().GetLastUsedInputDevice() == EInputDeviceType.GAMEPAD)
		{
			AddActionListeners();
			return false;
		}
		
		return super.OnFocus(w,x,y);
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{		
		RemoveActionListeners();
		
		return super.OnFocusLost(w,x,y);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void AddActionListeners()
	{
		if (m_bHasActionListeners)
			return;
		
		GetGame().GetInputManager().AddActionListener(UIConstants.MENU_ACTION_SELECT, EActionTrigger.DOWN, OnFocusSliderGamepad);
		
		m_bHasActionListeners = true;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void RemoveActionListeners()
	{
		if (!m_bHasActionListeners)
			return;
		
		GetGame().GetInputManager().RemoveActionListener(UIConstants.MENU_ACTION_SELECT, EActionTrigger.DOWN, OnFocusSliderGamepad);
		GetGame().GetInputManager().RemoveActionListener(UIConstants.MENU_ACTION_BACK, EActionTrigger.DOWN, OnUnfocusSliderGamepad);
		
		m_bHasActionListeners = false;
	}
	
	protected void OnFocusSliderGamepad()
	{	
		GetGame().GetWorkspace().SetFocusedWidget(m_wSlider);
		
		GetGame().GetInputManager().AddActionListener(UIConstants.MENU_ACTION_BACK, EActionTrigger.DOWN, OnUnfocusSliderGamepad);
		
		SCR_MenuHelper.SetActiveWidgetInteractionState(true, 0);
	}
	
	protected void OnUnfocusSliderGamepad()
	{
		GetGame().GetWorkspace().SetFocusedWidget(null);
		GetGame().GetWorkspace().SetFocusedWidget(m_wRoot);
		
		GetGame().GetInputManager().RemoveActionListener(UIConstants.MENU_ACTION_BACK, EActionTrigger.DOWN, OnUnfocusSliderGamepad);
		
		SCR_MenuHelper.SetActiveWidgetInteractionState(false, 0);
	}
}