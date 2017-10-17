#include "DefaultRenderManager.h"
#include "IrayRenderEngine.h"

#define ID_C4D_CLICK_ACTION 1440

#define ID_IRAY_VIDEOPOST 1034128

#define ID_IRAY_CUSTOM_DATATYPE 100
#define ID_IRAY_INSTANCE_TAG_LPE_LABEL 1003

#define ID_IRAY_LPE_ADD_PASS 2704
#define VP_IRAY_LPE_COUNT 2799
#define VP_IRAY_LPE_START 2800

//----------------------------------------------------------------------------------------
/// Get the Iray Video Post, it's assume you already checked Iray is the render engine.
/// @return										the Iray Video Post
//----------------------------------------------------------------------------------------
BaseVideoPost* IrayRenderEngine::GetIrayVideoPost()
{
	BaseVideoPost* vp = rd->GetFirstVideoPost();
	while (vp->GetType() != ID_IRAY_VIDEOPOST)
	{
		vp->GetNext();
	}
	return vp;
}

//----------------------------------------------------------------------------------------
/// Get the actual buffer id value For exemple "E'Cube'.*L" return Cube
/// @param[in] original							the original String of the render data
/// @return										String of only the pass name
//----------------------------------------------------------------------------------------
String IrayRenderEngine::GetNameObjectBuffer(const String original)
{
	try
	{
		//remove space
		String strWithoutSpace;
		Int32 counter = 0;
		for (Int32 i = 0; i < original.GetLength(); i++)
		{
			if (original[i] != String(" ")[0])
			{
				strWithoutSpace.Insert(counter, original[i]);
				++counter;
			}
		}

		// Check if start with "E'" and end with "'.*L"
		String startMatch = "E'";
		String endMatch = "'.*L";

		Int32 posStart = 0;
		Int32 posEnd = 0;

		if (!strWithoutSpace.FindFirst(startMatch, &posStart) || !strWithoutSpace.FindFirst(endMatch, &posEnd))
			return String();

		return strWithoutSpace.SubStr(posStart + 2, posEnd - (posStart + startMatch.GetLength()));
	}
	catch (int e)
	{
		return String();
	}
	return String();
}

//----------------------------------------------------------------------------------------
/// Get all pass since we are limited to 30 pass in Iray
/// @param[in] count							The number of pass we want to add
/// @param[out] buffer							List of all pass we have to add
//----------------------------------------------------------------------------------------
void IrayRenderEngine::GetEmptyPassInRenderData(const Int32& count, maxon::BaseArray<Int32>& buffer)
{
	GeData bcDataType;
	for (Int32 i = 0; i < count; i++)
	{
		Int32 bcID = VP_IRAY_LPE_START + 1 + i * 3;
		IrayVideoPost->GetParameter(DescID(bcID-1), bcDataType, DESCFLAGS_GET_0);

		//check if it's none 
		if (bcDataType.GetType() == DA_NIL || bcDataType.GetType() == DA_LONG && bcDataType.GetInt32() == 0)
			buffer.Append(bcID);
	}
}

//----------------------------------------------------------------------------------------
/// Get all availables bc wich are ready to be written
/// @param[in] newIds							All the ids we want to add (is not garanted that all ids will be included since Iray limite to 30 pass)
/// @param[out] aviaibleIds						List of all id where we can write
//----------------------------------------------------------------------------------------
void IrayRenderEngine::GetBcIDWhereToInsertObjectBuffer(maxon::BaseArray<String>& newIds, maxon::BaseArray<Int32>& aviaibleIds)
{
	Int32 const maxCount = 30;

	GeData GeCount;
	IrayVideoPost->GetParameter(DescID(VP_IRAY_LPE_COUNT), GeCount, DESCFLAGS_GET_0);
	Int32 const count = GeCount.GetInt32();

	maxon::BaseArray<Int32> IdsFree;
	GetEmptyPassInRenderData(count, IdsFree);

	Int32 const numberAvailablePass = IdsFree.GetCount();
	Int32 const maxNumberAvailablePass = maxCount - (count - numberAvailablePass);
	Int32 const numberWanted = newIds.GetCount();

	// Get the max number from numberPassToAdd and maxNumberAvailablePass
	Int32 numberPassToAdd;
	if (numberWanted < maxNumberAvailablePass)
		numberPassToAdd = numberWanted;
	else
		numberPassToAdd = maxNumberAvailablePass;

	// Create pass if needed
	if (numberPassToAdd <= numberAvailablePass)
	{
		aviaibleIds.CopyFrom(IdsFree);
	}
	else
	{
		// Click on button Add Pass
		for (Int32 i = 0; i < numberPassToAdd - numberAvailablePass; i++)
		{
			BaseContainer msg(BFM_ACTION);
			msg.SetInt32(BFM_ACTION_ID, ID_C4D_CLICK_ACTION);
			msg.SetInt32(BFM_ACTION_VALUE, TRUE);

			DescriptionCommand dc;
			dc.id = DescID(DescLevel(ID_IRAY_LPE_ADD_PASS, DTYPE_BUTTON, IrayVideoPost->GetType()));

			DescriptionCheckUpdate du;
			du.doc = IrayVideoPost->GetDocument();
			du.descid = &dc.id;
			du.drawflags = 0;

			IrayVideoPost->Message(MSG_DESCRIPTION_COMMAND, (void*)&dc);
			IrayVideoPost->Message(MSG_DESCRIPTION_CHECKUPDATE, (void*)&du);

			EventAdd();
		}
	}

	IrayVideoPost->GetParameter(DescID(VP_IRAY_LPE_COUNT), GeCount, DESCFLAGS_GET_0);
	GetEmptyPassInRenderData(GeCount.GetInt32(), aviaibleIds);
}

//----------------------------------------------------------------------------------------
/// Set all renders options in RenderData
///		Add post effect pass if not pressent
//----------------------------------------------------------------------------------------
void IrayRenderEngine::SetRenderOption()
{
	rd->SetParameter(DescID(RDATA_MULTIPASS_ENABLE), GeData(true), DESCFLAGS_SET_0);

	//Check if post effect is pressent
	MultipassObject* mp = rd->GetFirstMultipass();
	Bool getPostEffect = false;
	GeData value;
	while (mp || getPostEffect)
	{
		//If there is a post effect we leave
		mp->GetParameter(DescID(MULTIPASSOBJECT_TYPE), value, DESCFLAGS_GET_0);
		if (value.GetInt32() == 12)
			return;
		
		mp = mp->GetNext();
	}

	//If we don't get post effect mp we create one
	if (!mp)
	{
		MultipassObject* postEffectMp = static_cast<MultipassObject*>(BaseList2D::Alloc(Zmultipass));
		postEffectMp->SetParameter(DescID(MULTIPASSOBJECT_TYPE), GeData(VPBUFFER_ALLPOSTEFFECTS), DESCFLAGS_SET_0);
		rd->InsertMultipass(postEffectMp);
	}
}

//----------------------------------------------------------------------------------------
/// Get all objects id stored in one tag
/// @param[in] tag								the tag that hold the object id
/// @param[out] buffer							List of all object id
//----------------------------------------------------------------------------------------
void IrayRenderEngine::GetObjectBufferId(BaseTag* tag, maxon::BaseArray<String>& buffer)
{
	if (!tag)
		return;

	GeData GeValue;
	tag->GetParameter(DescID(ID_IRAY_INSTANCE_TAG_LPE_LABEL), GeValue, DESCFLAGS_GET_0);
	buffer.Append(GeValue.GetString());

	return;
}

//----------------------------------------------------------------------------------------
/// Get all objects id that are actually inside the RenderData
/// @param[out] buffer							List of all object id
//----------------------------------------------------------------------------------------
void IrayRenderEngine::GetAllObjectsBufferInRenderData(maxon::BaseArray<String>& buffer)
{
	GeData lpeCount;
	GeData bcValue;
	GeData bcDataType;
	IrayVideoPost->GetParameter(DescID(VP_IRAY_LPE_COUNT), lpeCount, DESCFLAGS_GET_0);

	//For each pass
	for (Int32 i = 0; i < lpeCount.GetInt32(); i++)
	{
		Int32 bcID = VP_IRAY_LPE_START + 1 + i * 3;
		IrayVideoPost->GetParameter(DescID(bcID - 1), bcDataType, DESCFLAGS_GET_0);

		//If it's custom datatype
		if (bcDataType.GetType() == DA_LONG && bcDataType.GetInt32() == ID_IRAY_CUSTOM_DATATYPE)
		{
			IrayVideoPost->GetParameter(DescID(bcID), bcValue, DESCFLAGS_GET_0);
			String text = GetNameObjectBuffer(bcValue.GetString());

			//Only add if it's not empty
			if (text.Content())
				buffer.Append(text);
		}
	}
}

//----------------------------------------------------------------------------------------
/// Add all our objects id to RenderData (is not garanted that all ids will be included since Iray limite to 30 pass)
/// @param[out] newIDs							List of all object id
//----------------------------------------------------------------------------------------
void IrayRenderEngine::AddObjectBuffer(const maxon::BaseArray<String>& newIDs)
{
	maxon::BaseArray<String> finalIdToAdd;
	maxon::BaseArray<Int32> WhereToAdd;

	RemoveTwice(newIDs, finalIdToAdd);
	GetBcIDWhereToInsertObjectBuffer(finalIdToAdd, WhereToAdd);

	// We repeat this step in case of hidded custom Param so we have to check them since whe don't get them in the start.
	finalIdToAdd.Flush();
	WhereToAdd.Flush();
	RemoveTwice(newIDs, finalIdToAdd);
	GetBcIDWhereToInsertObjectBuffer(finalIdToAdd, WhereToAdd);

	// Get bigger array
	Int32 maxCount;
	if (WhereToAdd.GetCount() < finalIdToAdd.GetCount())
		maxCount = WhereToAdd.GetCount();
	else
		maxCount = finalIdToAdd.GetCount();

	// Then Add data
	for (Int32 i = 0; i < maxCount; i++)
	{
		// Set to custom
		IrayVideoPost->SetParameter(DescID(WhereToAdd[i]-1), GeData(100), DESCFLAGS_SET_0);

		// Set String pass
		GeData finalString = String("E'") + finalIdToAdd[i] + String("'.*L");
		IrayVideoPost->SetParameter(DescID(WhereToAdd[i]), finalString, DESCFLAGS_SET_0);
	}

}
