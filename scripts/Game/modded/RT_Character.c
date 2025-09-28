modded class SCR_ChimeraCharacter : ChimeraCharacter
{
	// Only for owners
	void SaveVisualToServer_O(ResourceName head, ResourceName body)
	{
		RplComponent rplComp = RplComponent.Cast(FindComponent(RplComponent));
		if (!rplComp) return;
		
		if (!rplComp.IsOwner()) return;
		
		Rpc(SaveVisualToServer_S, rplComp.Id(), head, body);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void SaveVisualToServer_S(RplId id, ResourceName head, ResourceName body)
	{
		IEntity entity = RT_CF_Utils.GetEntityByRplComponentId(id);
		
		if (!entity) return;
		
		SCR_CharacterIdentityComponent iden = SCR_CharacterIdentityComponent.Cast(entity.FindComponent(SCR_CharacterIdentityComponent));
		if (!iden) return;
		
		iden.GetIdentity().GetVisualIdentity().SetHead(head);
		iden.GetIdentity().GetVisualIdentity().SetBody(body);
		iden.SetIdentity(iden.GetIdentity());
	}
}