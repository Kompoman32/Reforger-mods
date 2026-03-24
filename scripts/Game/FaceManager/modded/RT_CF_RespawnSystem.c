modded class SCR_RespawnSystemComponent : RespawnSystemComponent
{
	override void OnPlayerSpawnFinalize_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnHandlerComponent handlerComponent, SCR_SpawnData data, IEntity entity)
	{
		super.OnPlayerSpawnFinalize_S(requestComponent, handlerComponent, data, entity);
		
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(entity);
		
		if (!character) return;
		
		character.RequestSaveVisualFromOwner_S();
	}
}