modded enum ChimeraMenuPreset : ScriptMenuPresetEnum
{
	FaceEditorBrowserDialog
}

modded class SCR_DeployMenuMain : SCR_DeployMenuBase {
	protected override void OnPlayerLoadoutResponse(SCR_PlayerLoadoutComponent component, int loadoutIndex, bool response)
	{
		super.OnPlayerLoadoutResponse(component, loadoutIndex, response);
		
		LoadSavedFace();
	}
	
	protected void LoadSavedFace()
	{
		SCR_LoadoutRequestUIComponent loadoutRequestUIHandler = m_LoadoutRequestUIHandler;
		
		if (!loadoutRequestUIHandler) return;
		
		IEntity previewEntity = loadoutRequestUIHandler.GetPreviewedEntity();
				
		if (!previewEntity) return;
		VisualIdentity currentFace = SCR_PlayerData.GetCurrentUserVisual(previewEntity);
		
		RT_CF_Utils.SavePlayerIdentity(previewEntity, currentFace);		
		loadoutRequestUIHandler.ResetLoadoutPreview();
	}
}