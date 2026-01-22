[EntityEditorProps(category: "GameScripted/WavesSpawner", description: "Allows infinite spawning of units")]
class RT_WavesSpawnerEntityClass: GenericEntityClass {}
class RT_WavesSpawnerEntity: GenericEntity {
	[Attribute("", params: "et", category: "Spawner Settings")]
	protected ResourceName m_sUnitsGroupPrefab;
	
	[Attribute("", UIWidgets.EditBox, "Predetermined ResourceNames to add to select (AddNew) in Spawner", category: "Spawner Settings")]
	ref array<ResourceName> m_aAvailableResourceNames;
	
	[Attribute("", UIWidgets.EditBox, "Display names for every ResourceName, if not specified will get from ResourceName path (Empty string also means not specified)", category: "Spawner Settings")]
	ref array<string> m_aAvailableDisplayNames;

	protected SCR_CustomAreaMeshComponent m_AreaMesh;
	protected SCR_FactionAffiliationComponent m_FactionAffiliationComponent;
	protected SCR_EditableSystemComponent m_EditableSystemComponent;
	
	protected ResourceName m_sMovePointTemplate = "{70C1179FB1CA6C05}Prefabs/Systems/WavesSpawner/RT_WS-WavesSpawner_MovePoint.et";
	protected RT_WavesSpawner_MovePointEntity m_MovePoint;
	[RplProp(onRplName: "CreateMovePoint_Proxy")]
	protected RplId m_MovePointRplId;
	protected vector m_vLastMovePointPosition[4];
	
	protected vector m_vLastSpawnerPosition[4];
	
	protected ref array<SCR_AIGroup> m_aGroups;
	protected ref array<AIWaypointCycle> m_aWaypoints;
	
	bool m_bFactionLess = false;
	protected bool m_bEnabled = false;
	
	protected int m_iSpawnDelay = 2;
	protected float m_fSpawnTimer;
	protected float m_fSpawnRadius = 10;
	
	protected float m_fWaypointUpdateTimer = 1;
	protected float m_iWaypointUpdateDelay = 1;
	
	protected bool m_bIsEnabledAttribute = true;
	protected int m_iMaxUnits = 10;
	
	protected int m_iMinInGroup = 1;
	protected int m_iMaxInGroup = 1;
	
	ref array<ref Tuple2<string, int>> m_aUnitsSettings;
	protected ref SCR_WeightedArray<ResourceName> m_WeightedUnits = new SCR_WeightedArray<ResourceName>();
	
	
	ref Shape m_ConnectLine;
	bool m_bShowConnectLine;
	ShapeFlags m_ConnectLineFlagsOnHover = ShapeFlags.NOZBUFFER | ShapeFlags.TRANSP;
	
	void RT_WavesSpawnerEntity(IEntitySource src, IEntity parent) {
		if (!SCR_Global.IsEditMode())
			SetEventMask(EntityEvent.FRAME | EntityEvent.INIT);		
	}
	
	void ~RT_WavesSpawnerEntity() {
		if (m_MovePoint) 
			SCR_EntityHelper.DeleteEntityAndChildren(m_MovePoint);
	}
	
	override void EOnInit(IEntity owner)
	{
		m_AreaMesh = SCR_CustomAreaMeshComponent.Cast(FindComponent(SCR_CustomAreaMeshComponent));		
		m_FactionAffiliationComponent = SCR_FactionAffiliationComponent.Cast(FindComponent(SCR_FactionAffiliationComponent));
		m_EditableSystemComponent = SCR_EditableSystemComponent.Cast(FindComponent(SCR_EditableSystemComponent));
		
		m_bFactionLess = !GetFaction();
		
		m_fSpawnTimer = m_iSpawnDelay;
		
		m_aGroups = {};
		m_aWaypoints = {};
		m_aUnitsSettings = {};		
		
		if (m_AreaMesh)
		{
			m_fSpawnRadius = m_AreaMesh.GetRadius();
		}
		
		foreach (int i, ResourceName resourceName: m_aAvailableResourceNames)
		{
			string name = string.Empty;
			
			if (i < m_aAvailableDisplayNames.Count()) {
				name = m_aAvailableDisplayNames[i];
			}
			
			if (!name)
			{
				name = SCR_StringHelper.FormatResourceNameToUserFriendly(resourceName);
			}
			
			if (!name)
			{
				name = string.Format("NO_USE");
			}
			
			m_aAvailableDisplayNames[i] = name;
		}
		
		if (m_bFactionLess && GetGame().GetFactionManager())
		{
			SetFaction(GetGame().GetFactionManager().GetFactionByKey("US"));
		}
		
//		TODO: some day...
//		InitDefaultUnitSettings();
		
		if (Replication.IsServer())
		{
			CreateMovePoint();
			GetGame().GetCallqueue().CallLater(DelayedInit, 200, false);
		}
		else
		{
			GetGame().GetCallqueue().CallLater(CreateMovePoint_Proxy, 200, false);
		}
	}
	
	override void EOnFrame(IEntity owner, float timeSlice)
	{		
		UpdateConnectedLine();

		if (vector.Distance(GetOrigin(), m_vLastSpawnerPosition[3]) > 1) 
		{
			m_vLastSpawnerPosition = GetOrigin();
		}
		
		if (!Replication.IsServer())
			return;
		
		m_fWaypointUpdateTimer -= timeSlice;	
		if (m_fWaypointUpdateTimer < 0) 
		{
			m_fWaypointUpdateTimer = m_iWaypointUpdateDelay;
		
			UpdateWaypoints();	
		}
		
		if (!m_bEnabled) {
			return;
		}
		
		m_fSpawnTimer -= timeSlice;

		if (m_fSpawnTimer < 0) 
		{
			m_fSpawnTimer = m_iSpawnDelay;
		
			Spawn();
		}	
	}
	
	protected void DelayedInit() {						
		RefreshSpawnParameters();
	}
	
//	protected void InitDefaultUnitSettings()
//	{
//		foreach(int i, ResourceName resourceName : m_aAvailableResourceNames)
//		{
//			m_aUnitsSettings.Insert(new Tuple2<string, int>(resourceName, 0));
//		};		
//	}
	
	protected void CreateMovePoint()
	{					
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;		
		params.Transform[3] = RandomPointInRadius(GetOrigin(), m_fSpawnRadius / 3, m_fSpawnRadius / 2);
			
		m_MovePoint = RT_WavesSpawner_MovePointEntity.Cast(GetGame().SpawnEntityPrefabEx(m_sMovePointTemplate, false, GetGame().GetWorld(), params));
		if (!m_MovePoint) return;
		
		m_MovePoint.m_Spawner = this;
		
		vector movePointTransform[4];
		m_MovePoint.GetWorldTransform(movePointTransform);
		
		m_vLastMovePointPosition = movePointTransform;
		
		if (!m_MovePoint || !m_AreaMesh) return;
		
		SCR_CustomAreaMeshComponent areaMesh = SCR_CustomAreaMeshComponent.Cast(m_MovePoint.FindComponent(SCR_CustomAreaMeshComponent));
		
		if (!areaMesh) return;
		
		areaMesh.SetMaterial(m_AreaMesh.GetMaterialPublic());
		areaMesh.GenerateAreaMesh();
		
		GetGame().GetCallqueue().Remove(CreateMovePoint_Delayed);
		GetGame().GetCallqueue().CallLater(CreateMovePoint_Delayed, 500, false);
	}
	
	protected void CreateMovePoint_Delayed()
	{	
		UpdateConnectedLine();
		
		m_MovePointRplId = Replication.FindItemId(m_MovePoint);
		
		Replication.BumpMe();
	}
	
	protected void CreateMovePoint_Proxy()
	{
		m_MovePoint = RT_WavesSpawner_MovePointEntity.Cast(Replication.FindItem(m_MovePointRplId));
		
		if (!m_MovePoint) return;
		
		m_MovePoint.m_Spawner = this;
		
		SCR_CustomAreaMeshComponent areaMesh = SCR_CustomAreaMeshComponent.Cast(m_MovePoint.FindComponent(SCR_CustomAreaMeshComponent));
		
		if (!areaMesh) return;
		
		areaMesh.SetMaterial(m_AreaMesh.GetMaterialPublic());
		areaMesh.GenerateAreaMesh();
	}
	
	protected void CreateConnectLine()
	{		
		if (!m_MovePoint) return;
		
		vector movePointTransform[4];
			
		m_MovePoint.GetWorldTransform(movePointTransform);
		
		vector lineVerts[2];
		lineVerts[0] = GetOrigin();
		lineVerts[1] = movePointTransform[3];
	
		Color color = Color.White;
		
		if (m_FactionAffiliationComponent)
		{
			Faction faction = m_FactionAffiliationComponent.GetAffiliatedFaction();
			
			if (faction)
			{
				color = faction.GetFactionColor();
			}
		}
		
		m_ConnectLine = Shape.CreateLines(color.PackToInt(), m_ConnectLineFlagsOnHover, lineVerts, 2);
	}
	
	protected void UpdateConnectedLine()
	{
		m_bShowConnectLine = false;
		
		if (!m_EditableSystemComponent || !m_MovePoint || !m_MovePoint.m_EditableEntityComponent) return;
		
		EEditableEntityState entityStateToCheck = EEditableEntityState.HOVER | EEditableEntityState.SELECTED;
		EEditableEntityState spawnerState = m_EditableSystemComponent.GetEntityStates();
		EEditableEntityState movePointState = m_MovePoint.m_EditableEntityComponent.GetEntityStates();
		
		if ((spawnerState & entityStateToCheck || movePointState & entityStateToCheck) 
			&& m_EditableSystemComponent.GetVisibleSelf()
			&& m_MovePoint.m_EditableEntityComponent.GetVisibleSelf())
		{
			m_bShowConnectLine = true;
		}	
				
		if (!m_bShowConnectLine)
		{
			m_ConnectLine = null;
			return;
		}
		
		vector movePointTransform[4];
		m_MovePoint.GetWorldTransform(movePointTransform);
		
		if (m_ConnectLine && vector.Distance(movePointTransform[3], m_vLastMovePointPosition[3]) < 1 && vector.Distance(GetOrigin(), m_vLastSpawnerPosition[3]) < 1)
			return;
		
		CreateConnectLine();
	}
	
	protected void Spawn() 
	{	
		CleanFirstNullUnit();
		
		if (m_aGroups.Count() >= m_iMaxUnits) return;
		
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		GetWorldTransform(params.Transform);
		
		vector offsetPos;
	
		bool positionFound = false;
		
		for (int i = 0; i < 5; i++ )
		{
			positionFound = SCR_WorldTools.FindEmptyTerrainPosition(offsetPos, RandomPointInRadius(GetOrigin(), 1, m_fSpawnRadius * 3/4), m_fSpawnRadius / 2);
			
			if (positionFound) break;
		}
		
		if (!positionFound) return;
		
		params.Transform[3] = offsetPos;
		
		Resource groupPrefab = Resource.Load(m_sUnitsGroupPrefab);
		
		SCR_AIGroup group = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(groupPrefab, null, params));
		
		if (!group) return;
		
		if (m_bFactionLess && m_FactionAffiliationComponent)
		{
			group.SetFaction(m_FactionAffiliationComponent.GetAffiliatedFaction())
		}
		
		int unitsInGroup = s_AIRandomGenerator.RandInt(m_iMinInGroup, m_iMaxInGroup + 1);
		while (unitsInGroup > m_iMaxInGroup)
		{
			unitsInGroup = s_AIRandomGenerator.RandInt(m_iMinInGroup, m_iMaxInGroup + 1);
		}
		
		group.SetMaxUnitsToSpawn(unitsInGroup);
		
		group.m_aUnitPrefabSlots.Clear();
		for (int i = 0; i < unitsInGroup; i++ )
		{
			ResourceName currentUnitResource;
			m_WeightedUnits.GetRandomValue(currentUnitResource, s_AIRandomGenerator);
			
			group.m_aUnitPrefabSlots.Insert(currentUnitResource);
		}
		
		group.m_aSpawnedWaypoints.Clear();
		
		AIWaypointCycle wp = CreateUnitWaypoint();
		if (!wp) return;
		
		group.AddWaypoint(wp);
		group.SpawnUnits();
		
		m_aGroups.Insert(group);
		m_aWaypoints.Insert(wp);
	}
	
	protected void CleanFirstNullUnit()
	{		
		bool allCleared = false;
		
		while (!allCleared)
		{
			allCleared = true;
			
			foreach(int i, SCR_AIGroup group: m_aGroups)
			{
				if (!group)
				{
					m_aGroups.Remove(i);
					
					if (m_aWaypoints[i])
					{
						ClearCycleWaypoint(m_aWaypoints[i]);
					}
					
					m_aWaypoints.Remove(i);
					
					allCleared = false;
					break;
				}
			}
			
		}
	}
	
	protected void UpdateWaypoints() {		
		if (!m_MovePoint) return;		
				
		vector movePointTransform[4];
		m_MovePoint.GetWorldTransform(movePointTransform);
		
		if (vector.Distance(movePointTransform[3], m_vLastMovePointPosition[3]) < 1) return;
		
		m_vLastMovePointPosition = movePointTransform;
		
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = m_MovePoint.GetOrigin();

		foreach (int i, AIWaypointCycle cycleWp: m_aWaypoints)
		{
			SCR_AIGroup group = m_aGroups[i];
			
			ClearCycleWaypoint(cycleWp);
			AddCycleMoveWaypoints(cycleWp, params);
			cycleWp.SetTransform(movePointTransform);		
		}
	}
	
	AIWaypointCycle CreateUnitWaypoint()
	{		
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = m_MovePoint.GetOrigin();
		
		// Cycle Waypoint
		Resource waypointResource = Resource.Load("{35BD6541CBB8AC08}Prefabs/AI/Waypoints/AIWaypoint_Cycle.et");

		if (!waypointResource || !waypointResource.IsValid()) return null;

		AIWaypointCycle cycleWP = AIWaypointCycle.Cast(GetGame().SpawnEntityPrefab(waypointResource, null, params));

		if (!cycleWP) return null;
		
		AddCycleMoveWaypoints(cycleWP, params);
		
		return cycleWP;
	}
	
	void AddCycleMoveWaypoints(AIWaypointCycle cycleWP, EntitySpawnParams params)
	{		
		Resource waypointResource = Resource.Load("{750A8D1695BD6998}Prefabs/AI/Waypoints/AIWaypoint_Move.et");

		if (!waypointResource || !waypointResource.IsValid())
			return;

		// First Move Waypoint
		AIWaypoint moveWP = AIWaypoint.Cast(GetGame().SpawnEntityPrefab(waypointResource, null, params));
		if (!moveWP) return;
		
		float completeRadius = 5;
		
		if (m_MovePoint)
		{
			completeRadius = m_MovePoint.GetAreaRadius();
		}
		
		moveWP.SetCompletionRadius(completeRadius);
		
		array<AIWaypoint> waypoints = {};
		waypoints.Insert(moveWP);
		
		// Second Move Waypoint on same cords
		moveWP = AIWaypoint.Cast(GetGame().SpawnEntityPrefab(waypointResource, null, params));
		if (!moveWP) return;
		
		moveWP.SetCompletionRadius(completeRadius);
		waypoints.Insert(moveWP);
		
		cycleWP.SetWaypoints(waypoints);
	}
	
	void RefreshSpawnParameters()
	{
		if (!Replication.IsServer() || SCR_Global.IsEditMode()) return;		
		
		GetGame().GetCallqueue().Remove(SetSpawnParameters);
		GetGame().GetCallqueue().CallLater(SetSpawnParameters, 300, false);
	}
	
	protected void SetSpawnParameters()
	{		
		// Fix min max in group
		if (m_iMinInGroup > m_iMaxInGroup)
		{
			m_iMinInGroup = Math.Min(m_iMinInGroup, m_iMaxInGroup);
			m_iMaxInGroup = m_iMinInGroup;
		}
		
		if (!m_bIsEnabledAttribute)
		{
			m_bEnabled = false;
			SetSpawnParametersEnabled();
			m_WeightedUnits.Clear();
			return;
		}
		
		m_bEnabled = false;
		
		int totalCount = 0;
		
		foreach (Tuple2<string, int> setting: m_aUnitsSettings)
		{
			totalCount += setting.param2;
			
			if (setting.param2 != 0) {
				m_bEnabled = true;
				break;
			}
		}
		
		SetSpawnParametersEnabled();
		
		m_WeightedUnits.Clear();
		
		if (!m_bEnabled || totalCount == 0) return;	
		
		foreach (Tuple2<string, int> setting: m_aUnitsSettings)
		{
			m_WeightedUnits.Insert(setting.param1, setting.param2 / totalCount);
		}
		
		m_fSpawnTimer = m_iSpawnDelay;
		
		foreach (AIWaypointCycle cycleWp: m_aWaypoints)
		{
			if (!cycleWp) continue;
			
			array<AIWaypoint> waypoints = {};
			cycleWp.GetWaypoints(waypoints);
			foreach (AIWaypoint wp: waypoints)
			{
				wp.SetCompletionRadius(m_MovePoint.GetAreaRadius());
			}
		}
	}
	
	protected void SetSpawnParametersEnabled()
	{
		
	}
	
	protected void ClearCycleWaypoint(AIWaypointCycle cycleWp)
	{
		array<AIWaypoint> waypoints = {};
		cycleWp.GetWaypoints(waypoints);
		
		foreach (AIWaypoint wp: waypoints)
		{
			wp.SetCompletionRadius(9999999999);
		}
		
		cycleWp.SetWaypoints({});
	}
	
	
	vector RandomPointInRadius(vector origin, float minRadius, float maxRadius) {
		vector direction = vector.FromYaw(s_AIRandomGenerator.RandFloatXY(0, 360));
		float rand = minRadius + (s_AIRandomGenerator.RandFloat01() * maxRadius);

		return origin + rand * direction;
	}
	
	Faction GetFaction()
	{
		if (!m_FactionAffiliationComponent) return null;
		
		return m_FactionAffiliationComponent.GetAffiliatedFaction();
	}
	
	void SetFaction(Faction faction)
	{
		if (!m_FactionAffiliationComponent) return;
		
		m_FactionAffiliationComponent.SetAffiliatedFaction(faction);
		
		if (m_bFactionLess)
		{
			foreach (SCR_AIGroup group: m_aGroups)
			{
				if (!group) continue;
				
				group.SetFaction(faction);				
			}
		}
	}
	
	bool GetEnabled()
	{
		return m_bIsEnabledAttribute;
	}	
	
	void SetEnabled(int pIsEnabled)
	{
		m_bIsEnabledAttribute = pIsEnabled;
		
		RefreshSpawnParameters();
	}		
	
	int GetMaxUnits()
	{
		return m_iMaxUnits;
	}	
	
	void SetMaxUnits(int pMaxUnits)
	{
		m_iMaxUnits = pMaxUnits;
		
		RefreshSpawnParameters();
	}
	
	int GetSpawnDelay()
	{
		return m_iSpawnDelay;
	}
	
	void SetSpawnDelay(int pSpawnDelay)
	{
		m_iSpawnDelay = pSpawnDelay;
		
		RefreshSpawnParameters();
	}
	
	int GetMinInGroup()
	{
		return m_iMinInGroup;
	}
	
	void SetMinInGroup(int pMinInGroup)
	{
		m_iMinInGroup = pMinInGroup;
		
		RefreshSpawnParameters();
	}
	
	int GetMaxInGroup()
	{
		return m_iMaxInGroup;
	}
	
	void SetMaxInGroup(int pMaxInGroup)
	{
		m_iMaxInGroup = pMaxInGroup;
		
		RefreshSpawnParameters();
	}
	
	int GetRadius()
	{
		return m_fSpawnRadius;
	}
	
	void SetRadius(int pSpawnRadius)
	{
		m_fSpawnRadius = pSpawnRadius;
		
		if (m_AreaMesh)
		{
			m_AreaMesh.SetRadius(pSpawnRadius);
		}
		
		RefreshSpawnParameters();
	}
	
	array<ref Tuple2<string, int>> GetSelectedUnits()
	{
		return m_aUnitsSettings;
	}
	
	void SetSelectedUnits(notnull array<string> pSelectedResourceNames, notnull array<int> pSelectedWeights)
	{
		m_aUnitsSettings.Clear();
		
		foreach (int i, string name: pSelectedResourceNames)
		{
			m_aUnitsSettings.Insert(new Tuple2<string, int>(name, pSelectedWeights[i]));
		}
		
		RefreshSpawnParameters();
	}
}