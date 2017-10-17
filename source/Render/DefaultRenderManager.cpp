#include "DefaultRenderManager.h"

//----------------------------------------------------------------------------------------
/// Search if a given int is in a BaseArray
/// @param[in] IntToSearch						The int value to search for
/// @param[in] ArrayToSearch					The BaseArray to search in
/// @return										@trueIfOtherwiseFalse{successful}
//----------------------------------------------------------------------------------------
Bool DefaultRenderManager::IsInBaseArray(Int32 IntToSearch, maxon::BaseArray<Int32>& ArrayToSearch)
{
	for (Int32 i = 0; i < ArrayToSearch.GetCount(); i++)
	{
		if (ArrayToSearch[i] == IntToSearch)
			return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------
/// Search if a given String is in a BaseArray
/// @param[in] StringToSearch					The String value to search for
/// @param[in] ArrayToSearch					The BaseArray to search in
/// @return										@trueIfOtherwiseFalse{successful}
//----------------------------------------------------------------------------------------
Bool DefaultRenderManager::IsInBaseArray(String StringToSearch, maxon::BaseArray<String>& ArrayToSearch)
{
	for (Int32 i = 0; i < ArrayToSearch.GetCount(); i++)
	{
		if (ArrayToSearch[i] == StringToSearch)
			return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------
/// Remove all defaults Object buffer inside RenderData
//----------------------------------------------------------------------------------------
void DefaultRenderManager::RemoveDefaultObjectBuffer()
{
	MultipassObject* nextMp;
	GeData mpType;

	for (MultipassObject* mp = rd->GetFirstMultipass(); mp != nullptr; mp = nextMp)
	{
		nextMp = mp->GetNext();

		mp->GetParameter(DescID(MULTIPASSOBJECT_TYPE), mpType, DESCFLAGS_GET_0);

		if (mpType.GetInt32() == VPBUFFER_OBJECTBUFFER)
		{
			mp->Remove();
			AutoFree<BaseList2D> mpFree;
			mpFree.Assign(mp);
			mpFree.Free();
		}
	}
}

//----------------------------------------------------------------------------------------
/// Get the finalId that can be adden (in order to be sure an Id is not inserted twice)
/// @param[in] newIDs							A list of all wanteds new ID
/// @param[out] finalIDs						A list of all new Id that can be added
//----------------------------------------------------------------------------------------
void DefaultRenderManager::RemoveTwice(const maxon::BaseArray<Int32>& newIDs, maxon::BaseArray<Int32>& finalIDs)
{
	maxon::BaseArray<Int32> currentIDs;
	this->GetAllObjectsBufferInRenderData(currentIDs);

	//if not into currentIds and value is not 0
	for (Int32 i = 0; i < newIDs.GetCount(); i++)
	{
		if (!IsInBaseArray(newIDs[i], currentIDs) && newIDs[i] != 0)
			finalIDs.Append(newIDs[i]);
	}
	currentIDs.Flush();
}

//----------------------------------------------------------------------------------------
/// Get the finalId that can be adden (in order to be sure an Id is not inserted twice)
/// @param[in] newIDs							A list of all wanteds new ID
/// @param[out] finalIDs						A list of all new Id that can be added
//----------------------------------------------------------------------------------------
void DefaultRenderManager::RemoveTwice(const maxon::BaseArray<String>& newIDs, maxon::BaseArray<String>& finalIDs)
{
	maxon::BaseArray<String> currentIDs;
	this->GetAllObjectsBufferInRenderData(currentIDs);

	//if not into currentIds and value is not 0
	for (Int32 i = 0; i < newIDs.GetCount(); i++)
	{
		if (!IsInBaseArray(newIDs[i], currentIDs) && newIDs[i] != 0)
			finalIDs.Append(newIDs[i]);
	}
	currentIDs.Flush();
}

//----------------------------------------------------------------------------------------
/// Get all objects ID in the scene that are present into objects
/// @param[in] obj								The obj where start the search
/// @param[in] tagId							The tag type wich identify a object buffer tag
/// @param[out] TagInScene						A list of all object buffer in the scene
//----------------------------------------------------------------------------------------
void DefaultRenderManager::SearchObjectIDInScene(BaseObject* obj, Int32 tagId, maxon::BaseArray<Int32>& TagInScene)
{
	while (obj)
	{
		BaseTag* tag = obj->GetTag(tagId);
		if (tag)
		{
			//Get object id from tag
			maxon::BaseArray<Int32> IDsOnTag;
			this->GetObjectBufferId(tag, IDsOnTag);

			//Only add tag one time
			for (Int32 i = 0; i < IDsOnTag.GetCount(); i++)
			{
				if (!IsInBaseArray(IDsOnTag[i], TagInScene))
					TagInScene.Append(IDsOnTag[i]);
			}
		}


		SearchObjectIDInScene(obj->GetDown(), tagId, TagInScene);
		obj = obj->GetNext();
	}
}

//----------------------------------------------------------------------------------------
/// Get all objects ID in the scene that are present into objects
/// @param[in] obj								The obj where start the search
/// @param[in] tagId							The tag type wich identify a object buffer tag
/// @param[out] TagInScene						A list of all object buffer in the scene
//----------------------------------------------------------------------------------------
void DefaultRenderManager::SearchObjectIDInScene(BaseObject* obj, Int32 tagId, maxon::BaseArray<String>& TagInScene)
{
	while (obj)
	{
		BaseTag* tag = obj->GetTag(tagId);
		if (tag)
		{
			//Get object id from tag
			maxon::BaseArray<String> IDsOnTag;
			this->GetObjectBufferId(tag, IDsOnTag);

			//Only add tag one time
			for (Int32 i = 0; i < IDsOnTag.GetCount(); i++)
			{
				if (!IsInBaseArray(IDsOnTag[i], TagInScene))
					TagInScene.Append(IDsOnTag[i]);
			}
		}

		SearchObjectIDInScene(obj->GetDown(), tagId, TagInScene);
		obj = obj->GetNext();
	}
}