class RT_LightsHazardUserAction : SCR_LightsHazardUserAction
{
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		if (!pOwnerEntity || !pUserEntity)
			return;

		BaseLightManagerComponent lightManager = GetLightManager();
		if (!lightManager)
			return;
		
		bool lightsState;	
		
		lightsState = lightManager.GetLightsState(ELightType.Hazard);

		if (RplSession.Mode() != RplMode.Client)
			lightsState = !lightsState;

		lightManager.SetLightsState(ELightType.Hazard, false);
		lightManager.SetLightsState(ELightType.Hazard, lightsState);
				
		// Sound		
		PlaySound(pOwnerEntity, lightsState);
	}
};
