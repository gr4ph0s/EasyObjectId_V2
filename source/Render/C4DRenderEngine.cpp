#include "DefaultRenderManager.h"
#include "C4DRenderEngine.h"

//----------------------------------------------------------------------------------------
/// Set all renders options in RenderData
///		Enable multi pass if not active
//----------------------------------------------------------------------------------------
void C4DRenderEngine::SetRenderOption()
{
	rd->SetParameter(DescID(RDATA_MULTIPASS_ENABLE), GeData(true), DESCFLAGS_SET_0);
}

//----------------------------------------------------------------------------------------
/// Get all objects id stored in one tag
/// @param[in] tag								the tag that hold the object id
/// @param[out] buffer							List of all object id
//----------------------------------------------------------------------------------------
void C4DRenderEngine::GetObjectBufferId(BaseTag* tag, maxon::BaseArray<Int32>& buffer)
{
	if (!tag)
		return;

	maxon::BaseArray<Int32> listOfCompTagEnable;
	listOfCompTagEnable.Append(COMPOSITINGTAG_ENABLECHN0);
	listOfCompTagEnable.Append(COMPOSITINGTAG_ENABLECHN1);
	listOfCompTagEnable.Append(COMPOSITINGTAG_ENABLECHN2);
	listOfCompTagEnable.Append(COMPOSITINGTAG_ENABLECHN3);
	listOfCompTagEnable.Append(COMPOSITINGTAG_ENABLECHN4);
	listOfCompTagEnable.Append(COMPOSITINGTAG_ENABLECHN5);
	listOfCompTagEnable.Append(COMPOSITINGTAG_ENABLECHN6);
	listOfCompTagEnable.Append(COMPOSITINGTAG_ENABLECHN7);
	listOfCompTagEnable.Append(COMPOSITINGTAG_ENABLECHN8);
	listOfCompTagEnable.Append(COMPOSITINGTAG_ENABLECHN9);
	listOfCompTagEnable.Append(COMPOSITINGTAG_ENABLECHN10);
	listOfCompTagEnable.Append(COMPOSITINGTAG_ENABLECHN11);

	maxon::BaseArray<Int32> listOfCompTagValue;
	listOfCompTagValue.Append(COMPOSITINGTAG_IDCHN0);
	listOfCompTagValue.Append(COMPOSITINGTAG_IDCHN1);
	listOfCompTagValue.Append(COMPOSITINGTAG_IDCHN2);
	listOfCompTagValue.Append(COMPOSITINGTAG_IDCHN3);
	listOfCompTagValue.Append(COMPOSITINGTAG_IDCHN4);
	listOfCompTagValue.Append(COMPOSITINGTAG_IDCHN5);
	listOfCompTagValue.Append(COMPOSITINGTAG_IDCHN6);
	listOfCompTagValue.Append(COMPOSITINGTAG_IDCHN7);
	listOfCompTagValue.Append(COMPOSITINGTAG_IDCHN8);
	listOfCompTagValue.Append(COMPOSITINGTAG_IDCHN9);
	listOfCompTagValue.Append(COMPOSITINGTAG_IDCHN10);
	listOfCompTagValue.Append(COMPOSITINGTAG_IDCHN11);

	GeData GeOn;
	GeData GeValue;

	//For each line in compositing tag we check if is enable and if it's we add the value to the buffer
	for (Int i = 0; i < listOfCompTagEnable.GetCount(); i++)
	{
		tag->GetParameter(DescID(listOfCompTagEnable[i]), GeOn, DESCFLAGS_GET_0);
		if (GeOn.GetBool())
		{
			tag->GetParameter(DescID(listOfCompTagValue[i]), GeValue, DESCFLAGS_GET_0);
			buffer.Append(GeValue.GetInt32());
		}
	}


	listOfCompTagEnable.Flush();
	listOfCompTagValue.Flush();

	return;
}

//----------------------------------------------------------------------------------------
/// Get all objects id that are actually inside the RenderData
/// @param[out] buffer							List of all object id
//----------------------------------------------------------------------------------------
void C4DRenderEngine::GetAllObjectsBufferInRenderData(maxon::BaseArray<Int32>& buffer)
{
	MultipassObject* mp = rd->GetFirstMultipass();
	GeData mpType;
	GeData mpValue;

	while (mp)
	{
		mp->GetParameter(DescID(MULTIPASSOBJECT_TYPE), mpType, DESCFLAGS_GET_0);

		if (mpType.GetInt32() == VPBUFFER_OBJECTBUFFER)
		{
			mp->GetParameter(DescID(MULTIPASSOBJECT_OBJECTBUFFER), mpValue, DESCFLAGS_GET_0);
			buffer.Append(mpValue.GetInt32());
		}
			
		mp = mp->GetNext();
	}
}

//----------------------------------------------------------------------------------------
/// Add all our objects id to RenderData
/// @param[out] newIDs							List of all object id
//----------------------------------------------------------------------------------------
void C4DRenderEngine::AddObjectBuffer(const maxon::BaseArray<Int32>& newIDs)
{
	maxon::BaseArray<Int32> finalIdToAdd;
	RemoveTwice(newIDs, finalIdToAdd);

	for (Int i = 0; i < finalIdToAdd.GetCount(); i++)
	{
		MultipassObject* objectBuffer = static_cast<MultipassObject*>(BaseList2D::Alloc(Zmultipass));
		objectBuffer->SetParameter(DescID(MULTIPASSOBJECT_TYPE), GeData(VPBUFFER_OBJECTBUFFER), DESCFLAGS_SET_0);
		objectBuffer->SetParameter(DescID(MULTIPASSOBJECT_OBJECTBUFFER), GeData(finalIdToAdd[i]), DESCFLAGS_SET_0);
		objectBuffer->SetName(String("ObjectBuffer_") + String::IntToString(finalIdToAdd[i]));
		rd->InsertMultipass(objectBuffer);
	}

	finalIdToAdd.Flush();
}
