class RT_CF_EditableEntityUIInfo : SCR_EditableEntityUIInfo 
{
	string m_sDisplayName = string.Empty;
	string m_sHeadName = string.Empty;
	
	ResourceName m_Head;
	ref array<ref ResourceName> m_aCamos;
	SCR_Faction m_Faction;
	VisualIdentity m_Visual;
	
	void RT_CF_EditableEntityUIInfo(SCR_UIName source, SCR_Faction faction = null, VisualIdentity visual = null, array<ref ResourceName> headCamos = null)
	{
		CopyFrom(source);
		
		m_Faction = faction;
		m_Visual = visual;
		
		m_Head = GetName();
		
		m_aCamos = new array<ref ResourceName>();
		
		if (headCamos)
		{
			foreach (ResourceName camo: headCamos)
			{
				m_aCamos.Insert(camo);
			}
		}
		
		GetHeadName();
	}
	
	
	override int GetEntityLabels(out notnull array<EEditableEntityLabel> entityLabels)
	{
		return 0;
	}
	
	protected override void CopyFrom(SCR_UIName source)
	{		
		m_SlotPrefab = source.GetName();
		
		super.CopyFrom(source);
	}
	
	protected void GetHeadName()
	{
		ResourceName resource = GetName();
		
		if (!resource) return;
		
		string nameAsString = resource;
		string headName;
		LocalizedString nameLocalized;
		string nameTranslated;

		// remove Prefab path
		nameAsString = FilePath.StripPath(nameAsString);
		// remove extension
		nameAsString = FilePath.StripExtension(nameAsString);
		m_sHeadName = nameAsString;		
		
		nameLocalized = "#RT_" + nameAsString;
		nameTranslated = WidgetManager.Translate(nameLocalized);
		
		if (nameLocalized != nameTranslated)
		{
			m_sDisplayName = nameTranslated;
			Name = nameTranslated;
			return;
		}
		
		array<string> splittedName = {};
		
		// remove Head prefix
		if (nameAsString.StartsWith("Head")) {
			nameAsString = nameAsString.Substring(4, nameAsString.Length()-4);
		}
		
		nameAsString.Split("_", splittedName, true);
		nameAsString = "";
		
		foreach(int i, string split: splittedName)
		{
			string firstLeter = split.Get(0);
			firstLeter.ToUpper();
			
			string other = split.Substring(1, split.Length()-1);
			
			split = string.Format("%1%2", firstLeter, other);
			
			if (i != 0)
			{
				nameAsString += " ";
			}
			
			nameAsString += split;			
		}	
	
		m_sDisplayName = nameAsString;
		Name = nameAsString;
	}
	
	string GetHeadCamoName(int index)
	{
		if (!m_aCamos.IsIndexValid(index)) return string.Empty;
		
		string nameAsString = m_aCamos.Get(index);
		
		if (!nameAsString) return string.Empty;
		
		string headName;
		LocalizedString nameLocalized;
		string nameTranslated;

		// remove Prefab path
		nameAsString = FilePath.StripPath(nameAsString);
		// remove extension
		nameAsString = FilePath.StripExtension(nameAsString);
		m_sHeadName = nameAsString;		
		
		nameLocalized = "#RT_" + nameAsString;
		nameTranslated = WidgetManager.Translate(nameLocalized);
		
		if (nameLocalized != nameTranslated)
		{
			return nameTranslated;
		}
		
		array<string> splittedName = {};
		
		// remove Head prefix
		if (nameAsString.StartsWith("Head")) {
			nameAsString = nameAsString.Substring(4, nameAsString.Length()-4);
		}
		
		nameAsString.Split("_", splittedName, true);
		nameAsString = "";
		
		foreach(int i, string split: splittedName)
		{
			string firstLeter = split.Get(0);
			firstLeter.ToUpper();
			
			string other = split.Substring(1, split.Length()-1);
			
			split = string.Format("%1%2", firstLeter, other);
			
			if (i != 0)
			{
				nameAsString += " ";
			}
			
			nameAsString += split;			
		}
		
		return nameAsString;
	}
}