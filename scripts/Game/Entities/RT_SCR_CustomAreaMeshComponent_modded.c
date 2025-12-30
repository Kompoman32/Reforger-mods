modded class SCR_CustomAreaMeshComponent : ScriptComponent
{
	//------------------------------------------------------------------------------------------------
	void SetRadius(float pRadius)
	{
		m_fRadius = pRadius;
		GenerateAreaMesh();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Set material used for area mesh
	void SetMaterial(ResourceName pResourceName)
	{
		m_Material = pResourceName;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Get material used for area mesh
	ResourceName GetMaterialPublic()
	{
		return m_Material;
	}
}