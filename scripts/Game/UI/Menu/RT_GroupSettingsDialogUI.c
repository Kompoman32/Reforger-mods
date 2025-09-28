class RT_GroupSettingsDialogUI: DialogUI
{
	protected SCR_EditBoxComponent m_Description;
	protected SCR_ComboBoxComponent m_GroupStatus;
	protected SCR_SliderComponent m_GroupMaxSize;
	protected SCR_EditBoxComponent m_GroupName;
	protected SCR_EditBoxComponent m_GroupDescription;
	
	protected static ref SCR_ScriptPlatformRequestCallback m_CallbackGetPrivilege;
		
	protected SCR_PlayerControllerGroupComponent m_PlayerComponent;
	protected SCR_GroupsManagerComponent m_GroupsManager;
	protected SCR_AIGroup m_PlayerGroup;
	
	protected bool m_bHasPrivilege;

	//------------------------------------------------------------------------------------------------
	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		
		m_PlayerComponent = SCR_PlayerControllerGroupComponent.GetLocalPlayerControllerGroupComponent();
		if (!m_PlayerComponent)
			return;
		
		if (m_PlayerComponent.GetSelectedGroupID() == -1)
		{
			Close();
			return;
		}
		if (!m_CallbackGetPrivilege)
			m_CallbackGetPrivilege = new SCR_ScriptPlatformRequestCallback();
		
		m_CallbackGetPrivilege.m_OnResult.Insert(OnPrivilegeCallback);
		
		GetGame().GetPlatformService().GetPrivilegeAsync(UserPrivilege.USER_GEN_CONTENT, m_CallbackGetPrivilege);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnMenuClose()
	{
		super.OnMenuClose();
		
		if (m_CallbackGetPrivilege)
			m_CallbackGetPrivilege.m_OnResult.Remove(OnPrivilegeCallback);
	}
	
	void OnPrivilegeCallback(UserPrivilege privilege, UserPrivilegeResult result)
	{
		
		m_GroupsManager = SCR_GroupsManagerComponent.GetInstance();
		if (!m_GroupsManager)
			return;
		
		m_PlayerGroup = m_GroupsManager.FindGroup(m_PlayerComponent.GetGroupID());
		if (!m_PlayerGroup)
			return;
		
		Widget w = GetRootWidget();
		if (!w)
			return;
		
		m_GroupStatus = SCR_ComboBoxComponent.GetComboBoxComponent("Type", w);
		if (!m_GroupStatus)
			return;
		
		m_GroupMaxSize = SCR_SliderComponent.GetSliderComponent("MaxGroupSize", w);
		if (!m_GroupMaxSize)
			return;
		
		m_GroupName = SCR_EditBoxComponent.GetEditBoxComponent("Name", w);
		if (!m_GroupName)
			return;
		
		m_GroupDescription = SCR_EditBoxComponent.GetEditBoxComponent("Description", w);
		if (!m_GroupDescription)
			return;
		
		if (result == UserPrivilegeResult.ALLOWED)
		{
			m_GroupName.SetEnabled(true);
			m_GroupName.SetValue(m_PlayerGroup.GetCustomName());
			
			m_GroupMaxSize.SetEnabled(true);
			m_GroupMaxSize.SetValue(m_PlayerGroup.GetMaxMembers());
			
			m_GroupDescription.SetEnabled(true);
			m_GroupDescription.SetValue(m_PlayerGroup.GetCustomDescription());
			m_bHasPrivilege = true;
		}
		else
		{
			m_GroupName.SetEnabled(false);
			m_GroupName.SetValue(WidgetManager.Translate("#AR-UserActionUnavailable"));
			m_GroupMaxSize.SetEnabled(false);
			m_GroupMaxSize.SetValue(m_PlayerGroup.GetMaxMembers());
			m_GroupDescription.SetEnabled(false);
			m_GroupDescription.SetValue(WidgetManager.Translate("#AR-UserActionUnavailable"));
			m_bHasPrivilege = false;
		}
		
		SetupGroupStatusCombo();
	}

	//------------------------------------------------------------------------------------------------
	override void OnMenuUpdate(float tDelta)
	{
		super.OnMenuUpdate(tDelta);
		
		GetGame().GetInputManager().ActivateContext("InteractableDialogContext");
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnConfirm()
	{
		SCR_PlayerControllerGroupComponent groupController = SCR_PlayerControllerGroupComponent.GetLocalPlayerControllerGroupComponent();
		if (!groupController)
			return;
		
		if (m_bHasPrivilege)
		{
			int groupID = m_PlayerGroup.GetGroupID();
			groupController.RequestSetCustomGroupDescription(groupID, m_GroupDescription.GetValue());
			groupController.RequestSetCustomGroupName(groupID, m_GroupName.GetValue());
			groupController.RequestSetGroupMaxMembers(groupID, m_GroupMaxSize.GetValue());
		}
		
		Close();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void SetupGroupStatusCombo()
	{
		m_GroupStatus.AddItem("#AR-Player_Groups_Public");
		m_GroupStatus.AddItem("#AR-Player_Groups_Private");
		
		m_GroupStatus.SetCurrentItem(m_PlayerGroup.IsPrivate());
		m_GroupStatus.m_OnChanged.Insert(OnStatusChanged);
		
	}
	
	//------------------------------------------------------------------------------------------------
	void OnStatusChanged(SCR_ComboBoxComponent combo, int index)
	{
		if (!m_PlayerComponent)
			return;
		
		m_PlayerComponent.RequestPrivateGroupChange(m_PlayerComponent.GetPlayerID() , index);
	}
}