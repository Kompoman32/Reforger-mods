[BaseContainerProps(), BaseContainerCustomTitleField("m_sDisplayName")]
class RT_WaveSpawnerNextSpawnTooltipDetail : SCR_EntityTooltipDetail
{
	protected TextWidget m_Value;
	protected SCR_WLibProgressBarComponent m_Bar;
	
	//------------------------------------------------------------------------------------------------
	override bool NeedUpdate()
	{
		return m_Value || m_Bar;
	}

	//------------------------------------------------------------------------------------------------
	override void UpdateDetail(SCR_EditableEntityComponent entity)
	{
		RT_WavesSpawnerEntity spawner = RT_WavesSpawnerEntity.Cast(entity.GetOwner());
		if (!spawner)
			return;
		
		int spawnTimer = spawner.GetSpawnTimer();
		
		bool isSpawnEnabled = this.isSpawnEnabled(spawner);
		
		if (!isSpawnEnabled) {
			spawnTimer = spawner.GetSpawnDelay();
		}
		
		if (m_Bar)
		{
			m_Bar.SetMax(spawner.GetSpawnDelay());
			m_Bar.SetValue(spawnTimer, true);			
		}
		
		if (m_Value) {
			if (isSpawnEnabled) 
				m_Value.SetText(string.Format("%1s", spawnTimer.ToString(-1, 0)));
			else 
				m_Value.SetText("");
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool isSpawnEnabled(RT_WavesSpawnerEntity spawner)
	{
		return spawner.GetSpawnEnabledBySettings() && spawner.GetGroupsCount() < spawner.GetMaxUnits();
	}

	//------------------------------------------------------------------------------------------------
	override bool InitDetail(SCR_EditableEntityComponent entity, Widget widget)
	{	
		RT_WavesSpawnerEntity spawner = RT_WavesSpawnerEntity.Cast(entity.GetOwner());
		if (!spawner) return false;
		
			
		m_Value = TextWidget.Cast(m_Widget.FindAnyWidget("Value"));
		
		SizeLayoutWidget barWidget = SizeLayoutWidget.Cast(m_Widget.FindAnyWidget("ProgressBar"));
		
		if (barWidget)
		{
			barWidget.SetMinDesiredWidth(60);
			m_Bar = SCR_WLibProgressBarComponent.Cast(barWidget.FindHandler(SCR_WLibProgressBarComponent));
		}
		
		if (m_Bar)
			m_Bar.SetMin(0);
		
		return m_Value || m_Bar;
	}
}

