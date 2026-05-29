modded class SCR_PlayerController : PlayerController
{
	void RT_EEI_PasteLoadoutToEntities(array<RplId> selectedEntityIds, string pasteContextString)
	{
		Rpc(RT_EEI_PasteLoadoutToEntities_S, selectedEntityIds, pasteContextString);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RT_EEI_PasteLoadoutToEntities_S(array<RplId> selectedEntityIds, string pasteContextString)
	{
		set<IEntity> selectedEntities = new set<IEntity>();
		
		foreach (RplId selectedEntityRplId : selectedEntityIds)
		{
			SCR_EditableEntityComponent selectedEntityComponent;
			
			Managed entityComponent = Replication.FindItem(selectedEntityRplId);
			if (!entityComponent) continue;
			
			SCR_EditableEntityComponent editableEntityComponent = SCR_EditableEntityComponent.Cast(entityComponent);
			if (!editableEntityComponent) continue;
			
			IEntity entity = editableEntityComponent.GetOwner();
			if (!entity) continue;
			
			selectedEntities.Insert(entity);
		}
		
		if (selectedEntities.Count() == 0) return;
		
		JsonLoadContext ctx = new JsonLoadContext();		
		if (!ctx.ImportFromString(pasteContextString)) return;
		
		foreach (IEntity entity : selectedEntities)
		{
			SCR_PlayerArsenalLoadout.ApplyLoadoutString(entity, ctx);			
		}
		
		GetGame().GetCallqueue().CallLater(RT_EEI_PasteLoadoutToEntities_FixAnimation_S, 300, false, selectedEntities);
	}
	
	protected void RT_EEI_PasteLoadoutToEntities_FixAnimation_S(set<IEntity> selectedEntities)
	{
		foreach (IEntity entity : selectedEntities)
		{
			SCR_CharacterInventoryStorageComponent sm = SCR_CharacterInventoryStorageComponent.Cast(entity.FindComponent(SCR_CharacterInventoryStorageComponent));
			
			if (!sm) continue;

			IEntity currentItem = sm.GetCurrentItem();
			if (!currentItem) continue;
			
			WeaponComponent weapon = WeaponComponent.Cast(currentItem.FindComponent(WeaponComponent));
			if (!weapon) continue;
			
			sm.UnequipCurrentItem();
			
			ChimeraCharacter character = ChimeraCharacter.Cast(entity);
			if (!character) continue;
			
			CharacterControllerComponent controller = character.GetCharacterController();
			if (!controller) continue;
			
			GetGame().GetCallqueue().CallLater(RT_EEI_PasteLoadoutToEntities_FixAnimation_2_S, 2000, false, controller, weapon);
		}
		
	}
	
	protected void RT_EEI_PasteLoadoutToEntities_FixAnimation_2_S(CharacterControllerComponent controller, WeaponComponent weapon)
	{
		controller.SelectWeapon(weapon)
	}
}