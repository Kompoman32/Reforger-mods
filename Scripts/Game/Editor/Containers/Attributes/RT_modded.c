modded class SCR_AIGroup : ChimeraAIGroup
{
	string RT_GetCustomName()
	{
		return m_sCustomName;
	}
	
	string RT_GetCustomDescription()
	{
		return m_sCustomDescription;
	}
}


modded class SCR_MapJournalUI : SCR_MapUIBaseComponent
{
	protected override void ShowEntry(SCR_MapJournalUIButton journalBtn)
	{
		if (!m_wEntryLayout) return;
		
		super.ShowEntry(journalBtn);
	}
	
	protected override void OnPlayerFactionResponse(SCR_PlayerFactionAffiliationComponent component, int factionIndex, bool response)
	{
		if (!m_wEntryLayout) return;
		
		super.OnPlayerFactionResponse(component, factionIndex, response);
	}
}
