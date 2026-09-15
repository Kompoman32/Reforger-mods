//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class RT_WS_AddUnitsToCustomSpawnerContextAction : SCR_SelectedEntitiesContextAction
{	
	override bool CanBeShown(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
	{
		RT_WavesSpawnerEntity entity = RT_WavesSpawnerEntity.Cast(selectedEntity.GetOwner());
		
		if (!entity) return false;

		return true;
	}
	
	override bool CanBePerformed(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
	{		
		return true;
	}
	
	override void Perform(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition)
	{		
		RT_WavesSpawnerEntity entity = RT_WavesSpawnerEntity.Cast(selectedEntity.GetOwner());
		
		if (!entity) return;
		
		entity.AddNearbyUnits();
	}
};

[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class RT_WS_TeleportCameraContextAction : SCR_GeneralContextAction
{
	[Attribute("1", desc: "Determines if this action will teleport camera to Assembly point")]
	bool m_bIsMoveToPoint;
	
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		if (!hoveredEntity) return false;
		
		RT_WavesSpawnerEntity spawner = RT_WavesSpawnerEntity.Cast(hoveredEntity.GetOwner());
		RT_WavesSpawner_MovePointEntity movePoint = RT_WavesSpawner_MovePointEntity.Cast(hoveredEntity.GetOwner());
		
		Managed teleportTo;
		
		if (m_bIsMoveToPoint) {
			if (spawner)
				teleportTo = spawner.m_MovePoint;
		} else {
			if (movePoint) 
				teleportTo = movePoint.m_Spawner;
		}
		
		if (!teleportTo) return false;
		
		return true;
	}
	
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition,int flags, int param = -1)
	{
		if (!hoveredEntity) return;
		
		RT_WavesSpawnerEntity spawner = RT_WavesSpawnerEntity.Cast(hoveredEntity.GetOwner());
		RT_WavesSpawner_MovePointEntity movePoint = RT_WavesSpawner_MovePointEntity.Cast(hoveredEntity.GetOwner());
		
		IEntity teleportTo;
		
		if (m_bIsMoveToPoint) {
			if (spawner)
				teleportTo = spawner.m_MovePoint;
		} else {
			if (movePoint) 
				teleportTo = movePoint.m_Spawner;
		}
		
		if (!teleportTo) return;
		
		
		SCR_CameraEditorComponent cameraManager = SCR_CameraEditorComponent.Cast(SCR_CameraEditorComponent.GetInstance(SCR_CameraEditorComponent));
		if (!cameraManager) return;
		
		SCR_ManualCamera camera = cameraManager.GetCamera();
		if (!camera) return;
		
		SCR_TeleportToCursorManualCameraComponent teleportComponent = SCR_TeleportToCursorManualCameraComponent.Cast(camera.FindCameraComponent(SCR_TeleportToCursorManualCameraComponent));
		if (!teleportComponent) return;
		
		teleportComponent.TeleportCamera(teleportTo.GetOrigin(), true, false);
	}
};