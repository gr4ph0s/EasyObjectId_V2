#include "DefaultRenderManager.h"
#include "ArnoldRenderEngine.h"

#define ID_ARNOLD_DRIVER 1030141
#define ID_ARNOLD_OBJECT_PASS 1030369

#define C4DAI_DRIVER_TYPE 101
#define C4DAIN_DRIVER_C4D_DISPLAY 1927516736
#define C4DAI_AOV_DATATYPE 104

//----------------------------------------------------------------------------------------
/// Get the First Arnold driver in the scene of the RenderData filled in the init
/// @return										the Base object that represent an arnold driver
//----------------------------------------------------------------------------------------
BaseObject* ArnoldRenderEngine::GetDriver(){
	BaseDocument* doc = rd->GetDocument();
	BaseObject* searched_obj = doc->GetFirstObject();

	//Search in the scene
	while (searched_obj)
	{
		if (searched_obj->GetType() == ID_ARNOLD_DRIVER)
			return searched_obj;

		searched_obj = searched_obj->GetNext();
	}
	
	// Create if search give nothing
	AutoAlloc<BaseObject> obj(ID_ARNOLD_DRIVER);
	if (!obj)
		return nullptr;

	obj->SetName("Object Buffer Driver");
	obj->SetParameter(DescID(C4DAI_DRIVER_TYPE), GeData(Int32(C4DAIN_DRIVER_C4D_DISPLAY)), DESCFLAGS_SET_0);

	// Insert To obj
	BaseObject* finalObj = obj.Release();
	doc->InsertObject(finalObj, nullptr, nullptr);
	return finalObj;
}

//----------------------------------------------------------------------------------------
/// Set all renders options in RenderData
///		For arnold there is nothing to do since Arnold care of us <3
//----------------------------------------------------------------------------------------
void ArnoldRenderEngine::SetRenderOption()
{
}

//----------------------------------------------------------------------------------------
/// Get all objects id stored in one tag
/// @param[in] tag								the tag that hold the object id
/// @param[out] buffer							List of all object id
//----------------------------------------------------------------------------------------
void ArnoldRenderEngine::GetObjectBufferId(BaseTag* tag, maxon::BaseArray<String>& buffer)
{
	if (!tag)
		return;

	GeData GeValue;
	tag->GetParameter(DescID(2100), GeValue, DESCFLAGS_GET_0);
	buffer.Append(GeValue.GetString());

	return;
}

//----------------------------------------------------------------------------------------
/// Get all objects id that are actually inside the driver
/// @param[out] buffer							List of all object id
//----------------------------------------------------------------------------------------
void ArnoldRenderEngine::GetAllObjectsBufferInRenderData(maxon::BaseArray<String>& buffer)
{
	GeData passType;
	BaseObject* passName = driver->GetDown();

	while (passName)
	{
		passName->GetParameter(DescID(C4DAI_AOV_DATATYPE), passType, DESCFLAGS_GET_0);

		// Check if it's float
		if (passType.GetInt32() == 4)
			buffer.Append(passName->GetName());

		passName = passName->GetNext();
	}
}

//----------------------------------------------------------------------------------------
/// Create arnold driver object in order to create pass for each object id
/// @param[out] newIDs							List of all object id
//----------------------------------------------------------------------------------------
void ArnoldRenderEngine::AddObjectBuffer(const maxon::BaseArray<String>& newIDs)
{
	maxon::BaseArray<String> finalIdToAdd;
	RemoveTwice(newIDs, finalIdToAdd);

	for (Int32 i = 0; i < finalIdToAdd.GetCount(); i++)
	{
		AutoAlloc<BaseObject> obj(ID_ARNOLD_OBJECT_PASS);
		if (!obj)
			continue;

		obj->SetParameter(DescID(C4DAI_AOV_DATATYPE), GeData(Int32(4)), DESCFLAGS_SET_0);
		obj->SetName(finalIdToAdd[i]);
		driver->GetDocument()->InsertObject(obj.Release(), driver, nullptr);
	}

	finalIdToAdd.Flush();
}
