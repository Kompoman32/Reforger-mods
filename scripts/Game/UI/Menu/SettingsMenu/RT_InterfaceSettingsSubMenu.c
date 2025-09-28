modded class SCR_InterfaceSettingsSubMenu : SCR_SettingsSubMenuBase
{
	
	protected const string STANCEINFO_SPINBOX_WIDGET_NAME = "StanceInfo";
	protected const string STANCEINFO_NEW_SPINBOX_WIDGET_NAME = "StanceInfoNew";
	
	//------------------------------------------------------------------------------------------------
	override void OnTabCreate(Widget menuRoot, ResourceName buttonsLayout, int index)
	{
		super.OnTabCreate(menuRoot, buttonsLayout, index);
		
		// Setup StanceInfo
		SetupSpinboxes(STANCEINFO_SPINBOX_WIDGET_NAME, "m_bShowStanceInfo");
		if (m_SpinnerBoxComp)
			m_SpinnerBoxComp.m_OnChanged.Insert(OnSpinBoxChange);
		
		// Setup StanceInfo
		SetupSpinboxes(STANCEINFO_NEW_SPINBOX_WIDGET_NAME, "m_bShowStanceInfoNew");
		if (m_SpinnerBoxComp)
			m_SpinnerBoxComp.m_OnChanged.Insert(OnSpinBoxChange);
	}
	
}

modded class SCR_InterfaceSettings : ModuleGameSettings
{
	[Attribute("1")]
	bool m_bShowStanceInfo;
	
	[Attribute("0")]
	bool m_bShowStanceInfoNew;
}