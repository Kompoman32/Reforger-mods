//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_PlayerGroupFactionEditorAttribute : RT_PlayerFactionEditorAttribute
{
	protected override bool ValidEntity(GenericEntity entity)
	{
		SCR_PlayerFactionAffiliationComponent playerFactionAff = GetFactionAffiliation(entity);
		SCR_GroupsManagerComponent groupsManager = SCR_GroupsManagerComponent.GetInstance();
		return groupsManager && playerFactionAff && super.ValidEntity(entity) && groupsManager.GetPlayerGroup(playerFactionAff.GetPlayerId());
	}
	
	protected override void SetFaction(GenericEntity entity, Faction faction)
	{
		SCR_GroupsManagerComponent groupsManager = SCR_GroupsManagerComponent.GetInstance();
		
		if (!groupsManager) {
			super.SetFaction(entity, faction);
			return;
		}
		
		SCR_AIGroup previousGroup = groupsManager.GetPlayerGroup(GetFactionAffiliation(entity).GetPlayerId());
		
		if (!previousGroup) return;	
		
		previousGroup.SetDeleteWhenEmpty(true);
		
		array<int> playerIds = previousGroup.GetPlayerIDs();
		
		int leaderId;
		
		foreach (int playerId: playerIds)
		{
			if (previousGroup.IsPlayerLeader(playerId)) {
				leaderId = playerId;	
				break;
			}
		}
		
		playerIds.RemoveItem(leaderId);
		
		IEntity leaderEnity = GetGame().GetPlayerManager().GetPlayerControlledEntity(leaderId);
		
		SCR_AIGroup newGroup = createNewGroup(faction, previousGroup);	
		
		if (!newGroup) return;
	
		SetFactionToPlayer(leaderEnity, faction, previousGroup, newGroup);
					
		foreach (int playerId: playerIds)
		{
			IEntity playerEntity = GetGame().GetPlayerManager().GetPlayerControlledEntity(leaderId);
			SetFactionToPlayer(playerEntity, faction, previousGroup, newGroup);
		}
		
		newGroup.SetCustomName(previousGroup.RT_GetCustomName(), previousGroup.GetNameAuthorID());
		newGroup.SetCustomDescription(previousGroup.RT_GetCustomDescription(), previousGroup.GetDescriptionAuthorID());
	}
	
	protected SCR_AIGroup createNewGroup(Faction faction, SCR_AIGroup oldGroup)
	{
		if (!oldGroup) return null;
		
		SCR_GroupsManagerComponent groupsManager = SCR_GroupsManagerComponent.GetInstance();
		
		if (!groupsManager) {
			return null;
		}
		
		SCR_AIGroup newGroup = groupsManager.CreateNewPlayableGroup(faction);
		if (!newGroup) {
			return null;
		}
		
		return newGroup;
	}
	
	
	protected void SetFactionToPlayer(IEntity entity, Faction faction, SCR_AIGroup previousGroup, SCR_AIGroup newGroup)
	{
		if (!newGroup) return;
		
		GenericEntity genericEntity = GenericEntity.Cast(entity);
		
		SCR_PlayerFactionAffiliationComponent playerFactionAff = GetFactionAffiliation(genericEntity);
		
		int playerId = playerFactionAff && playerFactionAff.GetPlayerId();
		if (playerId) {
			previousGroup.RemovePlayer(playerId);
			
			playerFactionAff.RequestFaction(faction);
	
			SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(entity);
			
			if (!char) return;
			
			char.m_pFactionComponent.SetAffiliatedFaction(faction);
			
			SCR_PlayerControllerGroupComponent pcg = GetPlayerControllerGroupComponent(genericEntity);
			pcg.RequestJoinGroup(newGroup.GetGroupID());	
		} else {
//			previousGroup.RemoveAgent(agent);
//
//			FactionAffiliationComponent factionAffiliation = FactionAffiliationComponent.Cast(entity.FindComponent(FactionAffiliationComponent));
//			if (factionAffiliation)
//				factionAffiliation.SetAffiliatedFaction(faction);
//			
//			newGroup.AddAgent(agent);
		}
	}
}
