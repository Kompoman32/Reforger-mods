[BaseContainerProps(), BaseContainerCustomTitleField("m_sDisplayName")]
class RT_WaveSpawnerUseVehiclesTooltipDetail : SCR_EntityTooltipDetail
{
	protected TextWidget m_Text;
	
	//------------------------------------------------------------------------------------------------
	override bool NeedUpdate()
	{
		return m_Text;
	}

	//------------------------------------------------------------------------------------------------
	override void UpdateDetail(SCR_EditableEntityComponent entity)
	{
		RT_WavesSpawnerEntity spawner = RT_WavesSpawnerEntity.Cast(entity.GetOwner());
		if (!spawner)
			return;
		
		if (m_Text) {
			if (spawner.GetUseVehicles())
				m_Text.SetText("#AR-UI_Yes");
			else
				m_Text.SetText("#AR-UI_No");
		}
	}

	//------------------------------------------------------------------------------------------------
	override bool InitDetail(SCR_EditableEntityComponent entity, Widget widget)
	{	
		RT_WavesSpawnerEntity spawner = RT_WavesSpawnerEntity.Cast(entity.GetOwner());
		if (!spawner) return false;
			
		m_Text = TextWidget.Cast(widget);
		
		return m_Text;
	}
}

