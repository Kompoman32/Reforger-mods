modded class SCR_InventoryMenuUI : ChimeraMenuBase
{
	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		
		RefreshPlayerWidget();
	}
}