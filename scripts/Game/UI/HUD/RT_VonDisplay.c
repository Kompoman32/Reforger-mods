
//------------------------------------------------------------------------------------------------
//! VON display of active outgoing and incoming transmissions
class RT_VonDisplay : SCR_VonDisplay
{

	//------------------------------------------------------------------------------------------------
	//! Update transmission data
	//! \param TransmissionData is the subject
	//! \param radioTransceiver is the used transceiver for the transmission
	//! \param IsReceiving is true when receiving transmission, false when transmitting
	//! \param isAdditionalSpeaker is true when all incomming transmission widgets are full
	//! \return false if the transimission is filtered out to not be visible
	protected override bool UpdateTransmission(TransmissionData data, BaseTransceiver radioTransceiver, int frequency, bool IsReceiving)
	{	
		data.m_RadioTransceiver = radioTransceiver;
		data.m_bForceUpdate = false;

		if (!radioTransceiver && m_bIsVONDirectDisabled && IsReceiving)	// can happen when existing RADIO transmission switches to direct
			return false;

		// faction
		Faction playerFaction;
		int controlledID = GetGame().GetPlayerController().GetPlayerId();

		SCR_FactionManager factionMgr = SCR_FactionManager.Cast(GetGame().GetFactionManager());
		if (controlledID && factionMgr)
		{
			playerFaction = factionMgr.SGetPlayerFaction(controlledID);
			data.m_Faction = factionMgr.SGetPlayerFaction(data.m_iPlayerID);
		}
		
		if (IsReceiving && playerFaction && playerFaction.IsFactionEnemy(data.m_Faction) && !radioTransceiver) // enemy direct transmission case
		{
			return false; // ignore all direct enemy speech
 	  	}
		
		return super.UpdateTransmission(data, radioTransceiver, frequency, IsReceiving);
	}

}
