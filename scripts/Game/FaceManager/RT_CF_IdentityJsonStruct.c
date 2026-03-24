class RT_CF_IdentityJsonStruct: JsonApiStruct
{
	string head;
	string body;

	void RT_CF_IdentityJsonStruct()
	{
		RegV("head");
		RegV("body");
	}
}