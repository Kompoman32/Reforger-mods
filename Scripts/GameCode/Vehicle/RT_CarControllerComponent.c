class RT_CarControllerComponentClass : SCR_CarControllerComponentClass
{
}

/*!
	Class responsible for game car.
	It connects all car components together and handles all comunication between them.
*/
class RT_CarControllerComponent : SCR_CarControllerComponent 
{
	//------------------------------------------------------------------------------------------------
	override void OnCompartmentEntered(IEntity vehicle, BaseCompartmentManagerComponent mgr, IEntity occupant, int managerId, int slotID)
	{
		super.OnCompartmentEntered(vehicle, mgr, occupant, managerId, slotID);
		
		if (mgr.FindCompartment(slotID, managerId) == GetPilotCompartmentSlot()) 
		{
			GetGame().GetInputManager().AddActionListener("CarTurnSignalLeft", EActionTrigger.UP, ActionTurnSignalsLeftToggle);
			GetGame().GetInputManager().AddActionListener("CarTurnSignalRight", EActionTrigger.UP, ActionTurnSignalsRightToggle);
			GetGame().GetInputManager().AddActionListener("CarHazardLights", EActionTrigger.UP, ActionHazardLightsToggle);
		}
	}

	//------------------------------------------------------------------------------------------------
	override void OnCompartmentLeft(IEntity vehicle, BaseCompartmentManagerComponent mgr, IEntity occupant, int managerId, int slotID)
	{
		super.OnCompartmentLeft(vehicle, mgr, occupant, managerId, slotID);

		if (mgr.FindCompartment(slotID, managerId) == GetPilotCompartmentSlot())
		{
			GetGame().GetInputManager().RemoveActionListener("CarTurnSignalLeft", EActionTrigger.UP, ActionTurnSignalsLeftToggle);
			GetGame().GetInputManager().RemoveActionListener("CarTurnSignalRight", EActionTrigger.UP, ActionTurnSignalsRightToggle);
			GetGame().GetInputManager().RemoveActionListener("CarHazardLights", EActionTrigger.UP, ActionHazardLightsToggle);
		}
	}
	
	void ActionTurnSignalsToggle(IEntity owner, bool left)
	{
		ActionsManagerComponent actionManager = ActionsManagerComponent.Cast(GetOwner().FindComponent(ActionsManagerComponent));
		
		array <BaseUserAction> userActions = {};
		
		actionManager.GetActionsList(userActions);
				
		foreach (BaseUserAction userAction : userActions)
		{
			if (left && userAction.IsInherited(SCR_LightsTurnLeftUserAction) || !left && userAction.IsInherited(SCR_LightsTurnRightUserAction))
			{
				ScriptedUserAction.Cast(userAction).PerformAction(owner, owner);
				break;
			}			
		}
	}
		
	void ActionTurnSignalsLeftToggle(float value, EActionTrigger trigger)
	{
		PilotCompartmentSlot pilotSlot = GetPilotCompartmentSlot();
		if (!pilotSlot || pilotSlot.GetOccupant() != SCR_PlayerController.GetLocalControlledEntity())
			return;
		
		ActionTurnSignalsToggle(pilotSlot.GetOccupant(), true);
	}
	
	void ActionTurnSignalsRightToggle(float value, EActionTrigger trigger)
	{
		PilotCompartmentSlot pilotSlot = GetPilotCompartmentSlot();
		if (!pilotSlot || pilotSlot.GetOccupant() != SCR_PlayerController.GetLocalControlledEntity())
			return;
		
		ActionTurnSignalsToggle(pilotSlot.GetOccupant(), false);
	}
	
	void ActionHazardLightsToggle(float value, EActionTrigger trigger)
	{
		PilotCompartmentSlot pilotSlot = GetPilotCompartmentSlot();
		if (!pilotSlot || pilotSlot.GetOccupant() != SCR_PlayerController.GetLocalControlledEntity())
			return;
		
		ActionsManagerComponent actionManager = ActionsManagerComponent.Cast(GetOwner().FindComponent(ActionsManagerComponent));
		
		array <BaseUserAction> userActions = {};
		
		actionManager.GetActionsList(userActions);
				
		foreach (BaseUserAction userAction : userActions)
		{
			if (userAction.IsInherited(SCR_LightsHazardUserAction))
			{
				ScriptedUserAction.Cast(userAction).PerformAction(pilotSlot.GetOccupant(), pilotSlot.GetOccupant());
				break;
			}			
		}
	}
}
