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
	void SaveVisualToServer_S(RplId id, ResourceName head, ResourceName body)
	{
		IEntity entity = RT_CF_Utils.GetEntityByRplComponentId(id);
		
		if (!entity) return;
		
		SCR_CharacterIdentityComponent iden = SCR_CharacterIdentityComponent.Cast(entity.FindComponent(SCR_CharacterIdentityComponent));
		if (!iden) return;
		
		iden.GetIdentity().GetVisualIdentity().SetHead(head);
		iden.GetIdentity().GetVisualIdentity().SetBody(body);
		iden.SetIdentity(iden.GetIdentity());
		iden.CommitChanges();
	}
	
	void RequestSaveVisualFromOwner_S()
	{
		RplComponent rplComp = RplComponent.Cast(FindComponent(RplComponent));
		if (!rplComp) return;
		
		Rpc(RequestSaveVisualFromOwner_O, rplComp.Id());
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RequestSaveVisualFromOwner_O(RplId id)
	{
		IEntity entity = RT_CF_Utils.GetEntityByRplComponentId(id);
		
		if (!entity) return;
		
		VisualIdentity currentFace = SCR_PlayerData.GetCurrentUserVisual(entity);
		
		if (currentFace && currentFace.GetHead() && currentFace.GetBody())
		{
			GetGame().GetCallqueue().Remove(SaveVisualToServer_O);
			GetGame().GetCallqueue().CallLater(SaveVisualToServer_O, 100, false, currentFace.GetHead(), currentFace.GetBody());
		}
	}
}