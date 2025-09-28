class RT_FacesBrowserEditorUIComponent : MenuRootSubComponent
{
	protected string m_sFacesList = "FacesList";
	protected string m_sLoadoutSelector = "LoadoutSelector";
	protected string m_sSearchEditBoxName = "EditBoxSearch";
	protected string m_sCurrentFaceText = "CurrentFaceText";
	protected string m_sLoadoutSelectorGallery = "Selector";
	protected string m_sLoadoutPreview = "LoadoutPreview";
	protected string m_sFaceButtonLayout = "{A865184601CD77A8}UI/layouts/Editor/ContextMenu/ActionMenu/ActionMenu_Button.layout";
	
	protected VerticalLayoutWidget m_wFacesList;
	
	protected SCR_LoadoutManager m_LoadoutManager;
	protected SCR_PlayerLoadoutComponent m_PlyLoadoutComp;
	protected SCR_PlayerFactionAffiliationComponent m_PlyFactionAffilComp;
	
	protected Widget m_wLoadoutSelector;
	protected SCR_LoadoutRequestUIComponent m_LoadoutSelector;
	
	protected SCR_EditBoxComponent m_SearchEditBox;
	protected string m_sCurrentSearchText;
	
	protected TextWidget m_wCurrentFaceText;
	
	protected Widget m_wLoadoutSelectorGallery;
	protected SCR_LoadoutGallery m_LoadoutSelectorGallery;
	
	protected Widget m_wLoadoutPreview;
	protected SCR_LoadoutPreviewComponent m_PreviewComp;
	
	
	protected ref array<ref RT_EditableEntityUIInfo> m_aAllFaces = {};
	

	VisualIdentity m_CurrentFace;
	
	override protected void HandlerAttachedScripted(Widget w)
	{
		super.HandlerAttachedScripted(w);
		
		m_wFacesList = VerticalLayoutWidget.Cast(w.FindAnyWidget(m_sFacesList));

		m_wCurrentFaceText = TextWidget.Cast(w.FindAnyWidget(m_sCurrentFaceText));
		
		PlayerController pc = GetGame().GetPlayerController();
		m_PlyLoadoutComp = SCR_PlayerLoadoutComponent.Cast(pc.FindComponent(SCR_PlayerLoadoutComponent));
		m_PlyFactionAffilComp = SCR_PlayerFactionAffiliationComponent.Cast(pc.FindComponent(SCR_PlayerFactionAffiliationComponent));
		m_LoadoutManager = GetGame().GetLoadoutManager();
		
		m_SearchEditBox = SCR_EditBoxComponent.GetEditBoxComponent(m_sSearchEditBoxName, w, true);
		if (m_SearchEditBox)
		{
			m_SearchEditBox.m_OnConfirm.Insert(OnSearchConfirmed);
			m_SearchEditBox.m_OnFocusChangedEditBox.Insert(OnSearchFocusChanged);
		}
		
		
		SetupPreview(w);
		SetupList();
	}
	
	protected void SetupPreview(Widget w)
	{
		m_wLoadoutSelector = w.FindAnyWidget(m_sLoadoutSelector);
		
		if (m_wLoadoutSelector)
		{
			m_LoadoutSelector = SCR_LoadoutRequestUIComponent.Cast(m_wLoadoutSelector.FindHandler(SCR_LoadoutRequestUIComponent));
			m_wLoadoutSelectorGallery = m_wLoadoutSelector.FindAnyWidget(m_sLoadoutSelectorGallery);
			
			if (m_wLoadoutSelectorGallery)
				m_LoadoutSelectorGallery = SCR_LoadoutGallery.Cast(m_wLoadoutSelector.FindHandler(SCR_LoadoutGallery));
				
			m_wLoadoutPreview = m_wLoadoutSelector.FindAnyWidget(m_sLoadoutPreview);
			if (m_wLoadoutPreview)
				m_PreviewComp = SCR_LoadoutPreviewComponent.Cast(m_wLoadoutPreview.FindHandler(SCR_LoadoutPreviewComponent));
			
			if (m_LoadoutSelector)
				m_LoadoutSelector.RefreshLoadoutPreview();
			
			if (m_LoadoutSelector.GetPreviewedEntity())
			{
				SCR_CharacterIdentityComponent iden = SCR_CharacterIdentityComponent.Cast(m_LoadoutSelector.GetPreviewedEntity().FindComponent(SCR_CharacterIdentityComponent));
				if (!iden) return;	
		
				m_CurrentFace = iden.GetIdentity().GetVisualIdentity();
				
				if (!m_CurrentFace) return;
				
				string head = m_CurrentFace.GetHead();	
				if (head)
				{
					RT_EditableEntityUIInfo info = new RT_EditableEntityUIInfo(SCR_UIInfo.CreateInfo(head), null, null);
				
					m_wCurrentFaceText.SetText(info.GetName());
				}
			}
		}
	}
	
	protected void SetPreviewhead(VisualIdentity face)
	{
		IEntity previewEntity = m_LoadoutSelector.GetPreviewedEntity();
		
		if (!previewEntity) return;
		
		m_CurrentFace = face;
		RT_CF_Utils.SetIdentity(previewEntity, face);
		m_LoadoutSelector.ResetLoadoutPreview();	
	}
	
	protected void SetupList()
	{		
		if (!m_wFacesList) return;

		m_aAllFaces.Clear();		
		GetAllFacesPrefabs(m_aAllFaces);
		
		FilterEntries(true);
	}
	
	protected void CreateListItem(RT_EditableEntityUIInfo info)
	{
		Widget cw = GetGame().GetWorkspace().CreateWidgets(m_sFaceButtonLayout, m_wFacesList);
		
		if (!cw) return;
		
		cw.SetFlags(WidgetFlags.CLIPCHILDREN);
		
		SCR_ContextMenuButtonEditorUIComponent cmHandler = SCR_ContextMenuButtonEditorUIComponent.Cast(cw.FindHandler(SCR_ContextMenuButtonEditorUIComponent));
	
		if (!cmHandler) return;
	
		cmHandler.SetData(info, "", true);
		cmHandler.SetIconVisible(false);
		cmHandler.SetShortcutVisible(false);
	
		if (m_CurrentFace && info.m_Visual && (m_CurrentFace.GetHead().Compare(info.m_Visual.GetHead(), false) == 0))
		{
			GetGame().GetCallqueue().Remove(FocusItemLater);
			GetGame().GetCallqueue().CallLater(FocusItemLater, 100, false, cw);
		}
	
	
		ButtonActionComponent buttonHandler = ButtonActionComponent.Cast(cw.FindHandler(ButtonActionComponent));
	
		buttonHandler.GetOnAction().Insert(onAction);
	
		TextWidget t = TextWidget.Cast(cw.FindAnyWidget("Text"));
		
		if (t) 
		{
			t.SetFlags(WidgetFlags.INHERIT_CLIPPING);
			t.SetZOrder(0);
		}
		
		
	}
	
	protected void FocusItemLater(Widget w)
	{
		GetGame().GetWorkspace().SetFocusedWidget(w);
	}
	
	protected void onAction(Widget widget, float value, EActionTrigger reason)
	{
		SCR_ContextMenuButtonEditorUIComponent cmHandler = SCR_ContextMenuButtonEditorUIComponent.Cast(widget.FindHandler(SCR_ContextMenuButtonEditorUIComponent));
		
		if (!cmHandler) return;
		
		RT_EditableEntityUIInfo info = cmHandler.m_FaceInfo;
		
		if (!info) return;
		
		m_wCurrentFaceText.SetText(info.GetName());		
		SetPreviewhead(info.m_Visual);
	}
	
	protected void ClearList()
	{
		if (!m_wFacesList) return;
		
		Widget w = m_wFacesList.GetChildren();
		
		while (w)
		{
			w.RemoveFromHierarchy();
			w = m_wFacesList.GetChildren();
		}
	}
	
	void GetAllFacesPrefabs(out notnull array<ref RT_EditableEntityUIInfo> heads)
	{
		array<ref RT_EditableEntityUIInfo> initialHeads = {};
		ref set<ResourceName> visuals = new set<ResourceName>();		
		
		FactionManager factionManager = GetGame().GetFactionManager();
		if (!factionManager) return;
		
		array<Faction> factions = {};
		factionManager.GetFactionsList(factions);
		
		foreach(Faction f: factions)
		{
			SCR_Faction scrF = SCR_Faction.Cast(f);
			if (scrF)
			{
				FactionIdentity factionIdentity = FactionIdentity.Cast(scrF.GetFactionIdentity());
				if (factionIdentity) {
					array<ref VisualIdentity> factionIdentityVisuals = {};
					factionIdentity.GetVisualIdentities(factionIdentityVisuals);
					
					foreach(VisualIdentity fv: factionIdentityVisuals)
					{
						string head = fv.GetHead();
						
						if (visuals.Contains(head)) continue;
		
						visuals.Insert(head);
						
						initialHeads.Insert(
							new RT_EditableEntityUIInfo(SCR_UIInfo.CreateInfo(head), scrF, fv)
						);						
					}
				}
			}
		}
		
		SortInfos(initialHeads, heads);		
	}
	
	protected void SortInfos(array<ref RT_EditableEntityUIInfo> from, out notnull array<ref RT_EditableEntityUIInfo> to)
	{
		map<string, ref array<ref RT_EditableEntityUIInfo>> infoMap = new map<string, ref array<ref RT_EditableEntityUIInfo>>();
		array<string> names = {};
		
		
		foreach(RT_EditableEntityUIInfo info : from)
		{
			string name = info.GetName();
			if (!infoMap.Get(name))
			{
				names.Insert(name);
				infoMap.Set(name, {info});
				continue;
			}
			
			infoMap.Get(name).Insert(info);			
		}
		
		names.Sort();
		
		foreach(string name : names)
		{
			array<ref RT_EditableEntityUIInfo> infos = infoMap.Get(name);
			
			foreach(RT_EditableEntityUIInfo info : infos)
			{
				to.Insert(info);
			}
		}		
	}
	
	protected void OnSearchFocusChanged(SCR_EditBoxComponent editBoxComponent, EditBoxWidget editBoxWidget, bool focus)
	{
//		TODO		
//		if (focus != m_bSearchIsToggled)
//			m_bSearchIsToggled = focus	
	}
	
	protected void OnSearchConfirmed(SCR_EditBoxComponent editBox, string text)
	{
		string search = text.Trim();
		m_sCurrentSearchText = search;
		
		
		FilterEntries();
		
		//If using gamepad make sure search box is no longer selected
//		if (m_bUsingGamePad && m_wFocusedCard)
//			GamePadToggleSearch(false);
	}
	
	protected void ResetSearch()
	{
		if (m_SearchEditBox)
		{
			m_SearchEditBox.SetValue(string.Empty);	
		}
		
		m_sCurrentSearchText = string.Empty;
		
//		if (m_bUsingGamePad)
//			GamePadToggleSearch(false);
	}
	
	protected void FilterEntries(bool resetSearch  = false)
	{
		// Search
		if (resetSearch)
		{
			ResetSearch();
		}
		else
		{
			string searchInputText = m_SearchEditBox.GetValue().Trim();
			if (!searchInputText.IsEmpty() && searchInputText != m_sCurrentSearchText)
			{
				m_sCurrentSearchText = searchInputText;
			}
		}
		
		ClearList();
		
		string searchText = m_sCurrentSearchText;
		searchText.ToLower();
		
		foreach(RT_EditableEntityUIInfo info: m_aAllFaces)
		{			
			if (resetSearch )
			{		
				CreateListItem(info);
				continue;
			}
			
			string infoName = info.GetName();
			infoName.ToLower();
			
			if (infoName.Contains(searchText))
			{		
				CreateListItem(info);
			}
		}	
	}
}
