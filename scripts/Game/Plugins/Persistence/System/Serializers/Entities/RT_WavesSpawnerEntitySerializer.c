class RT_PersistentSpawnerEntity
{
	bool m_Enabled;
	int m_MaxUnits;
	int m_SpawnDelay;
	int m_MinInGroup;
	int m_MaxInGroup;
	int m_Radius;
	int m_UseVehicles;
	
	FactionKey m_Faction;
	vector m_MovePointPos[4];
	int m_MovePointRadius;
	
	ref array<string> m_UnitsNames;
	ref array<int> m_UnitsWeights;
	
	ref array<UUID> m_Groups;
	ref array<int> m_GroupsSizes;
}

class RT_WavesSpawnerEntitySerializer : GenericEntitySerializer
{
	const static int CURRENT_VERSION = 1;
	
	//------------------------------------------------------------------------------------------------
	override static typename GetTargetType()
	{
		return RT_WavesSpawnerEntity;
	}

	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity entity, notnull SaveContext context)
	{
		const RT_WavesSpawnerEntity wavesSpawner = RT_WavesSpawnerEntity.Cast(entity);
		const RT_WavesSpawner_MovePointEntity movePoint = wavesSpawner.GetMovePoint();
		
		RT_PersistentSpawnerEntity savedSpawner();
		// Static Settings
		savedSpawner.m_Enabled = wavesSpawner.GetEnabled();
		savedSpawner.m_MaxUnits = wavesSpawner.GetMaxUnits();
		savedSpawner.m_SpawnDelay = wavesSpawner.GetSpawnDelay();
		savedSpawner.m_MinInGroup = wavesSpawner.GetMinInGroup();
		savedSpawner.m_MaxInGroup = wavesSpawner.GetMaxInGroup();
		savedSpawner.m_Radius = wavesSpawner.GetRadius();
		savedSpawner.m_UseVehicles = wavesSpawner.GetUseVehicles();
		
		if (wavesSpawner.GetFaction())
		{
			savedSpawner.m_Faction = wavesSpawner.GetFaction().GetFactionKey();			
		}
		
		// Move Point
		if (movePoint)
		{
			vector movePointPos[4];
			movePoint.GetTransform(movePointPos);
			savedSpawner.m_MovePointPos = movePointPos;
			savedSpawner.m_MovePointRadius = movePoint.GetAreaRadius();
		}
		
		
		// Names and Weights
		array<ref Tuple2<string, int>> units = wavesSpawner.GetSelectedUnits();
		
		array<string> unitsNames = {};
		array<int> unitsWeights = {};
		
		foreach (int i, Tuple2<string, int> unit: units)
		{
			unitsNames.Insert(unit.param1);
			unitsWeights.Insert(unit.param2);
		}
		
		savedSpawner.m_UnitsNames = unitsNames;
		savedSpawner.m_UnitsWeights = unitsWeights;		
		
		// Groups
		array<UUID> groupsUUIDs();
		array<int> groupsSizes();
		array<SCR_AIGroup> groups = wavesSpawner.GetGroups();
		
		foreach (auto group : groups)
		{
			if (!group)
				continue;

			const UUID uuid = GetSystem().GetId(group);
			if (!uuid.IsNull()) {	
				groupsUUIDs.Insert(uuid);
				groupsSizes.Insert(group.GetMaxUnitsToSpawn());
			}
		}
		
		savedSpawner.m_Groups = groupsUUIDs;		
		savedSpawner.m_GroupsSizes = groupsSizes;		
		
		// SAVE
		context.WriteValue("version", CURRENT_VERSION);
		const bool prev = context.EnableTypeDiscriminator(false);
		context.Write(savedSpawner);
		context.EnableTypeDiscriminator(prev);		
		
		return ESerializeResult.OK;
	}

	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity entity, notnull LoadContext context)
	{
		const RT_WavesSpawnerEntity wavesSpawner = RT_WavesSpawnerEntity.Cast(entity);
		const RT_WavesSpawner_MovePointEntity movePoint = wavesSpawner.GetMovePoint();
		
		int version;
		context.Read(version);

		RT_PersistentSpawnerEntity savedSpawner();
		const bool prev = context.EnableTypeDiscriminator(false);
		context.Read(savedSpawner);
		context.EnableTypeDiscriminator(prev);
		
		// Static Settings
		wavesSpawner.SetEnabled(savedSpawner.m_Enabled);
		wavesSpawner.SetMaxUnits(savedSpawner.m_MaxUnits);
		wavesSpawner.SetSpawnDelay(savedSpawner.m_SpawnDelay);
		wavesSpawner.SetMinInGroup(savedSpawner.m_MinInGroup);
		wavesSpawner.SetMaxInGroup(savedSpawner.m_MaxInGroup);
		wavesSpawner.SetRadius(savedSpawner.m_Radius);
		wavesSpawner.SetUseVehicles(savedSpawner.m_UseVehicles);
		
		if (savedSpawner.m_Faction) {
			SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
			if (factionManager) {
				Faction faction = factionManager.GetFactionByKey(savedSpawner.m_Faction);
				if (faction) {
					wavesSpawner.SetFaction(faction);
				}
			}
		}
		
		// Move Point
		if (movePoint)
		{
			movePoint.SetTransform(savedSpawner.m_MovePointPos);
			movePoint.SetAreaRadius(savedSpawner.m_MovePointRadius);
		}
		
		// Names and Weights
		wavesSpawner.SetSelectedUnits(savedSpawner.m_UnitsNames, savedSpawner.m_UnitsWeights);
		
		wavesSpawner.GetGroups().Clear();
		wavesSpawner.GetWaypoints().Clear();
		
		// Groups
		array<UUID> groupUUIDs = savedSpawner.m_Groups;
		
		foreach (int i, auto group : groupUUIDs)
		{
			if (i >= savedSpawner.m_GroupsSizes.Count()) continue;
			
			Tuple2<RT_WavesSpawnerEntity, int> groupContext(wavesSpawner, savedSpawner.m_GroupsSizes[i]);
			PersistenceWhenAvailableTask groupTask(OnGroupAvailable, groupContext);
			GetSystem().WhenAvailable(group, groupTask);
		}
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	protected static void OnGroupAvailable(Managed instance, PersistenceDeferredDeserializeTask task, bool expired, Managed context)
	{
		SCR_AIGroup group = SCR_AIGroup.Cast(instance);
		if (!group) return;
		
		auto groupContext = Tuple2<RT_WavesSpawnerEntity, int>.Cast(context);
		group.m_aUnitPrefabSlots.Clear();
		
		if (groupContext.param1)
		{
			RT_WavesSpawnerEntity spawner = groupContext.param1;
			
			if (spawner.GetFaction())
			{
				group.SetFaction(spawner.GetFaction());
			}		
			
			array<SCR_AIGroup> groups = spawner.GetGroups();
			groups.Insert(group);
			
			ClearGroupWaypoints(group);
			
			AIWaypointCycle wp = spawner.CreateUnitWaypoint();		
			group.AddWaypoint(wp);
			
			wp.PerformOn(group);
			
			array<AIWaypointCycle> waypoints = spawner.GetWaypoints();
			waypoints.Insert(wp);	
			
			array<AIWaypoint> waypointsG = {};
			group.GetWaypoints(waypointsG);	
		}
	}
	
	//------------------------------------------------------------------------------------------------
	static void ClearGroupWaypoints(SCR_AIGroup group)
	{
		if (!group) return;
		
		array<AIWaypoint> waypoints = {};
		group.GetWaypoints(waypoints);
		
		foreach (AIWaypoint wp: waypoints)
		{
			if (!wp) continue;
			
			if (SCR_TimedWaypoint.Cast(wp))
				SCR_TimedWaypoint.Cast(wp).SetHoldingTime(0);
			
			wp.SetCompletionRadius(9999999999);
			
			AIWaypointCycle cycleWp = AIWaypointCycle.Cast(wp);
			if (cycleWp) {
				cycleWp.SetWaypoints({});
			}
		}
		
		group.CompleteAllWaypoints();
	}
}
