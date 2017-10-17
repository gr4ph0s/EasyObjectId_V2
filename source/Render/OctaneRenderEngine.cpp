#include "DefaultRenderManager.h"
#include "OctaneRenderEngine.h"

#define ID_OCTANE_DRIVER 1029525

#define ID_OCTANE_OBJECTTAG_LAYERID 1288

#define ID_OCTANE_SET_PASSES_ENABLED 3280
#define ID_OCTANE_SET_PASSES_OBJID 3342
#define VP_PASSES_RL_MASK1 3411


//----------------------------------------------------------------------------------------
/// Get the octane Video Post, it's assume you already checked Octane is the render engine.
/// @return										the Base object that represent an octane Video Post
//----------------------------------------------------------------------------------------
BaseVideoPost* OctaneRenderEngine::GetVideoPost(){
	BaseVideoPost* vp = rd->GetFirstVideoPost();
	while (vp->GetType() != ID_OCTANE_DRIVER)
	{
		vp->GetNext();
	}
	return vp;
}

//----------------------------------------------------------------------------------------
/// Remove all Octanes objects Id from the render data
//----------------------------------------------------------------------------------------
void OctaneRenderEngine::removeAllOctaneBufferId()
{
	for (Int32 i = 0; i < 24; i++)
	{
		vp->SetParameter(VP_PASSES_RL_MASK1 + i, GeData(Bool(false)), DESCFLAGS_SET_0);
	}
}

//----------------------------------------------------------------------------------------
/// Set all renders options in RenderData
///		Add post effect pass if not pressent
//----------------------------------------------------------------------------------------
void OctaneRenderEngine::SetRenderOption()
{
	if (options.UseLayerID)
		vp->SetParameter(ID_OCTANE_SET_PASSES_ENABLED, GeData(Bool(true)), DESCFLAGS_SET_0);

	if (options.UseObjectId)
		vp->SetParameter(ID_OCTANE_SET_PASSES_OBJID, GeData(Bool(true)), DESCFLAGS_SET_0);

	// Add multipass
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
/// @param[in] tag								the tag that hold the layer id
/// @param[out] buffer							List of all layer id
//----------------------------------------------------------------------------------------
void OctaneRenderEngine::GetObjectBufferId(BaseTag* tag, maxon::BaseArray<Int32>& buffer)
{
	if (!tag)
		return;

	GeData GeValue;
	tag->GetParameter(DescID(ID_OCTANE_OBJECTTAG_LAYERID), GeValue, DESCFLAGS_GET_0);
	const Int32 value = GeValue.GetInt32();

	// Check value is beetween 0 and 25
	if (value > 0 && value < 25)
		buffer.Append(GeValue.GetInt32());

	return;
}

//----------------------------------------------------------------------------------------
/// Get all objects id that are actually inside RenderData
/// @param[out] buffer							List of all layer id
//----------------------------------------------------------------------------------------
void OctaneRenderEngine::GetAllObjectsBufferInRenderData(maxon::BaseArray<Int32>& buffer)
{
	GeData GeValue;
	for (Int32 i = 0; i < 24; i++)
	{
		vp->GetParameter(VP_PASSES_RL_MASK1 + i, GeValue, DESCFLAGS_GET_0);

		// Only accept bool
		if (GeValue.GetType() != DA_LONG)
			continue;

		// Add the actual obj ID used if true
		if (GeValue.GetBool())
			buffer.Append(i+1);
	}
}


//----------------------------------------------------------------------------------------
/// Sreate Octane LayerID in order to create pass for each layer id
/// @param[out] newIDs							List of all layer id
//----------------------------------------------------------------------------------------
void OctaneRenderEngine::AddObjectBuffer(const maxon::BaseArray<Int32>& newIDs)
{
	// Check if we must add Layer Id
	if (!options.UseLayerID) return;

	maxon::BaseArray<Int32> finalIdToAdd;
	RemoveTwice(newIDs, finalIdToAdd);

	// Add layer ID
	for (Int32 i = 0; i < finalIdToAdd.GetCount(); i++)
	{
		vp->SetParameter(VP_PASSES_RL_MASK1 + finalIdToAdd[i] - 1, GeData(Bool(true)), DESCFLAGS_SET_0);
	}
}
