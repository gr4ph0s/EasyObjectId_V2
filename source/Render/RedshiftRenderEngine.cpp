#include "DefaultRenderManager.h"
#include "RedshiftRenderEngine.h"

#define ID_REDSHIFT_VIDEOPOST 1036219

#define REDSHIFT_OBJECT_OBJECTID_OVERRIDE 1999
#define REDSHIFT_OBJECT_OBJECTID_ID 1000

#define REDSHIFT_RENDERER_AOV_GLOBAL_MODE 3001
#define REDSHIFT_RENDERER_AOV_COUNT 3101

#define REDSHIFT_RSAOV_DATATYPE 1036235

#define REDSHIFT_FIRST_AOV 3102
#define REDSHIFT_MAX_AOV 64

#define REDSHIFT_AOV_TYPE 1003
#define REDSHIFT_OBJ_ID 4
#define REDSHIFT_PUZZLE_MATT_ID 2

#define REDSHIFT_AOV_PUZZLE_MATTE_MODE 1014
#define REDSHIFT_MAT_ID_MODE 0
#define REDSHIFT_OBJ_ID_MODE 1

#define REDSHIFT_AOV_PUZZLE_MATTE_RED_ID 1015
#define REDSHIFT_AOV_PUZZLE_MATTE_GREEN_ID 1016
#define REDSHIFT_AOV_PUZZLE_MATTE_BLUE_ID 1017

#define REDSHIFT_AOV_MULTIPASS_ENABLED 5000
#define REDSHIFT_AOV_NAME 1001

#define REDSHIFT_AOV_MULTIPASS_BIT_DEPTH 1002
#define REDSHIFT_AOV_FILE_ENABLED 5001
#define REDSHIFT_AOV_FILE_PATH 6000
#define REDSHIFT_AOV_FILE_DATA_TYPE 6001
#define REDSHIFT_AOV_FILE_FORMAT 6002
#define REDSHIFT_AOV_FILE_BIT_DEPTH 6003
#define REDSHIFT_AOV_FILE_COMPRESSION 6004
#define REDSHIFT_AOV_FILE_STORAGE 6005
#define REDSHIFT_AOV_FILE_JPEG_QUALITY 6006
#define REDSHIFT_AOV_FILE_EXR_DWA_COMPRESSION 6007

//----------------------------------------------------------------------------------------
/// Get the Redshift Video Post, it's assume you already checked Redshift is the render engine.
/// @return										the Redshift Video Post
//----------------------------------------------------------------------------------------
BaseVideoPost* RedshiftRenderEngine::GetRedshiftVideoPost()
{
	BaseVideoPost* vp = rd->GetFirstVideoPost();
	while (vp->GetType() != ID_REDSHIFT_VIDEOPOST)
	{
		vp->GetNext();
	}
	return vp;
}

//----------------------------------------------------------------------------------------
/// Set all renders options in RenderData
///		Add post effect pass if not pressent
//----------------------------------------------------------------------------------------
void RedshiftRenderEngine::SetRenderOption()
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

	// Enable AOV
	RedshiftVideoPost->SetParameter(DescID(REDSHIFT_RENDERER_AOV_GLOBAL_MODE), GeData(true), DESCFLAGS_SET_0);
}

//----------------------------------------------------------------------------------------
/// Get all objects id stored in one tag
/// @param[in] tag								the tag that hold the object id
/// @param[out] buffer							List of all object id
//----------------------------------------------------------------------------------------
void RedshiftRenderEngine::GetObjectBufferId(BaseTag* tag, maxon::BaseArray<Int32>& buffer)
{
	if (!tag)
		return;

	// Check if obj_id is enable
	GeData GeValue;
	tag->GetParameter(DescID(REDSHIFT_OBJECT_OBJECTID_OVERRIDE), GeValue, DESCFLAGS_GET_0);
	if (!GeValue.GetBool())
		return;

	// Get Object ID
	tag->GetParameter(DescID(REDSHIFT_OBJECT_OBJECTID_ID), GeValue, DESCFLAGS_GET_0);
	if (GeValue.GetInt32())
		buffer.Append(GeValue.GetInt32());

	return;
}

//----------------------------------------------------------------------------------------
/// Get all objects id that are actually inside the RenderData
/// @param[out] buffer							List of all object id
//----------------------------------------------------------------------------------------
void RedshiftRenderEngine::GetAllObjectsBufferInRenderData(maxon::BaseArray<Int32>& buffer)
{
	// Check AOV are enable
	GeData AOVenable;
	RedshiftVideoPost->GetParameter(DescID(REDSHIFT_RENDERER_AOV_GLOBAL_MODE), AOVenable, DESCFLAGS_GET_0);
	if (AOVenable.GetInt32() < 1)
		return;

	// Check AOV count
	GeData AOVcount;
	RedshiftVideoPost->GetParameter(DescID(REDSHIFT_RENDERER_AOV_COUNT), AOVcount, DESCFLAGS_GET_0);

	// Get pass ID
	GeData bcAOV;
	GeData AOVtype;
	GeData AOVmode;
	GeData AOVred;
	GeData AOVgreen;
	GeData AOVblue;
	for (Int32 i = 0; i < AOVcount.GetInt32(); i++)
	{
		DescLevel rsaov = DescLevel(REDSHIFT_FIRST_AOV + i, REDSHIFT_RSAOV_DATATYPE, 0);

		// Check the type of the pass (if it's a puzzle mat)
		RedshiftVideoPost->GetParameter(DescID(rsaov, DescLevel(REDSHIFT_AOV_TYPE)), AOVtype, DESCFLAGS_GET_0);
		if (AOVtype.GetInt32() != REDSHIFT_PUZZLE_MATT_ID)
			continue;

		// Check the mode of puzzle mat (if it's a obj_id)
		RedshiftVideoPost->GetParameter(DescID(rsaov, DescLevel(REDSHIFT_AOV_PUZZLE_MATTE_MODE)), AOVmode, DESCFLAGS_GET_0);
		if (AOVmode.GetInt32() != REDSHIFT_OBJ_ID_MODE)
			continue;

		// Get object id
		RedshiftVideoPost->GetParameter(DescID(rsaov, DescLevel(REDSHIFT_AOV_PUZZLE_MATTE_RED_ID)), AOVred, DESCFLAGS_GET_0);
		RedshiftVideoPost->GetParameter(DescID(rsaov, DescLevel(REDSHIFT_AOV_PUZZLE_MATTE_GREEN_ID)), AOVgreen, DESCFLAGS_GET_0);
		RedshiftVideoPost->GetParameter(DescID(rsaov, DescLevel(REDSHIFT_AOV_PUZZLE_MATTE_BLUE_ID)), AOVblue, DESCFLAGS_GET_0);

		// Only add if it's not 0
		if (AOVred.GetInt32())
			buffer.Append(AOVred.GetInt32());

		if (AOVgreen.GetInt32())
			buffer.Append(AOVgreen.GetInt32());

		if (AOVblue.GetInt32())
			buffer.Append(AOVblue.GetInt32());
	}
}

//----------------------------------------------------------------------------------------
/// Add all our objects id to RenderData (is not garanted that all ids will be included since Redshift limite to 64 pass)
/// @param[out] newIDs							List of all object id
//----------------------------------------------------------------------------------------
void RedshiftRenderEngine::AddObjectBuffer(const maxon::BaseArray<Int32>& newIDs)
{
	// Remove twice indice in our list of newIDs
	maxon::BaseArray<Int32> finalIdToAdd;
	RemoveTwice(newIDs, finalIdToAdd);

	// Calculate how many time we should run
	Int32 countToAdd = finalIdToAdd.GetCount() / 3;
	if (finalIdToAdd.GetCount() % 3)
		countToAdd++;

	GeData AOVcount;
	RedshiftVideoPost->GetParameter(DescID(REDSHIFT_RENDERER_AOV_COUNT), AOVcount, DESCFLAGS_GET_0);
	Int32 aviableCountID = Int32(REDSHIFT_MAX_AOV) - AOVcount.GetInt32();

	// Then Add data
	GeData bcAOV;
	for (Int32 i = 0; i < Min(aviableCountID, countToAdd); i++)
	{
		// Add new AOV
		Int32 newAOVid = AOVcount.GetInt32() + i + 1;
		RedshiftVideoPost->SetParameter(DescID(REDSHIFT_RENDERER_AOV_COUNT), GeData(newAOVid), DESCFLAGS_SET_0);
		EventAdd();

		DescLevel rsaov = DescLevel(REDSHIFT_FIRST_AOV + newAOVid - 1, REDSHIFT_RSAOV_DATATYPE, 0);
		// Set to puzzle mat
		RedshiftVideoPost->SetParameter(
			DescID(rsaov,DescLevel(REDSHIFT_AOV_TYPE)),
			GeData(Int32(REDSHIFT_PUZZLE_MATT_ID)),
			DESCFLAGS_SET_0);
		

		// Set to object id
		RedshiftVideoPost->SetParameter(
			DescID(rsaov, DescLevel(REDSHIFT_AOV_PUZZLE_MATTE_MODE)),
			GeData(Int32(REDSHIFT_OBJ_ID_MODE)),
			DESCFLAGS_SET_0);

		// Get object id
		Int32 rId = i * 3;
		Int32 gId = i * 3 + 1;
		Int32 bId = i * 3 + 2;

		// Add data
		// Red
		if (rId < finalIdToAdd.GetCount())
		{
			RedshiftVideoPost->SetParameter(
				DescID(rsaov, DescLevel(REDSHIFT_AOV_PUZZLE_MATTE_RED_ID)),
				GeData(Int32(finalIdToAdd[rId])),
				DESCFLAGS_SET_0);
		}
		// Green
		if (gId < finalIdToAdd.GetCount())
		{
			RedshiftVideoPost->SetParameter(
				DescID(rsaov, DescLevel(REDSHIFT_AOV_PUZZLE_MATTE_GREEN_ID)),
				GeData(Int32(finalIdToAdd[gId])),
				DESCFLAGS_SET_0);
		}
		// Blue
		if (bId < finalIdToAdd.GetCount())
		{
			RedshiftVideoPost->SetParameter(
				DescID(rsaov, DescLevel(REDSHIFT_AOV_PUZZLE_MATTE_BLUE_ID)),
				GeData(Int32(finalIdToAdd[bId])),
				DESCFLAGS_SET_0);
		}

		// Set the name
		RedshiftVideoPost->SetParameter(
			DescID(rsaov, DescLevel(REDSHIFT_AOV_NAME)),
			GeData(String::IntToString(i)),
			DESCFLAGS_SET_0);

		// Set output multipass
		if (options.UseMpEnable)
		{
			RedshiftVideoPost->SetParameter(
				DescID(rsaov, DescLevel(REDSHIFT_AOV_MULTIPASS_ENABLED)),
				GeData(Bool(true)),
				DESCFLAGS_SET_0);
		}
		else
		{
			RedshiftVideoPost->SetParameter(
				DescID(rsaov, DescLevel(REDSHIFT_AOV_MULTIPASS_ENABLED)),
				GeData(Bool(options.MpEnable)),
				DESCFLAGS_SET_0);
		}

		// Set output multipass bits
		if (options.UseMpBitPersChannels)
			RedshiftVideoPost->SetParameter(
				DescID(rsaov, DescLevel(REDSHIFT_AOV_MULTIPASS_BIT_DEPTH)),
				GeData(Int32(options.MpBitPersChannels)),
				DESCFLAGS_SET_0);

		// Set Enable File Outpute multipass bits
		if (options.UseFoEnable)
			RedshiftVideoPost->SetParameter(
			DescID(rsaov, DescLevel(REDSHIFT_AOV_FILE_ENABLED)),
			GeData(Bool(options.FoEnable)),
			DESCFLAGS_SET_0);

		// Set Path
		if (options.UseFoPath)
			RedshiftVideoPost->SetParameter(
			DescID(rsaov, DescLevel(REDSHIFT_AOV_FILE_PATH)),
			GeData(String(options.FoPath)),
			DESCFLAGS_SET_0);

		// Set Data Type
		if (options.UseFoDataType)
			RedshiftVideoPost->SetParameter(
			DescID(rsaov, DescLevel(REDSHIFT_AOV_FILE_DATA_TYPE)),
			GeData(Int32(options.FoDataType)),
			DESCFLAGS_SET_0);

		// Set Format
		if (options.UseFoFormat)
			RedshiftVideoPost->SetParameter(
			DescID(rsaov, DescLevel(REDSHIFT_AOV_FILE_FORMAT)),
			GeData(Int32(options.FoFormat)),
			DESCFLAGS_SET_0);

		// Bits Per Channel
		if (options.UseFoBitPersChannels)
			RedshiftVideoPost->SetParameter(
			DescID(rsaov, DescLevel(REDSHIFT_AOV_FILE_BIT_DEPTH)),
			GeData(Int32(options.FoBitPersChannels)),
			DESCFLAGS_SET_0);

		// Compression
		if (options.UseFoCompression)
			RedshiftVideoPost->SetParameter(
			DescID(rsaov, DescLevel(REDSHIFT_AOV_FILE_COMPRESSION)),
			GeData(Int32(options.FoCompression)),
			DESCFLAGS_SET_0);

		// DWA Compression
		if (options.UseFoDwaCompression)
			RedshiftVideoPost->SetParameter(
			DescID(rsaov, DescLevel(REDSHIFT_AOV_FILE_EXR_DWA_COMPRESSION)),
			GeData(Float(options.FoDwaCompression)),
			DESCFLAGS_SET_0);

		// JPEG Compression
		if (options.UseFoJpegQuality)
			RedshiftVideoPost->SetParameter(
			DescID(rsaov, DescLevel(REDSHIFT_AOV_FILE_JPEG_QUALITY)),
			GeData(Int32(options.FoJpegQuality)),
			DESCFLAGS_SET_0);

		// Storage
		if (options.UseFoStorage)
			RedshiftVideoPost->SetParameter(
			DescID(rsaov, DescLevel(REDSHIFT_AOV_FILE_STORAGE)),
			GeData(Int32(options.FoStorage)),
			DESCFLAGS_SET_0);
	}

	finalIdToAdd.Flush();
}
