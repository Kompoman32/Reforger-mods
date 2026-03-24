modded class SCR_LoadoutRequestUIComponent : SCR_DeployRequestUIBaseComponent
{
	protected SCR_ButtonImageComponent m_ChooseFaceButton;
	
	IEntity m_SelectedEntity;
	
	IEntity GetPreviewedEntity()
	{
		return m_PreviewedEntity;
	}
	
	void ResetLoadoutPreview()
	{
		ResetLoadoutPreviewBackground();
		m_PreviewComp.GetPreviewManagerEntity().SetPreviewItem(m_PreviewComp.GetItemPreviewWidget(), GetPreviewedEntity(), null, true);
	}
	
	void ResetLoadoutPreviewBackground()
	{
		if (!m_SelectedEntity) return;
		
		ImageWidget background = ImageWidget.Cast((m_wLoadoutSelector.GetParent()).GetParent().FindAnyWidget("Background"));
		if (!background) return;
		
		SCR_CharacterFactionAffiliationComponent fac = SCR_CharacterFactionAffiliationComponent.Cast(m_SelectedEntity.FindComponent(SCR_CharacterFactionAffiliationComponent));
		if (!fac) return;
		
		TimeAndWeatherManagerEntity tmw = ((ChimeraWorld)GetGame().GetWorld()).GetTimeAndWeatherManager();
		if (!tmw) return;
		
		int dayTime = tmw.GetCurrentDayTimeUIInfo(uiInfo);
		SCR_Faction faction = SCR_Faction.Cast(fac.GetAffiliatedFaction());
		if (!faction) return;		
		
		WeatherStateTransitionManager transitionManager = WeatherStateTransitionManager.Cast(tmw.GetTransitionManager());
		m_sWeatherModifier = transitionManager.GetCurrentState().GetStateName();
		
		if (!m_sDayModifierStrings.IsIndexValid(dayTime))
			return;
		
		m_sDayModifier = m_sDayModifierStrings[dayTime];
		
		if (!faction) 
			return;
		
		m_sLoadoutImageSet = faction.GetFactionBackground();
		
		if (m_sWeatherModifier == "Rainy")
			m_sLoadoutImageName = m_sDayModifier + m_sWeatherModifier;	
		else
			m_sLoadoutImageName = m_sDayModifier;

		background.LoadImageFromSet(0, m_sLoadoutImageSet, m_sLoadoutImageName);
		
		background.Update();
	}
	
	protected override void SetLoadoutPreview(SCR_BasePlayerLoadout loadout)
	{
		super.SetLoadoutPreview(loadout);
		
		VisualIdentity currentFace = SCR_PlayerData.GetCurrentUserVisual(m_PreviewedEntity);
		
		RT_CF_Utils.SetIdentity(m_PreviewedEntity, currentFace);
		ResetLoadoutPreview();
	}
	
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		
		FixLoadoutPreview(w);
		
		m_ChooseFaceButton = SCR_ButtonImageComponent.GetButtonImage("ChooseFaceButton", w);
	}
	
	protected void FixLoadoutPreview(Widget w)
	{
//		// Some hack to not do it on RoleSelection window
//		// Too bad hack, but... whatever...
//		for (int i = 0; i < 4; i++)
//		{
//			if (!w) break;
//			w = w.GetParent();
//		}
//		
//		// Just not do anything in RoleSelection
//		if (w && w.GetName() == "RoleSelection") return;
		
		
		if (GetGame().GetLoadoutManager())
		{
			SCR_BasePlayerLoadout loadout = GetGame().GetLoadoutManager().GetLoadoutByIndex(0);
			
			if (loadout)
			{
				SetLoadoutPreview(loadout);
			}
		}
			
	}
	
	protected override void OnListExpand(SCR_DeployRequestUIBaseComponent component, bool expanded)
	{
		super.OnListExpand(component, expanded);
		
		if (component != this)
		{
			expanded = false;
		}
		
		if (m_ChooseFaceButton)
		{
			m_ChooseFaceButton.SetVisible(expanded, false);
		}
	}
}


modded class SCR_ContextMenuButtonEditorUIComponent : ScriptedWidgetComponent
{
	ref RT_CF_EditableEntityUIInfo m_FaceInfo;
	
	void SetData(RT_CF_EditableEntityUIInfo info, string shortcutAction, bool canBePerformed)
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