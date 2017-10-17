#include "DefaultRenderManager.h"
#include "VrayRenderEngine.h"

#define ID_VRAY_DRIVER 1019782
#define ID_VRAY_SCENE_HOOK 1028268

#define ID_MPNODE 1028267
#define MPNODE_TYPE 1000

#define MPNODE_TYPE_OBJID 1404
#define VRAYRE_OBJID 1300
#define VRAYRE_AFFECTMATTE 1400
#define VRAYRE_AA 1202
#define MPNODE_ISENABLED 1050

#define VRAYCOMPOSITINGTAG_OBJID 1143


//----------------------------------------------------------------------------------------
/// Get the Vray Video Post, it's assume you already checked Vray is the render engine.
/// @return										the Base object that represent an vray Video Post
//----------------------------------------------------------------------------------------
BaseVideoPost* VrayRenderEngine::GetVideoPost(){
	BaseVideoPost* vp = rd->GetFirstVideoPost();
	while (vp->GetType() != ID_VRAY_DRIVER)
	{
		vp->GetNext();
	}
	return vp;
}

//----------------------------------------------------------------------------------------
/// Get the Vray Master Node, it's assume you already checked Vray is the render engine.
/// @return										the Base object that represent an Vray MpNode who host all other
//----------------------------------------------------------------------------------------
BaseObject* VrayRenderEngine::GetMasterNode(){
	// Get the scene Hook
	BaseSceneHook* sceneHook = rd->GetDocument()->FindSceneHook(ID_VRAY_SCENE_HOOK);
	if (!sceneHook) return nullptr;

	// Get the last Branch Info and check if there is data into it
	BranchInfo branch[10000];
	Int32 count = sceneHook->GetBranchInfo(branch, 10000, GETBRANCHINFO_0);
	BaseObject* node = static_cast<BaseObject*>(branch[count - 1].head->GetFirst());

	// If there is no data that we open the Mp manager, it initizialize data for us.
	if (!node)
	{
		CallCommand(1028266);
		node = static_cast<BaseObject*>(branch[count - 1].head->GetFirst());
	}

	// If we still have no data it's weird
	if (!node) return nullptr;

	// The 4th mpNode is the MpNode who hold all Object ID
	node = node->GetNext()->GetNext()->GetNext();
	return node;
}

//----------------------------------------------------------------------------------------
/// Remove all Vray objects Id from the render data
//----------------------------------------------------------------------------------------
void VrayRenderEngine::removeAllVrayBufferId()
{
	BaseObject* MPNode = masterNode->GetDownLast();
	BaseObject* MPNodeBuffer;
	GeData GePassType;

	while (MPNode)
	{
		MPNodeBuffer = MPNode;
		MPNode = MPNode->GetPred();

		// Check if the mpnode is an object id mpnode
		MPNodeBuffer->GetParameter(MPNODE_TYPE, GePassType, DESCFLAGS_GET_0);
		if (GePassType.GetInt32() != MPNODE_TYPE_OBJID)
			continue;

		// Remove object ID MpNode object
		MPNodeBuffer->Remove();
		AutoFree<BaseList2D> MPNodeBufferFree;
		MPNodeBufferFree.Assign(MPNodeBuffer);
		MPNodeBufferFree.Free();
	}
}

//----------------------------------------------------------------------------------------
/// Set all renders options in RenderData
///		Add post effect pass if not pressent
//----------------------------------------------------------------------------------------
void VrayRenderEngine::SetRenderOption()
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
void VrayRenderEngine::GetObjectBufferId(BaseTag* tag, maxon::BaseArray<Int32>& buffer)
{
	if (!tag)
		return;

	GeData GeValue;
	tag->GetParameter(DescID(VRAYCOMPOSITINGTAG_OBJID), GeValue, DESCFLAGS_GET_0);
	buffer.Append(GeValue.GetInt32());

	return;
}

//----------------------------------------------------------------------------------------
/// Get all objects id that are actually inside RenderData
/// @param[out] buffer							List of all object id
//----------------------------------------------------------------------------------------
void VrayRenderEngine::GetAllObjectsBufferInRenderData(maxon::BaseArray<Int32>& buffer)
{
	BaseObject* MPNode = masterNode->GetDown();
	GeData GePassType;
	GeData GeObjectId;

	// Loop over all MpNode
	while (MPNode)
	{
		// Check if the mpnode is an object id mpnode
		MPNode->GetParameter(MPNODE_TYPE, GePassType, DESCFLAGS_GET_0);
		if (GePassType.GetInt32() != MPNODE_TYPE_OBJID)
			continue;

		// Get Data and check is not null
		MPNode->GetParameter(VRAYRE_OBJID, GeObjectId, DESCFLAGS_GET_0);
		if (GeObjectId.GetInt32())
			buffer.Append(GeObjectId.GetInt32());

		MPNode = MPNode->GetNext();
	}
}


//----------------------------------------------------------------------------------------
/// Create arnold driver object in order to create pass for each object id
/// @param[out] newIDs							List of all object id
//----------------------------------------------------------------------------------------
void VrayRenderEngine::AddObjectBuffer(const maxon::BaseArray<Int32>& newIDs)
{
	maxon::BaseArray<Int32> finalIdToAdd;
	RemoveTwice(newIDs, finalIdToAdd);

	for (Int32 i = 0; i < finalIdToAdd.GetCount(); i++)
	{
		AutoAlloc<BaseObject> obj(ID_MPNODE);
		if (!obj)
			continue;

		// Set Data
		obj->SetName(String("Oject_ID_") + String::IntToString(finalIdToAdd[i]));
		obj->SetParameter(MPNODE_TYPE, GeData(Int32(MPNODE_TYPE_OBJID)), DESCFLAGS_SET_0);					// Type ID
		obj->SetParameter(VRAYRE_OBJID, GeData(Int32(finalIdToAdd[i])), DESCFLAGS_SET_0);								// Obj ID
		obj->SetParameter(VRAYRE_AFFECTMATTE, GeData(Bool(options.AffectMatteObject)), DESCFLAGS_SET_0);	// Affect Matte
		obj->SetParameter(VRAYRE_AA, GeData(Bool(options.ConsiderForAA)), DESCFLAGS_SET_0);				// Consider for AA
		obj->SetParameter(MPNODE_ISENABLED, GeData(Bool(true)), DESCFLAGS_SET_0);							// Enable the pass

		// Insert It
		GePrint(masterNode->GetName());
		rd->GetDocument()->InsertObject(obj.Release(), masterNode, nullptr);
	}
}
