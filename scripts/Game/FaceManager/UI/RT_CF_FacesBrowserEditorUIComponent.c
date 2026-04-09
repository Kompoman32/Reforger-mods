class RT_CF_FacesBrowserEditorUIComponent : MenuRootSubComponent
{
	protected string m_sFacesList = "FacesList";
	protected string m_sLoadoutSelector = "LoadoutSelector";
	protected string m_sSearchEditBoxName = "EditBoxSearch";
	protected string m_sCurrentFaceText = "CurrentFaceText";
	protected string m_sLoadoutSelectorGallery = "Selector";
	protected string m_sLoadoutPreview = "LoadoutPreview";
	protected string m_sFaceButtonLayout = "{A865184601CD77A8}UI/layouts/Editor/ContextMenu/ActionMenu/ActionMenu_Button.layout";
	protected string m_sCamoComboboxLayout = "VerticalLayoutCamo";
	protected string m_sCamoCombobox = "CamoComboBox";
	
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
	
	protected VerticalLayoutWidget m_wCamoComboboxLayout;
	protected ButtonWidget m_wCamoCombobox;
	protected SCR_ComboBoxComponent m_CamoCombobox;		
	
	protected ref array<ref RT_CF_EditableEntityUIInfo> m_aAllFaces = {};
	
	protected IEntity m_SelectedEntity;
	VisualIdentity m_CurrentFace;
	ResourceName m_CurrentCamo;
	
	override protected void HandlerAttachedScripted(Widget w)
	{
		super.HandlerAttachedScripted(w);
		
		m_wFacesList = VerticalLayoutWidget.Cast(w.FindAnyWidget(m_sFacesList));

		m_wCurrentFaceText = TextWidget.Cast(w.FindAnyWidget(m_sCurrentFaceText));
		
		PlayerController pc = GetGame().GetPlayerController();
		
		if (pc)
		{
			m_PlyLoadoutComp = SCR_PlayerLoadoutComponent.Cast(pc.FindComponent(SCR_PlayerLoadoutComponent));
			m_PlyFactionAffilComp = SCR_PlayerFactionAffiliationComponent.Cast(pc.FindComponent(SCR_PlayerFactionAffiliationComponent));
		}
		
		m_LoadoutManager = GetGame().GetLoadoutManager();
		
		m_SearchEditBox = SCR_EditBoxComponent.GetEditBoxComponent(m_sSearchEditBoxName, w, true);
		if (m_SearchEditBox)
		{
			m_SearchEditBox.m_OnConfirm.Insert(OnSearchConfirmed);
			m_SearchEditBox.m_OnFocusChangedEditBox.Insert(OnSearchFocusChanged);
		}
		
		m_wLoadoutSelector = w.FindAnyWidget(m_sLoadoutSelector);
		
		m_wCamoComboboxLayout = VerticalLayoutWidget.Cast(w.FindAnyWidget(m_sCamoComboboxLayout));
		m_wCamoCombobox = ButtonWidget.Cast(w.FindAnyWidget(m_sCamoCombobox));
		
		if (m_wCamoCombobox)
		{
			m_CamoCombobox = SCR_ComboBoxComponent.Cast(m_wCamoCombobox.FindHandler(SCR_ComboBoxComponent));
			m_CamoCombobox.m_OnChanged.Insert(OnCamoComboBoxChanged);
		}
		
		SetupPreview();
		SetupList();
	}
	
	protected void SetupPreview()
	{
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
					RT_CF_EditableEntityUIInfo info = new RT_CF_EditableEntityUIInfo(SCR_UIInfo.CreateInfo(head));
				
					m_wCurrentFaceText.SetText(info.GetName());
				}
			}
		}
	}
	
	void SetPreviewHead(RT_CF_EditableEntityUIInfo info)
	{
		if (!m_wCurrentFaceText) return;	
		
		IEntity previewEntity = m_LoadoutSelector.GetPreviewedEntity();
		
		if (!previewEntity) return;
		
		m_CurrentFace = info.m_Visual;
		RT_CF_Utils.SetIdentity(previewEntity, info.m_Visual);
		m_LoadoutSelector.ResetLoadoutPreview();	
	}
	
	protected void SetupList()
	{		
		if (!m_wFacesList) return;

		m_aAllFaces.Clear();		
		GetAllFacesPrefabs(m_aAllFaces);
		
		FilterEntries(true);
	}
	
	protected void CreateListItem(RT_CF_EditableEntityUIInfo info)
	{
		Widget cw = GetGame().GetWorkspace().CreateWidgets(m_sFaceButtonLayout, m_wFacesList);
		
		if (!cw) return;
		
		cw.SetFlags(WidgetFlags.CLIPCHILDREN);
		
		SCR_ContextMenuButtonEditorUIComponent cmHandler = SCR_ContextMenuButtonEditorUIComponent.Cast(cw.FindHandler(SCR_ContextMenuButtonEditorUIComponent));
	
		if (!cmHandler) return;
	
		cmHandler.SetData(info, "", true);
		cmHandler.SetIconVisible(false);
		cmHandler.SetShortcutVisible(false);
	
		if (m_CurrentFace && info.m_Visual)
		{
			if (info.m_Head == m_CurrentFace.GetHead() || (info.m_aCamos.Contains(m_CurrentFace.GetHead()))) 
			{
				GetGame().GetCallqueue().Remove(FocusItemLater);
				GetGame().GetCallqueue().CallLater(FocusItemLater, 100, false, cw);	
			}
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
	
	void FocusItemByName(string faceName)
	{
		Widget w = m_wFacesList.GetChildren();
		
		while (w)
		{
			SCR_ContextMenuButtonEditorUIComponent cmHandler = SCR_ContextMenuButtonEditorUIComponent.Cast(w.FindHandler(SCR_ContextMenuButtonEditorUIComponent));
			
			if (cmHandler && cmHandler.m_FaceInfo)
			{	
				if (cmHandler.m_FaceInfo.m_Head == faceName || (cmHandler.m_FaceInfo.m_aCamos.Contains(faceName)))
				{
					// its camo
					if (cmHandler.m_FaceInfo.m_Head != faceName)
					{
						m_CurrentCamo = faceName;
					}
					
					GetGame().GetCallqueue().Remove(FocusItemLater);
					GetGame().GetCallqueue().CallLater(FocusItemLater, 100, false, w);	
				}
			}
			
			w = w.GetSibling();
		}
	}
	
	protected void FocusItemLater(Widget w)
	{
		SCR_ContextMenuButtonEditorUIComponent cmHandler = SCR_ContextMenuButtonEditorUIComponent.Cast(w.FindHandler(SCR_ContextMenuButtonEditorUIComponent));

		if (cmHandler && cmHandler.m_FaceInfo)
		{
			SetupCamoComboBox(cmHandler.m_FaceInfo);
			
			m_CurrentFace = cmHandler.m_FaceInfo.m_Visual;
			m_wCurrentFaceText.SetText(cmHandler.m_FaceInfo.GetName());	
		}
		
		GetGame().GetWorkspace().SetFocusedWidget(w);
	}
	
	protected void onAction(Widget widget, float value, EActionTrigger reason)
	{
		SCR_ContextMenuButtonEditorUIComponent cmHandler = SCR_ContextMenuButtonEditorUIComponent.Cast(widget.FindHandler(SCR_ContextMenuButtonEditorUIComponent));
		
		if (!cmHandler) return;
		
		RT_CF_EditableEntityUIInfo info = cmHandler.m_FaceInfo;
		
		if (!info) return;
		
		m_CurrentCamo = string.Empty;
		m_wCurrentFaceText.SetText(info.GetName());	
		SetPreviewHead(info);
		SetupCamoComboBox(info);
	}
	
	protected void SetupCamoComboBox(RT_CF_EditableEntityUIInfo info)
	{
		if (m_wCamoComboboxLayout)
		{
			m_wCamoComboboxLayout.SetVisible(info.m_aCamos && info.m_aCamos.Count() != 0);
		}
		
		if (m_CamoCombobox)
		{
			m_CamoCombobox.ClearAll();
			
			if (info.m_aCamos)
			{
				m_CamoCombobox.AddItem("No Camo", true, info);
				
				m_CamoCombobox.SetCurrentItem(0);
				
				foreach (int i, ResourceName camo: info.m_aCamos)
				{
					m_CamoCombobox.AddItem(info.GetHeadCamoName(i), true, info);
					
					if (camo == m_CurrentCamo)
					{
						m_CamoCombobox.SetCurrentItem(i + 1);
					}
				}
			}
		}
	}
	
	protected void OnCamoComboBoxChanged(SCR_ComboBoxComponent comp, int value)
	{
		RT_CF_EditableEntityUIInfo info = RT_CF_EditableEntityUIInfo.Cast(comp.GetItemData(value));
				
		IEntity previewEntity = m_LoadoutSelector.GetPreviewedEntity();
		
		if (!previewEntity) return;
		
		VisualIdentity vi = VisualIdentity.Cast(info.m_Visual.Clone());
		
		if (value > 0)
		{
			m_CurrentCamo = info.m_aCamos.Get(value - 1);
			vi.SetHead(m_CurrentCamo);
		}
		else 
		{
			m_CurrentCamo = string.Empty;
			vi.SetHead(info.m_Visual.GetHead());
		}
		
		vi.SetBody(info.m_Visual.GetBody());
		
		
		RT_CF_Utils.SetIdentity(previewEntity, vi);
		m_LoadoutSelector.ResetLoadoutPreview();
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
	
	void GetAllFacesPrefabs(out notnull array<ref RT_CF_EditableEntityUIInfo> heads)
	{
		array<ref RT_CF_EditableEntityUIInfo> initialHeads = {};
		ref map<ResourceName, ref RT_CF_EditableEntityUIInfo> visuals = new map<ResourceName, ref RT_CF_EditableEntityUIInfo>();		
		
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
						
						RT_CF_EditableEntityUIInfo info = visuals.Get(head);
						
						array<ref ResourceName> headCamos = {};
						
						RT_CF_Utils.GetHeadCamos(fv, headCamos, true);						
						
						if (info) 
						{
							foreach(ResourceName camo: headCamos)
							{
								if (camo != head && !info.m_aCamos.Contains(camo))
								{
									info.m_aCamos.Insert(camo);
								}
							}

							continue;
						}
						
						info = new RT_CF_EditableEntityUIInfo(SCR_UIInfo.CreateInfo(head), scrF, fv, headCamos);
		
						visuals.Insert(head, info);
						initialHeads.Insert(info);						
					}
				}
			}
		}		
		
		SortInfos(initialHeads, heads);		
	}
	
	protected void SortInfos(array<ref RT_CF_EditableEntityUIInfo> from, out notnull array<ref RT_CF_EditableEntityUIInfo> to)
	{
		map<string, ref array<ref RT_CF_EditableEntityUIInfo>> infoMap = new map<string, ref array<ref RT_CF_EditableEntityUIInfo>>();
		array<string> names = {};
		
		
		foreach(RT_CF_EditableEntityUIInfo info : from)
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
			array<ref RT_CF_EditableEntityUIInfo> infos = infoMap.Get(name);
			
			foreach(RT_CF_EditableEntityUIInfo info : infos)
			{
				to.Insert(info);
			}
		}		
	}
	
	void SetSelectedEntity(IEntity entity)
	{
		m_SelectedEntity = entity;
		
		if (m_LoadoutSelector)
		{
			m_LoadoutSelector.m_SelectedEntity = entity;
		}
	}
	
	IEntity GetSelectedEntity()
	{
		return m_SelectedEntity;
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
		
		foreach(RT_CF_EditableEntityUIInfo info: m_aAllFaces)
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
