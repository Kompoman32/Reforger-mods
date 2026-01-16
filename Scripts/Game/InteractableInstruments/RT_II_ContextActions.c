[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
//------------------------------------------------------------------------------------------------
class RT_II_PlayInstrumentContextAction : SCR_SelectedEntitiesContextAction
{	
	[Attribute(desc: "Order of played action on instrument.")]
	int m_iPlayActionIndex;
	
	[Attribute("0", UIWidgets.ComboBox, "Instrument type", "", ParamEnumArray.FromEnum(SCR_EInstrumentType) )]
	SCR_EInstrumentType m_eInstrumentType;
	
	protected ref SCR_PlayInstrument m_SelectedPlayAction;
	
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		// No Multiple Entities
		if (!selectedEntities && selectedEntities.Count() > 1) return false;
		if (!hoveredEntity) return false;
		
		IEntity entity = hoveredEntity.GetOwner();
		
		if (!entity) return false;
		
		ActionsManagerComponent amc = ActionsManagerComponent.Cast(entity.FindComponent(ActionsManagerComponent));
		if (!amc) return false;
		
		array<BaseUserAction> actions = {};				
		amc.GetActionsList(actions);
		
		int playIndex = -1;
		
		foreach (BaseUserAction action: actions)
		{
			SCR_PlayInstrument playAction = SCR_PlayInstrument.Cast(action);
			
			if (!playAction) continue;
			if (playAction.GetInstrumentType() != m_eInstrumentType) continue;
			
			playIndex += 1;
			
			if (playIndex == m_iPlayActionIndex)
			{
				m_SelectedPlayAction = playAction;
				return true;
			}
		}
		
		return false;
	}
	
	override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{			
		return !!m_SelectedPlayAction;
	}
	
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags, int param = -1)
	{		
		if (!m_SelectedPlayAction) return;
		
		IEntity entity = hoveredEntity.GetOwner();
		
		if (!entity) return;
		
		ActionsManagerComponent amc = ActionsManagerComponent.Cast(entity.FindComponent(ActionsManagerComponent));
		if (!amc) return;
		
		array<BaseUserAction> actions = {};				
		amc.GetActionsList(actions);
		
		foreach (BaseUserAction action: actions)
		{
			SCR_PlayInstrument playAction = SCR_PlayInstrument.Cast(action);
			
			if (!playAction) continue;
			
			AudioSystem.TerminateSound(playAction.GetAudioHandle());
		}
				
		m_SelectedPlayAction.OnActionStart(null);
	}
};

[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
//------------------------------------------------------------------------------------------------
class RT_II_StopPlayInstrumentContextAction : SCR_SelectedEntitiesContextAction
{	
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		// No Multiple Entities
		if (!selectedEntities && selectedEntities.Count() > 1) return false;
		if (!hoveredEntity) return false;
		
		IEntity entity = hoveredEntity.GetOwner();

		if (!entity) return false;
		
		ActionsManagerComponent amc = ActionsManagerComponent.Cast(entity.FindComponent(ActionsManagerComponent));
		if (!amc) return false;
		
		array<BaseUserAction> actions = {};				
		amc.GetActionsList(actions);
		
		int playIndex = -1;
		
		foreach (BaseUserAction action: actions)
		{
			SCR_PlayInstrument playAction = SCR_PlayInstrument.Cast(action);
			
			if (!playAction) continue;
			
			return true;		
		}
		
		return false;
	}
	
	override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{			
		return true;
	}
	
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags, int param = -1)
	{		
		IEntity entity = hoveredEntity.GetOwner();
		
		if (!entity) return;
		
		ActionsManagerComponent amc = ActionsManagerComponent.Cast(entity.FindComponent(ActionsManagerComponent));
		if (!amc) return;
		
		array<BaseUserAction> actions = {};				
		amc.GetActionsList(actions);
		
		foreach (BaseUserAction action: actions)
		{
			SCR_PlayInstrument playAction = SCR_PlayInstrument.Cast(action);
			
			if (!playAction) continue;
			
			if (playAction.GetAudioHandle() == AudioHandle.Invalid) continue;
			
			playAction.OnActionCanceled(entity, null);
		}
	}
};

modded class SCR_PlayInstrument : ScriptedUserAction
{
	SCR_EInstrumentType GetInstrumentType()
	{
		return m_eInstrumentType;
	}
	
	AudioHandle GetAudioHandle()
	{
		return m_AudioHandle;
	}	
}