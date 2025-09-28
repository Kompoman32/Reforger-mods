modded class SCR_PlayerData : JsonApiStruct
{
	protected VisualIdentity m_SavedVisual;
	
	void SaveVisual(VisualIdentity pSavedVisual)
	{
		m_SavedVisual = pSavedVisual;
	}
	
	VisualIdentity GetSavedVisual()
	{
		return m_SavedVisual;
	}
	
	static void SaveCurrentUserVisual(VisualIdentity pSavedVisual)
	{
		SCR_PlayerData playerData = GetGame().GetDataCollector().GetPlayerData(SCR_PlayerController.GetLocalPlayerId(), true);
		if (!playerData)
		{
			Print("ERROR: save visual failed");
			return;
		}
		
		playerData.SaveVisual(pSavedVisual);		
	}
	
	static VisualIdentity GetCurrentUserVisual(IEntity entity)
	{
		VisualIdentity v = SCR_PlayerData.GetCurrentUserVisual();
		
		if (!v)
		{
			v = RT_CF_Utils.GetVisualIdentity(entity);
		}
		
		return v;	
	}
	
	static VisualIdentity GetCurrentUserVisual()
	{
		SCR_PlayerData playerData = GetGame().GetDataCollector().GetPlayerData(SCR_PlayerController.GetLocalPlayerId(), true);
		if (!playerData)
		{
			Print("ERROR: get visual failed");
			return null;
		}
		
		return playerData.GetSavedVisual();		
	}	
}