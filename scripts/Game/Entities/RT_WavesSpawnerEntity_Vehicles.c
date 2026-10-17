[EntityEditorProps(category: "GameScripted/WavesSpawner", description: "Allows infinite spawning of vehicles")]
class RT_WavesSpawnerEntity_VehiclesClass: RT_WavesSpawnerEntityClass {}
class RT_WavesSpawnerEntity_Vehicles: RT_WavesSpawnerEntity {
	protected ref array<Vehicle> m_aVehicles;
	
	protected vector matForCalcGetCurrentGroupsCount_Spawner[4];
	protected vector matForCalcGetCurrentGroupsCount_Vehicle[4];
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		m_bFactionLess = false;
		m_aVehicles = {};
		m_iMaxUnits = 4;
	}
	
	//------------------------------------------------------------------------------------------------
	protected override void CreateMovePoint()
	{		
	}
	
	//------------------------------------------------------------------------------------------------
	protected override void CreateMovePoint_Proxy()
	{
	}
	
	//------------------------------------------------------------------------------------------------
	protected override bool Spawn_CleanNullUnits()
	{	
		int oldVehCount = m_aVehicles.Count();
		
		m_aVehicles.Clear();
		GetGame().GetWorld().QueryEntitiesBySphere(GetOrigin(), m_fSpawnRadius, CheckSpawnVehicles_Add, CheckSpawnVehicles_FilterCallback);
		
		m_iGroupsCount = m_aVehicles.Count();
		return oldVehCount != GetGroupsCount();
	}

	//------------------------------------------------------------------------------------------------
	protected override bool AddNearbyUnits_AddEntity(IEntity pEntity)
	{
		Vehicle vehicleEntity = Vehicle.Cast(pEntity);
		
		if (!vehicleEntity ) return true;
		
		EntityPrefabData prefabData = vehicleEntity .GetPrefabData();
		if (!prefabData)
			return true;

		ResourceName prefabName = prefabData.GetPrefabName();
		if (!prefabName)
			return true;
				
		bool hasAlready = false;
		
		foreach (int i, Tuple2<string, int> setting: m_aUnitsSettings)
		{
			if (setting.param1 == prefabName) {
				hasAlready = true;
				break;
			}
		}
		
		if (!hasAlready)
		{
			m_aUnitsSettings.Insert(new Tuple2<string, int>(prefabName, 0));
		}
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	protected override bool AddNearbyUnitsFilterCallback(IEntity pEntity)
	{
		return !!Vehicle.Cast(pEntity);
	}
	
	protected bool CheckSpawnVehicles_Add(IEntity pEntity)
	{
		if (!Vehicle.Cast(pEntity)) return true;
		
		m_aVehicles.Insert(Vehicle.Cast(pEntity));
		
		return true;
	}
		
	//------------------------------------------------------------------------------------------------
	protected bool CheckSpawnVehicles_FilterCallback(IEntity pEntity)
	{
		return !!Vehicle.Cast(pEntity);
	}
		
	//------------------------------------------------------------------------------------------------
	protected override void Spawn() 
	{	
		if (GetGroupsCount() >= m_iMaxUnits)
			return;
		
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		GetWorldTransform(params.Transform);
		
		vector offsetPos = Spawn_GetPointToSpawn();
		
		if (offsetPos == vector.Zero)
			return;
		
		params.Transform[3] = offsetPos;
		
		ResourceName currentVehiclePrefab;
		
		m_WeightedUnits.GetRandomValue(currentVehiclePrefab, s_AIRandomGenerator);
		
		Resource currentVehicleResource = Resource.Load(currentVehiclePrefab);
		if (!currentVehicleResource)
			return;
		
		Vehicle.Cast(GetGame().SpawnEntityPrefab(currentVehicleResource, null, params));
		
		m_iGroupsCount++;
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	protected override vector Spawn_GetPointToSpawn()
	{
		array<vector> offsetPos = {};
		
		int count = SCR_WorldTools.FindAllEmptyTerrainPositions(
			offsetPos,
			RandomPointInRadius(GetOrigin(), 1, m_fSpawnRadius * 3/4),
			m_fSpawnRadius,
			1.5,
			2.5,
			m_iSpawnFindPositionAttempts,
			TraceFlags.ENTS 
		);
		
		if (count == 0) return vector.Zero;
		
		return offsetPos.GetRandomElement();
	}
	
	//------------------------------------------------------------------------------------------------
	array<Vehicle> GetVehicles()
	{
		return m_aVehicles;
	}
	
//	//------------------------------------------------------------------------------------------------
//	override int GetGroupsCount()
//	{
//		GetTransform(matForCalcGetCurrentGroupsCount_Spawner);
//		
//		int counter = 0;
//		foreach (Vehicle veh: m_aVehicles)
//		{
//			if (!veh) continue;
//			
//			veh.GetTransform(matForCalcGetCurrentGroupsCount_Vehicle);
//			
//			if (vector.Distance(GetOrigin(), matForCalcGetCurrentGroupsCount_Vehicle[3]) < GetRadius()) 
//				counter++;
//		}
//		return counter;
//	}
}