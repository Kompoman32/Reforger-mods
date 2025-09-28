class SCR_StanceInfo : SCR_InfoDisplayExtended
{	
	protected const float FADEOUT_PANEL_DELAY = 6; // Time until whole panel fades out

	protected ECharacterStance lastStance;
	protected int lastDynamicStance;
	protected float lastSpeed;
	
	protected bool m_bUseNewIcon = false;
		
	ref SCR_StanceInfoWidgets m_Widgets;
	
	protected ref SCR_FadeInOutAnimator m_StanceInfoPanelAnimator;
	
	//------------------------------------------------------------------------------------------------
	override void DisplayUpdate(IEntity owner, float timeSlice)
	{		
		super.DisplayUpdate(owner, timeSlice);
		
		UpdateStance();
		UpdateSpeed();
		
		if (m_StanceInfoPanelAnimator)
			m_StanceInfoPanelAnimator.Update(timeSlice);
	}
	
	//------------------------------------------------------------------------------------------------
	protected override void OnSettingsChanged()
	{	
		super.OnSettingsChanged();
		
		BaseContainer interfaceSettings = GetGame().GetGameUserSettings().GetModule(m_HUDManager.GetInterfaceSettingsClass());
		if (!interfaceSettings)
			return;
		
		bool newIconState;
		if (!interfaceSettings.Get("m_bShowStanceInfoNew", newIconState))
			return;
		
		m_bUseNewIcon = newIconState;
	}
	
	
	//------------------------------------------------------------------------------------------------
	void FadeElements()
	{
		m_StanceInfoPanelAnimator.FadeIn(true);
	}	
	
	protected void UpdateStance()
	{
		if (!m_Widgets || !m_CharacterController)
			return;
		
		Print(m_CharacterController.GetDynamicStance());

		ECharacterStance stance = m_CharacterController.GetStance();
		int dynamicStance = GetCharacterDynamicStanceCorrected();
		
		
		if (m_bUseNewIcon)
		{
			if (lastDynamicStance != dynamicStance) 
			{
				lastDynamicStance = dynamicStance;
				FadeElements();
			}
		}
		else 
		{
			if (lastStance != stance)
			{
				lastStance = stance;
				FadeElements();
			}
		}
		
		if (!m_Widgets.m_wStanceInfoPanel.IsVisible())
			return;
		
		if (m_bUseNewIcon)
		{
			m_Widgets.m_wStance.SetVisible(true);
			m_Widgets.m_wStance.LoadImageFromSet(0, "{507BDA0A5D363B53}UI/Textures/StanceIndicator/icons_stanceIndicator.imageset", dynamicStance.ToString());			
			
			m_Widgets.m_wStanceStand.SetVisible(false);
			m_Widgets.m_wStanceCrounch.SetVisible(false);
			m_Widgets.m_wStanceProne.SetVisible(false);
		}
		else 
		{
			m_Widgets.m_wStanceStand.SetVisible(stance == ECharacterStance.STAND);
			m_Widgets.m_wStanceCrounch.SetVisible(stance == ECharacterStance.CROUCH);
			m_Widgets.m_wStanceProne.SetVisible(stance == ECharacterStance.PRONE);

			m_Widgets.m_wStance.SetVisible(false);
		}
	}
	
	protected int GetCharacterDynamicStanceCorrected()
	{
		float dynamicStance = m_CharacterController.GetDynamicStance();
		
		if (dynamicStance < 0.01) return 0;
		
		if (dynamicStance < 0.19) return 1;
		
		return Math.Round(Math.Round(dynamicStance * 20) - 2);
	}
	
	protected void UpdateSpeed()
	{
		if (!m_Widgets || !m_CharacterController)
			return;
		
		float speed = Math.Round(m_CharacterController.GetMovementSpeed() * 10) / 2;
		
		if (speed > 0 && speed <= 10 && lastSpeed != speed) {
			lastSpeed = speed;
			
			FadeElements();
		}
		
		if (!m_Widgets.m_wStanceInfoPanel.IsVisible())
			return;

		m_Widgets.m_wStanceSpeed0.SetVisible(speed > 0);
		m_Widgets.m_wStanceSpeed1.SetVisible(speed > 2);
		m_Widgets.m_wStanceSpeed2.SetVisible(speed > 3);
		m_Widgets.m_wStanceSpeed3.SetVisible(speed > 4);
		m_Widgets.m_wStanceSpeed4.SetVisible(speed > 5);
		m_Widgets.m_wStanceSpeed5.SetVisible(speed > 6);
		m_Widgets.m_wStanceSpeed6.SetVisible(speed > 7);
		m_Widgets.m_wStanceSpeed7.SetVisible(speed > 8);
		m_Widgets.m_wStanceSpeed8.SetVisible(speed > 9);
		
		// Sprint
		m_Widgets.m_wStanceSpeed9.SetVisible(speed > 10);
	}

		
	//------------------------------------------------------------------------------------------------
	override bool DisplayStartDrawInit(IEntity owner)
	{
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(owner);
		if (!character)
			return false;

		// TODO idk, need this or not?		
		m_MenuManager = GetGame().GetMenuManager();
		if (!m_MenuManager)
			return false;

		
		UpdateStance();
		UpdateSpeed();
		
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override void DisplayStartDraw(IEntity owner)
	{	
		if (!m_wRoot)
			return;
		
		m_Widgets = new SCR_StanceInfoWidgets();
		m_Widgets.Init(m_wRoot);
		
		m_Widgets.m_wStanceInfoPanel.SetVisible(true);
		
		if (!m_StanceInfoPanelAnimator)
			m_StanceInfoPanelAnimator = new SCR_FadeInOutAnimator(m_Widgets.m_wStanceInfoPanel, UIConstants.FADE_RATE_FAST, UIConstants.FADE_RATE_SLOW, FADEOUT_PANEL_DELAY, true);
	}	
		
	//------------------------------------------------------------------------------------------------
	override void DisplayStopDraw(IEntity owner)
	{							
		// Destroy the animators
		m_StanceInfoPanelAnimator = null;
		
		m_wRoot = null;
	}
};