class RT_WS_ToolboxComponent: SCR_ToolboxMultilineComponent
{
	static const string CUSTOM_ITEM_DATA = "@CustomUnit@";
	
	[Attribute("NewButtonRow", UIWidgets.EditBox, "")]
	protected string m_sButtonRow;
	
	[Attribute("AddNewCombobox", UIWidgets.EditBox, "")]
	protected string m_sAddNewCombobox;
	
	[Attribute("AddNewButton", UIWidgets.EditBox, "")]
	protected string m_sAddNewButton;
	
	protected Widget m_wAddNewButton;
	protected Widget m_wAddNewCombobox;
	
	protected SCR_ButtonTextComponent m_AddNewButton;
	protected SCR_ComboBoxComponent m_AddNewCombobox;
	
	protected RT_WS_WavesSpawner_Attribute_Units m_Attribute;
	
	ref array<RT_SpawnerUnitRowComponent> m_aRows = new array<RT_SpawnerUnitRowComponent>;
	
	override void HandlerAttached(Widget w)
	{
		ResourceName oldElementLayout = m_ElementLayout;
		
		// This need to prevent parent logic of finding m_wButtonRow
		m_ElementLayout = "";
		super.HandlerAttached(w);
		m_ElementLayout = oldElementLayout;
		
		m_wButtonRow = w.FindAnyWidget(m_sButtonRow);
		
		if (!m_wButtonRow || !m_ElementLayout)
			return;

		CreateWidgets();
		HandleHeaderComponents(w);
		SetInitialState();
	}
	
	void InitByAttribute(RT_WS_WavesSpawner_Attribute_Units pAttribute)
	{
		m_Attribute = pAttribute;
		
		GenerateAddNewComboboxItems();
		
		GetGame().GetCallqueue().CallLater(DelayedInit, 300);
	}
	
	protected void HandleHeaderComponents(Widget w)
	{
		m_wAddNewButton = w.FindAnyWidget(m_sAddNewButton);
		m_wAddNewCombobox = w.FindAnyWidget(m_sAddNewCombobox);
		
		if (m_wAddNewButton)
		{
			m_AddNewButton = SCR_ButtonTextComponent.Cast(m_wAddNewButton.FindHandler(SCR_ButtonTextComponent));
			
			if (m_AddNewButton)
			{
				m_AddNewButton.m_OnClicked.Insert(OnAddNewButtonClick);
			}
		}
		
		if (m_wAddNewCombobox)
		{
			m_AddNewCombobox = SCR_ComboBoxComponent.Cast(m_wAddNewCombobox.FindHandler(SCR_ComboBoxComponent));
		}		
	}
	
	void DelayedInit()
	{
		foreach (int i, string name : m_Attribute.m_aSelectedResourceNames)
		{
			int availableIndex = m_Attribute.m_aAvailableResourceNames.Find(name);
			
			bool isCustom = availableIndex == -1;
			
			RT_SpawnerUnitRowComponent row;
			if (isCustom)
			{
				row = AddNewRow(name, name, m_aRows.Count(), true);	
			}
			else
			{
				row = AddNewRow(name, m_Attribute.m_aAvailableDisplayNames[availableIndex], m_aRows.Count(), false)
			}
			
			
			row.SetWeight(m_Attribute.m_aSelectedWeights[i]);
		}	
		
		RefreshPercents();
	}
	
//	override protected void CreateWidgets()
//	{
//		ClearWidgets();	
//		
//		float padding = m_fElementSpacing * 0.5;
//		LayoutSlot.SetPadding(m_wButtonRow, -padding, -padding, -padding, -padding);
//		foreach (int i, string name : m_aElementNames)
//		{
//			AddNewRow(name, i);
//		}
//		
//		RefreshPercents();
//	}
	
	protected void ChangeAttribute()
	{
		m_Attribute.m_aSelectedResourceNames.Clear();
		m_Attribute.m_aSelectedWeights.Clear();
		
		foreach (RT_SpawnerUnitRowComponent row: m_aRows)
		{
			if (!row.m_bIsValid) continue;
			
			m_Attribute.m_aSelectedResourceNames.Insert(row.GetResourceName());
			m_Attribute.m_aSelectedWeights.Insert(row.GetWeight());
		}
	}
	
	protected RT_SpawnerUnitRowComponent AddNewRow(string pResourceName, string pDisplayedName, int pRowIndex, bool pEnabled = true)
	{
		Widget button = GetGame().GetWorkspace().CreateWidgets(m_ElementLayout, m_wButtonRow);

		UniformGridSlot.SetRow(button, pRowIndex);
		UniformGridSlot.SetColumn(button, 0);

		RT_SpawnerUnitRowComponent comp = RT_SpawnerUnitRowComponent.Cast(button.FindHandler(RT_SpawnerUnitRowComponent));
		if (!comp) return null;

		m_aRows.Insert(comp);
		comp.SetWeight(0);
		
		comp.SetResourceName(pResourceName);
		comp.SetDisplayedName(pDisplayedName);
		
		comp.SetResourceNameEnabled(pEnabled);
		
		comp.m_OnResourceChange.Insert(OnRowChangeResourceName);
		comp.m_OnWeightChange.Insert(OnRowChangeWeight);
		comp.m_OnDeleteClick.Insert(OnRowDelete);
		
		return comp;
	}
	
	protected void OnRowChangeWeight(RT_SpawnerUnitRowComponent pRow, int pValue)
	{
		RefreshPercents();
		
		GetGame().GetCallqueue().CallLater(ChangeAttribute, 10);
	}
	
	protected void OnRowChangeResourceName(RT_SpawnerUnitRowComponent pRow, string pResourceName)
	{				
		GetGame().GetCallqueue().CallLater(ChangeAttribute, 10);
	}
	
	protected void OnRowDelete(RT_SpawnerUnitRowComponent comp, SCR_ModularButtonComponent button)
	{
		int index = m_aRows.Find(comp);
		
		if (index == -1) return;
		
		m_aRows.RemoveOrdered(index);
		
		comp.GetRootWidget().RemoveFromHierarchy();
		
		RefreshRowsPosition();
		RefreshPercents();
		
		GetGame().GetCallqueue().CallLater(ChangeAttribute, 10);
	}
	
	override protected void ClearWidgets()
	{
		foreach (RT_SpawnerUnitRowComponent row: m_aRows)
		{
			OnRowDelete(row, null);
		}
	}
	
	protected void GenerateAddNewComboboxItems()
	{
		if (!m_AddNewCombobox || !m_Attribute) return;
		
		array<ResourceName> pResourceNames = m_Attribute.m_aAvailableResourceNames;
		array<string> pDisplayNames = m_Attribute.m_aAvailableDisplayNames;
		
		m_AddNewCombobox.ClearAll();
		
		foreach (int i, ResourceName resourceName: pResourceNames)
		{
//			string name = string.Empty;
//			
//			if (i < pDisplayNames.Count()) {
//				name = pDisplayNames[i];
//			}
//			
//			if (!name)
//			{
//				name = SCR_StringHelper.FormatResourceNameToUserFriendly(resourceName);
//			}
//			
//			if (!name)
//			{
//				name = string.Format("NO_USE");
//			}
			
			
			m_AddNewCombobox.AddItem(pDisplayNames[i], true, new Tuple1<string>(resourceName));
		}
		
		m_AddNewCombobox.AddItem("Custom", true, new Tuple1<string>(CUSTOM_ITEM_DATA));
		
		m_AddNewCombobox.SetCurrentItem(0);
	}
	
	protected void RefreshPercents()
	{
		float weightSum = 0;
		
		foreach (RT_SpawnerUnitRowComponent row: m_aRows)
		{
			weightSum += row.GetWeight();
		}
		
		if (weightSum == 0) 
		{
			foreach (RT_SpawnerUnitRowComponent row: m_aRows)
			{
				row.SetPercentText(0);
			}
			return;
		}
		
		foreach (RT_SpawnerUnitRowComponent row: m_aRows)
		{
			int weight = row.GetWeight();
			
			row.SetPercentText((int)(weight/weightSum * 100));
		}
	}
	
	protected void RefreshRowsPosition()
	{		
		foreach (int i, RT_SpawnerUnitRowComponent row: m_aRows)
		{
			UniformGridSlot.SetRow(row.GetRootWidget(), i);
		}
	}
	
	protected void OnAddNewButtonClick(SCR_ButtonBaseComponent button)
	{
		string resourceName = string.Empty;
		string name = string.Empty;
		
		Tuple1<string> currentItemData = Tuple1<string>.Cast(m_AddNewCombobox.GetCurrentItemData());
		
		bool isCustom = currentItemData.param1 == CUSTOM_ITEM_DATA;
		
		if (!isCustom) 
		{
			resourceName = currentItemData.param1;
			name = m_AddNewCombobox.GetCurrentItem();
		}
		
		RT_SpawnerUnitRowComponent newRow = AddNewRow(resourceName, name, m_aRows.Count(), isCustom);
		
		if (isCustom) {
			newRow.SetResourceNameValid(false);
		}

		RefreshPercents();
		
		GetGame().GetCallqueue().CallLater(ChangeAttribute, 10);
	}	
	
	override protected void SetupLabel()
	{
		if (m_wLabelRoot)
			return;
		
		Widget contentRoot = m_wRoot.FindAnyWidget("HorizontalLayout");
		if (!contentRoot)
			return;
		
		m_wLabelRoot = GetGame().GetWorkspace().CreateWidgets(m_sLabelLayout, contentRoot);
		if (!m_wLabelRoot)
			return;
		
		TextWidget text = TextWidget.Cast(m_wLabelRoot.FindAnyWidget("Text"));
		if (!text)
			return;
		
		text.SetText(m_sLabel);
	}
	
	override void OnElementClicked(SCR_ButtonBaseComponent comp) {}
		
	override bool OnFocus(Widget w, int x, int y) { return false; }
	
	override bool OnFocusLost(Widget w, int x, int y) { return false; }
}

class RT_SpawnerUnitRowComponent: SCR_ScriptedWidgetComponent
{
	[Attribute("ResourceName", UIWidgets.EditBox, "")]
	protected string m_sResourceNameTemplate;
	
	[Attribute("Weight", UIWidgets.EditBox, "")]
	protected string m_sWeight;
	
	[Attribute("PercentText", UIWidgets.EditBox, "")]
	protected string m_sPercentText;
	
	[Attribute("DeleteButton", UIWidgets.EditBox, "")]
	protected string m_sDeleteButton;
	
	protected string m_sResourceName = string.Empty;
	protected string m_sResourceNameDisplayedValue = string.Empty;
	
	protected Widget m_wResourceName;
	protected Widget m_wWeight;
	protected Widget m_wPercentText;
	protected Widget m_wDeleteButton;
	
	protected SCR_EditBoxComponent m_ResourceName;
	protected EditBoxFilterComponent m_ResourceNameFilter;
	protected SCR_SliderComponent m_WeightSlider;
	protected TextWidget m_PercentText;
	protected SCR_ModularButtonComponent m_DeleteButton;
	
	protected bool m_bResourceNameEnabled = true;
	bool m_bIsValid = true;
	
	ref ScriptInvoker m_OnResourceChange = new ScriptInvoker(); 			// (RT_SpawnerUnitRowComponent comp, string pResourceName) 
	ref ScriptInvoker m_OnWeightChange = new ScriptInvoker(); 			// (RT_SpawnerUnitRowComponent comp, int pWeight) 
	ref ScriptInvoker m_OnDeleteClick = new ScriptInvoker(); 			// (RT_SpawnerUnitRowComponent comp, SCR_ModularButtonComponent pButton)
	
	protected override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		
		m_wResourceName = w.FindAnyWidget(m_sResourceNameTemplate);
		m_wWeight = w.FindAnyWidget(m_sWeight);
		m_wPercentText = w.FindAnyWidget(m_sPercentText);
		m_wDeleteButton = w.FindAnyWidget(m_sDeleteButton);
		
		m_ResourceName = SCR_EditBoxComponent.Cast(m_wResourceName.FindHandler(SCR_EditBoxComponent));
		if (m_ResourceName) 
		{
			m_ResourceNameFilter = EditBoxFilterComponent.Cast(m_ResourceName.m_wEditBox.FindHandler(EditBoxFilterComponent));
		}
		
		m_WeightSlider = SCR_SliderComponent.Cast(m_wWeight.FindHandler(SCR_SliderComponent));
		m_PercentText = TextWidget.Cast(m_wPercentText);
		m_DeleteButton = SCR_ModularButtonComponent.Cast(m_wDeleteButton.FindHandler(SCR_ModularButtonComponent));
		
		if (m_ResourceName) 
		{
			m_ResourceName.SetEnabled(m_bResourceNameEnabled);
		}
		
		AttachHandlers();
	}
		
	
	protected void AttachHandlers()
	{
		if (m_ResourceName)
		{
			m_ResourceName.m_OnWriteModeLeave.Insert(OnResourceNameChange);	
		}
		
		if (m_WeightSlider)
		{
			m_WeightSlider.m_OnChanged.Insert(OnWeightSliderChange);			
		}
		
		if (m_DeleteButton)
		{
			m_DeleteButton.m_OnClicked.Insert(OnDeleteClick)
		}
	}
	
	override void HandlerDeattached(Widget w)
	{
		super.HandlerDeattached(w);
		
		DetachHandlers();
	}
	
	protected void DetachHandlers()
	{
		if (m_ResourceName)
		{
			m_ResourceName.m_OnWriteModeLeave.Remove(OnResourceNameChange);			
		}
		
		if (m_WeightSlider)
		{
			m_WeightSlider.m_OnChanged.Remove(OnWeightSliderChange);			
		}
		
		if (m_DeleteButton)
		{
			m_DeleteButton.m_OnClicked.Remove(OnDeleteClick)
		}
	}
	
	protected void OnResourceNameChange(string pValue)
	{
		GetGame().GetCallqueue().Remove(SetResourceNameValid);
		
		m_bIsValid = false;
		
		if (SCR_StringHelper.IsEmptyOrWhiteSpace(pValue)) {
			if (m_ResourceNameFilter) 
			{
				m_ResourceNameFilter.m_OnInvalidInput.Invoke();
			}
			
			GetGame().GetCallqueue().CallLater(SetResourceNameValid, 300, false, false);
		}
		
		if (m_sResourceNameDisplayedValue == pValue) return;
		
		m_sResourceNameDisplayedValue = pValue;

		if (m_ResourceNameFilter) 
		{
			Resource r = Resource.Load(pValue);
			GetGame().GetCallqueue().CallLater(SetResourceNameValid, 300, false, r.IsValid());
		}
	}
	
	protected void OnWeightSliderChange(SCR_SliderComponent sliderComponent, float value) 
	{
		if (!m_WeightSlider) return;
		
		m_OnWeightChange.Invoke(this, (int)value);
	}
	
	protected void OnDeleteClick(SCR_ModularButtonComponent comp) 
	{
		m_OnDeleteClick.Invoke(this, comp);
	}
	
	void SetResourceNameValid(bool isValid)
	{
		if (!m_ResourceNameFilter) return;
		
		if (isValid)
		{
			m_ResourceNameFilter.m_OnValidInput.Invoke();
			m_sResourceName = m_sResourceNameDisplayedValue;
			m_OnResourceChange.Invoke(this, m_sResourceName);
		}
		else 
		{
			m_ResourceNameFilter.m_OnInvalidInput.Invoke();
		}
		
		m_bIsValid = isValid;
	}
	
	void SetResourceNameEnabled(bool pEnabled)
	{
		m_bResourceNameEnabled = pEnabled;
		
		if (!m_ResourceName) return;
		
		m_ResourceName.SetEnabled(m_bResourceNameEnabled);
	}
	
	void SetDisplayedName(string pValue)
	{
		if (!m_ResourceName) return;
		
		m_sResourceNameDisplayedValue = pValue;
		m_ResourceName.SetValue(pValue);
	}
	
	string GetDisplayedName()
	{
		if (!m_ResourceName) return string.Empty;
		
		return m_ResourceName.GetValue();
	}
	
	void SetResourceName(string pValue)
	{		
		m_sResourceName = pValue;
	}
	
	string GetResourceName()
	{		
		return m_sResourceName;
	}
	
	int GetWeight()
	{
		if (!m_WeightSlider) return 0;
		
		return m_WeightSlider.GetValue();
	}
	
	void SetWeight(int pWeight)
	{
		if (!m_WeightSlider) return ;
		
		m_WeightSlider.SetValue(pWeight);
	}
	
	int GetPercentText()
	{
		if (!m_PercentText) return string.Empty;
		
		return m_PercentText.GetText();
	}
	
	void SetPercentText(int pPercent)
	{
		if (!m_PercentText) return;
		
		m_PercentText.SetTextFormat("%1%", pPercent);
	}
}