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
	
	static SCR_PlayerData GetCurrentPlayerData()
	{
		SCR_PlayerData playerData = GetGame().GetDataCollector().GetPlayerData(SCR_PlayerController.GetLocalPlayerId(), true);
		if (!playerData)
		{
			return null;
		}	
		
		return playerData;
	}
	
	static void SaveCurrentUserVisual(VisualIdentity pSavedVisual)
	{
		SCR_PlayerData playerData = SCR_PlayerData.GetCurrentPlayerData();
		if (!playerData) 
		{
			Print("[ChangeYourFace] Error: save visual failed", LogLevel.ERROR);
			return;
		}
		
		playerData.SaveVisual(pSavedVisual);	
		
		if (!System.IsConsoleApp())
		{
		#ifdef PLATFORM_WINDOWS
			RT_CF_IdentityJsonStruct json = new RT_CF_IdentityJsonStruct();
			
			json.head = pSavedVisual.GetHead();
			json.body = pSavedVisual.GetBody();
		
			json.Pack();
		
			json.SaveToFile(RT_CF_Utils.SAVE_FILE);
		#endif
		}
	}
	
	static VisualIdentity GetCurrentUserVisual(IEntity defaultEntityIfEmpty)
	{
		VisualIdentity v;
		
		SCR_PlayerData playerData = SCR_PlayerData.GetCurrentPlayerData();
		
		if (playerData)
		{
			v = playerData.GetSavedVisual();
		}
		
		if (!v)
		{
			RT_CF_IdentityJsonStruct savedStruct = GetSavedUserVisualData();
			v = RT_CF_Utils.GetVisualIdentity(defaultEntityIfEmpty);
			
			if (v && savedStruct.head && savedStruct.body)
			{
				ResourceName head = savedStruct.head;				
				v = VisualIdentity.Cast(v.Clone());
				v.SetHead(head);
				v.SetBody(savedStruct.body);
			}
		}
		
		return v;
	}
	
	static RT_CF_IdentityJsonStruct GetSavedUserVisualData()
	{
		RT_CF_IdentityJsonStruct identityData = new RT_CF_IdentityJsonStruct();
		
		//return identityData;

		SCR_PlayerData playerData = SCR_PlayerData.GetCurrentPlayerData();
		if (!playerData || !identityData)
		{
			Print("[ChangeYourFace] Error: get visual failed", LogLevel.ERROR);
			return identityData;
		}
		
		if (!System.IsConsoleApp())
		{
		#ifdef PLATFORM_WINDOWS			
			identityData.LoadFromFile(RT_CF_Utils.SAVE_FILE);
		#endif	
		}
			
		return identityData;
		
		
	}	
}