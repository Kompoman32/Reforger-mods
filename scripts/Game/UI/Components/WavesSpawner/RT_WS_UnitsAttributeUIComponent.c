class RT_WS_UnitsAttributeUIComponent: SCR_BaseEditorAttributeUIComponent
{
	protected Widget m_wRoot;
	
	protected Widget m_wToolBoxComponent;
	
	protected RT_WS_WavesSpawner_Attribute_Units m_RTAttribute;
	protected RT_WS_ToolboxComponent m_ToolBoxComponent;

	protected ref SCR_BaseEditorAttributeFloatStringValues m_ButtonBoxData;
	
	protected bool m_bButtonValueInitCalled;
	
	override void Init(Widget w, SCR_BaseEditorAttribute attribute)
	{
		m_wRoot = w;
		
		m_AttributeManager = SCR_AttributesManagerEditorComponent.Cast(SCR_AttributesManagerEditorComponent.GetInstance(SCR_AttributesManagerEditorComponent));
		
		if (m_AttributeManager && !m_AttributeManager.m_RT_WS_DelayedUnitsAttribute)
		{
			m_AttributeManager.m_RT_WS_DelayedUnitsAttributeInit.Insert(DelayedUnitsAttributeInit);
		}
		
		m_RTAttribute = RT_WS_WavesSpawner_Attribute_Units.Cast(attribute);
		
		if (!m_RTAttribute) return;
		
		m_wToolBoxComponent = w.FindAnyWidget(m_sUiComponentName);
		if (!m_wToolBoxComponent) return;
		
		m_ToolBoxComponent = RT_WS_ToolboxComponent.Cast(m_wToolBoxComponent.FindHandler(RT_WS_ToolboxComponent));
		if (!m_ToolBoxComponent) return;
		
		if (m_AttributeManager.m_RT_WS_DelayedUnitsAttribute)
		{
			m_ToolBoxComponent.InitByAttribute(m_RTAttribute);
		}
		
		super.Init(w, attribute);
	}	
	
	protected void DelayedUnitsAttributeInit(RT_WS_WavesSpawner_Attribute_Units pAttribute)
	{
		m_AttributeManager.m_RT_WS_DelayedUnitsAttributeInit.Remove(DelayedUnitsAttributeInit);
		
		m_RTAttribute = pAttribute;
		
		m_ToolBoxComponent.InitByAttribute(m_RTAttribute);
	}
		
	RT_WS_ToolboxComponent GetToolboxComponent()
	{
		return m_ToolBoxComponent;
	}		
	

	SCR_BaseEditorAttributeFloatStringValues GetButtonBoxData()
	{
		return m_ButtonBoxData;
	}
	
	protected void UpdateButtonBorder(int selectedIndex)
	{
		array<SCR_ButtonBaseComponent> elements = new array<SCR_ButtonBaseComponent>;
		
		int count = m_ToolBoxComponent.GetItems(elements);
		
		for(int i = 0; i < count; i++)
        {
			//~ Show Border
			if (i == selectedIndex)
				elements[i].ShowBorder(true);
			//~ Hide border
			else
				elements[i].ShowBorder(false);
        }
	}	
	
	override void HandlerDeattached(Widget w)
	{
		if (m_AttributeManager)
			m_AttributeManager.m_RT_WS_DelayedUnitsAttributeInit.Remove(DelayedUnitsAttributeInit);
		super.HandlerDeattached(w);
	}
	
	override void SetVariableToDefaultValue(SCR_BaseEditorAttributeVar var)	{}
	override void SetFromVar(SCR_BaseEditorAttributeVar var) {}
	override void ToggleButtonSelected(bool selected, int index, bool animated = true) {}

}