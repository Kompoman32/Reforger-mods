modded class SCR_PlayerController : PlayerController
{
	void SaveEntityVisualToServer_O(SCR_ChimeraCharacter entity, ResourceName head, ResourceName body)
	{		
		RplComponent rplComp = RplComponent.Cast(entity.FindComponent(RplComponent));
		if (!rplComp) return;
		
		Rpc(SaveEntityVisualToServer_S, rplComp.Id(), head, body);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void SaveEntityVisualToServer_S(RplId id, ResourceName head, ResourceName body)
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
}