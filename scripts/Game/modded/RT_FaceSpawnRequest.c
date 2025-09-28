modded class SCR_SpawnRequestComponent : ScriptComponent
{	
	protected override void FinalizeRequest_S(SCR_SpawnHandlerComponent handler, SCR_SpawnData data, IEntity spawnedEntity)
	{
		super.FinalizeRequest_S(handler, data, spawnedEntity);		
		
		RplComponent rpl = RplComponent.Cast(spawnedEntity.FindComponent(RplComponent));
		
		Rpc(Rpc_DoSendFace, rpl.Id());
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void Rpc_DoSendFace(RplId id)
	{		
		IEntity entity = RT_CF_Utils.GetEntityByRplComponentId(id);
		
		if (!entity) return;
		
		VisualIdentity v = SCR_PlayerData.GetCurrentUserVisual();
		
		if (!v)
		{
			v = RT_CF_Utils.GetVisualIdentity(entity);
		}	
			
		if (v)
		{
			Rpc(Rpc_AskSendFace, id, v.GetHead(), v.GetBody());
		}		
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void Rpc_AskSendFace(RplId id, ResourceName head, ResourceName body)
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