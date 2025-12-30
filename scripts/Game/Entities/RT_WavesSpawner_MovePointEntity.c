[EntityEditorProps(category: "GameScripted/WavesSpawner", description: "Soldiers will go here after spawning")]
class RT_WavesSpawner_MovePointEntityClass: GenericEntityClass {}
class RT_WavesSpawner_MovePointEntity: GenericEntity {
	SCR_CustomAreaMeshComponent m_AreaMesh;
	
	RT_WavesSpawnerEntity m_Spawner;
	
	[RplProp(onRplName: "onRplChangeRadius")]
	float m_fRadius = 5;
	
	void RT_WavesSpawner_MovePointEntity(IEntitySource src, IEntity parent) {
		SetEventMask(EntityEvent.INIT);
	}
	
	override void EOnInit(IEntity owner)
	{
		m_AreaMesh = SCR_CustomAreaMeshComponent.Cast(FindComponent(SCR_CustomAreaMeshComponent));
		
		if (m_AreaMesh)
		{
			m_fRadius = m_AreaMesh.GetRadius();
		}
	}
	
	void SetAreaRadius(float pRadius)
	{		
		m_fRadius = pRadius;
		Replication.BumpMe();
		
		if (m_AreaMesh)
		{
			m_AreaMesh.SetRadius(pRadius);

		}
		
		if (m_Spawner)
		{
			m_Spawner.RefreshSpawnParameters();
		}
	}
	
	float GetAreaRadius()
	{		
		if (m_AreaMesh) return m_AreaMesh.GetRadius();
		
		return 0;
	}
	
	void onRplChangeRadius()
	{
		if (Replication.IsServer()) return;
		
		SetAreaRadius(m_fRadius);
	}

}