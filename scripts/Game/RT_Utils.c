class RT_CF_Utils
{	
	static void SetIdentity(IEntity pUserEntity, VisualIdentity pVisual)
	{		
		if (!pVisual)
		{
			pVisual = SCR_PlayerData.GetCurrentUserVisual();
			
			if (!pVisual)
			{
				pVisual = RT_CF_Utils.GetVisualIdentity(pUserEntity);	
				SCR_PlayerData.SaveCurrentUserVisual(pVisual);
			}			
		}
		
		if (!pUserEntity || !pVisual) return;
		
		SCR_CharacterIdentityComponent iden = SCR_CharacterIdentityComponent.Cast(pUserEntity.FindComponent(SCR_CharacterIdentityComponent));
		if (!iden) return;	

		iden.GetIdentity().SetVisualIdentity(pVisual);
		iden.CommitChanges();	
	}
	
	static void SetIdentity(IEntity pUserEntity, ResourceName pHead, ResourceName pBody)
	{		
		if (!pUserEntity || !(pHead && pBody)) return;
		
		SCR_CharacterIdentityComponent iden = SCR_CharacterIdentityComponent.Cast(pUserEntity.FindComponent(SCR_CharacterIdentityComponent));
		if (!iden) return;	

		iden.GetIdentity().GetVisualIdentity().SetHead(pHead);
		iden.GetIdentity().GetVisualIdentity().SetBody(pBody);
		iden.CommitChanges();	
	}
	
	static VisualIdentity GetVisualIdentity(IEntity pUserEntity)
	{
		if (!pUserEntity) return null;
		
		SCR_CharacterIdentityComponent iden = SCR_CharacterIdentityComponent.Cast(pUserEntity.FindComponent(SCR_CharacterIdentityComponent));
		if (!iden) return null;

		return iden.GetIdentity().GetVisualIdentity();
	}
	
	static IEntity GetEntityByRplComponentId(RplId id)
	{
		RplComponent rpl = RplComponent.Cast(Replication.FindItem(id));
		
		if (!rpl) return null;
		
		return rpl.GetEntity();
	}
}