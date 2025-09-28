//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_PlayerFactionEditorAttribute : SCR_EntityFactionEditorAttribute
{
	protected SCR_PlayerFactionAffiliationComponent GetFactionAffiliation(GenericEntity entity)
	{
		PlayerManager pm = GetGame().GetPlayerManager();		
		PlayerController pc = pm.GetPlayerController(pm.GetPlayerIdFromControlledEntity(entity));
		
		if (!pc)
			return null;
		
		return SCR_PlayerFactionAffiliationComponent.Cast(pc.FindComponent(SCR_PlayerFactionAffiliationComponent));
	}
	
	protected SCR_PlayerControllerGroupComponent GetPlayerControllerGroupComponent(GenericEntity entity)
	{
		PlayerManager pm = GetGame().GetPlayerManager();
		
		if (!pm)
			return null;
		
		PlayerController pc = pm.GetPlayerController(pm.GetPlayerIdFromControlledEntity(entity));
		
		if (!pc)
			return null;
		
		return SCR_PlayerControllerGroupComponent.GetPlayerControllerComponent(pc.GetPlayerId());
	}
	
	protected override bool ValidEntity(GenericEntity entity)
	{		
		SCR_PlayerFactionAffiliationComponent playerFactionAff = GetFactionAffiliation(entity);
		
		if (!playerFactionAff)
			return false;
		
		return true;
	}
	
	protected override Faction GetFaction(GenericEntity entity)
	{
		SCR_PlayerFactionAffiliationComponent playerFactionAff = GetFactionAffiliation(entity);
		
		if (!playerFactionAff)
			return null;
		
		return playerFactionAff.GetAffiliatedFaction();
	}
	
	protected override void SetFaction(GenericEntity entity, Faction faction)
	{
		SCR_PlayerFactionAffiliationComponent playerFactionAff = GetFactionAffiliation(entity);
		SCR_GroupsManagerComponent groupsManager = SCR_GroupsManagerComponent.GetInstance();
		
		if (!playerFactionAff || !groupsManager)
			return;

		SCR_AIGroup previousGroup = groupsManager.GetPlayerGroup(playerFactionAff.GetPlayerId());		
		
		SCR_PlayerControllerGroupComponent pcg = GetPlayerControllerGroupComponent(entity);
		
		playerFactionAff.RequestFaction(faction);
		
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(entity);		
		char.m_pFactionComponent.SetAffiliatedFaction(faction);
		
		
		if (previousGroup) {
			previousGroup.RemovePlayer(playerFactionAff.GetPlayerId());
		}
				
		SCR_AIGroup newGroup = groupsManager.CreateNewPlayableGroup(faction);	
		if (newGroup)
		{
			pcg.RequestJoinGroup(newGroup.GetGroupID());
		}
	}
}
