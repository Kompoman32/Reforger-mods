[EntityEditorProps(category: "GameScripted/WavesSpawner", description: "Soldiers will go here after spawning")]
class RT_WavesSpawner_MovePointEntityClass: GenericEntityClass {}
class RT_WavesSpawner_MovePointEntity: GenericEntity {
	SCR_CustomAreaMeshComponent m_AreaMesh;
	SCR_EditableEntityComponent m_EditableEntityComponent;
	
	bool m_bIsHovered = false;
	bool m_bIsSelected = false;
	
	RT_WavesSpawnerEntity m_Spawner;
	
	[RplProp(onRplName: "onRplChangeRadius")]
	float m_fRadius = 5;
	
	void RT_WavesSpawner_MovePointEntity(IEntitySource src, IEntity parent) {
		SetEventMask(EntityEvent.INIT);
	}
	
	//------------------------------------------------------------------------------------------------
	void ~RT_WavesSpawner_MovePointEntity() {		
		SCR_BaseEditableEntityFilter hoverFilter = SCR_BaseEditableEntityFilter.GetInstance(EEditableEntityState.HOVER);
		if (hoverFilter)
			hoverFilter.GetOnChanged().Remove(OnHover);
		
		SCR_BaseEditableEntityFilter selectFilter = SCR_BaseEditableEntityFilter.GetInstance(EEditableEntityState.SELECTED);
		if (selectFilter)
			selectFilter.GetOnChanged().Remove(OnSelected);
	}
	
	override void EOnInit(IEntity owner)
	{
		m_AreaMesh = SCR_CustomAreaMeshComponent.Cast(FindComponent(SCR_CustomAreaMeshComponent));
		m_EditableEntityComponent = SCR_EditableEntityComponent.Cast(FindComponent(SCR_EditableEntityComponent));
		
		if (m_AreaMesh)
		{
			m_fRadius = m_AreaMesh.GetRadius();
		}
		
		GetGame().GetCallqueue().Remove(DelayInit);
		GetGame().GetCallqueue().CallLater(DelayInit, 500, false);
		
		SCR_BaseEditableEntityFilter hoverFilter = SCR_BaseEditableEntityFilter.GetInstance(EEditableEntityState.HOVER);
		if (hoverFilter)
			hoverFilter.GetOnChanged().Insert(OnHover);
		
		SCR_BaseEditableEntityFilter selectFilter = SCR_BaseEditableEntityFilter.GetInstance(EEditableEntityState.SELECTED);
		if (selectFilter)
			selectFilter.GetOnChanged().Insert(OnSelected);
	}
	
	void DelayInit()
	{
		if (!m_Spawner) {
			SCR_EntityHelper.DeleteEntityAndChildren(this);
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
	
		//------------------------------------------------------------------------------------------------
	protected void OnHover(EEditableEntityState state, set<SCR_EditableEntityComponent> entitiesInsert, set<SCR_EditableEntityComponent> entitiesRemove)
	{
		if (entitiesRemove) {
			foreach (auto comp: entitiesRemove)
			{
				if (comp == m_EditableEntityComponent) {
					m_bIsHovered = false;
					return;
				}
			}
		}
		
		if (entitiesInsert) {
			foreach (auto comp: entitiesInsert)
			{
				if (comp == m_EditableEntityComponent) {
					m_bIsHovered = true;
					return;
				}
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnSelected(EEditableEntityState state, set<SCR_EditableEntityComponent> entitiesInsert, set<SCR_EditableEntityComponent> entitiesRemove)
	{
		if (entitiesRemove) {
			foreach (auto comp: entitiesRemove)
			{
				if (comp == m_EditableEntityComponent) {
					m_bIsSelected = false;
					return;
				}
			}
		}
		
		if (entitiesInsert) {
			foreach (auto comp: entitiesInsert)
			{
				if (comp == m_EditableEntityComponent) {
					m_bIsSelected = true;
					return;
				}
			}
		}
	}
}