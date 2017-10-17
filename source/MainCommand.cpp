#include "c4d.h"
#include "main.h"
#include "MainCommand.h"
#include "Render\DefaultRenderManager.h"
#include "Render\C4DRenderEngine.h"
#include "Render\ArnoldRenderEngine.h"
#include "Render\IrayRenderEngine.h"
#include "Render\VrayRenderEngine.h"
#include "Render\OctaneRenderEngine.h"
#include "Render\RedshiftRenderEngine.h"

#include "Options\Ui.h"


#define ID_PRORENDER 1037639


MainCommand::~MainCommand(void){
	if (dlg)
		delete dlg;
}


//----------------------------------------------------------------------------------------
/// Set the current :member: "render_engine_id" according the current active render engine within the doc
/// @param[in] doc								The document where objs with object pass tag and renderData are stored.
///		NOTE : normally it's only called from Command:Execute so doc is always valid
/// @return										@trueIfOtherwiseFalse{successful}
//----------------------------------------------------------------------------------------
Bool MainCommand::SetRenderEngineId(BaseDocument* doc)
{
	if (!doc)
		return false;

	if (!_optIO.ReadOptionData())
		return false;

	RenderData* rd = doc->GetActiveRenderData();

	GeData data;
	rd->GetParameter(DescID(RDATA_RENDERENGINE), data, DESCFLAGS_GET_0);
	const Int32 active_render_id = data.GetInt32();

	switch (active_render_id)
	{
	case RDATA_RENDERENGINE_PHYSICAL: case RDATA_RENDERENGINE_PREVIEWHARDWARE: case RDATA_RENDERENGINE_PREVIEWSOFTWARE: case RDATA_RENDERENGINE_STANDARD: case ID_PRORENDER:
		{
			render_engine_id = C4D_RENDER;
			tag_id = C4D_TAG;
			renderManager = new C4DRenderEngine(doc->GetActiveRenderData());
			break;
		}
		case VRAY_RENDER:
		{
			render_engine_id = VRAY_RENDER;
			tag_id = VRAY_TAG;
			renderManager = new VrayRenderEngine(doc->GetActiveRenderData(), _optIO.GetVrayOptions());
			break;
		}
		case ARNOLD_RENDER:
		{
			render_engine_id = ARNOLD_RENDER;
			tag_id = ARNOLD_TAG;
			renderManager = new ArnoldRenderEngine(doc->GetActiveRenderData());
			break;
		}
		case OCTANE_RENDER:
		{
			render_engine_id = OCTANE_RENDER;
			tag_id = OCTANE_TAG;
			renderManager = new OctaneRenderEngine(doc->GetActiveRenderData(), _optIO.GetOctaneOptions());
			break;
		}
		case IRAY_RENDER:
		{
			render_engine_id = IRAY_RENDER;
			tag_id = IRAY_TAG;
			renderManager = new IrayRenderEngine(doc->GetActiveRenderData());
			break;
		}
		case REDSHIFT_RENDER:
		{
			render_engine_id = REDSHIFT_RENDER;
			tag_id = REDSHIFT_TAG;
			renderManager = new RedshiftRenderEngine(doc->GetActiveRenderData(), _optIO.GetRedshiftOptions());
			break;
		}
		default:
		{
			render_engine_id = UNKNOW_RENDER;
			tag_id = UNKNOW_TAG;
			break;
		}
	}
	if (render_engine_id == UNKNOW_RENDER)
		return false;

	return true;
}

//----------------------------------------------------------------------------------------
/// Lunch GeDialog for set options
//----------------------------------------------------------------------------------------
void MainCommand::LunchUi()
{
	_optIO.ReadOptionData();
	if (!dlg)
		dlg = new Ui(_optIO);

	// open the dialog window if it is not already open
	if (dlg->IsOpen() == false)
		dlg->Open(DLG_TYPE_ASYNC, ID_CPP_EASYOBJECTID, -1, -1, 400, 400);
}
//----------------------------------------------------------------------------------------
/// Main action when object id are stored into integer. It handle the search and pass creation in RenderData / object
/// @param[in] doc								The document where objs with object pass tag and renderData are stored.
///		NOTE : normally it's only called from Command:Execute so doc is always valid
//----------------------------------------------------------------------------------------
void MainCommand::IntObjectIDAction(BaseDocument* doc)
{
	maxon::BaseArray<Int32> TagInScene;
	renderManager->SearchObjectIDInScene(doc->GetFirstObject(), tag_id, TagInScene);
	if (!TagInScene.GetCount())
	{
		MessageDialog(String("No object ID found"));
	}
	else
	{
		renderManager->SetRenderOption();
		renderManager->AddObjectBuffer(TagInScene);

		MessageDialog(String("Done,") + String::IntToString(TagInScene.GetCount()) + String(" Object(s) ID Added"));
	}
	TagInScene.Flush();
}

//----------------------------------------------------------------------------------------
/// Main action when object id are stored into string. It handle the search and pass creation in RenderData / object
/// @param[in] doc								The document where objs with object pass tag and renderData are stored.
///		NOTE : normally it's only called from Command:Execute so doc is always valid
//----------------------------------------------------------------------------------------
void MainCommand::StringObjectIDAction(BaseDocument* doc)
{
	maxon::BaseArray<String> TagInScene;
	renderManager->SearchObjectIDInScene(doc->GetFirstObject(), tag_id, TagInScene);
	if (!TagInScene.GetCount())
	{
		MessageDialog(String("No object ID found"));
	}
	else
	{
		renderManager->SetRenderOption();
		renderManager->AddObjectBuffer(TagInScene);

		MessageDialog(String("Done,") + String::IntToString(TagInScene.GetCount()) + String(" Object(s) ID Added"));
	}
	TagInScene.Flush();
}

//----------------------------------------------------------------------------------------
/// Set the render engine according in which scene user call it.
/// @param[in] doc								The document where objs with object pass tag and renderData are stored.
//----------------------------------------------------------------------------------------
Bool MainCommand::Execute(BaseDocument* doc)
{
	// define our render engine, if there is no that mean we don't support this render engine.
	if (!SetRenderEngineId(doc) || !renderManager)
		return true;

	// remove default c4d object buffer
	renderManager->RemoveDefaultObjectBuffer();
		
	// define if it's tag or string
	switch (render_engine_id)
	{
		case UNKNOW_RENDER:
			break;
		case C4D_RENDER:
			IntObjectIDAction(doc);
			break;
		case VRAY_RENDER:
			IntObjectIDAction(doc);
			break;
		case ARNOLD_RENDER:
			StringObjectIDAction(doc);
			break;
		case OCTANE_RENDER:
			IntObjectIDAction(doc);
			break;
		case IRAY_RENDER:
			StringObjectIDAction(doc);
			break;
		case REDSHIFT_RENDER:
			IntObjectIDAction(doc);
			break;
		default:
			break;
	}

	EventAdd();

	if (renderManager)
		delete renderManager;

	return true;
}

Bool MainCommand::ExecuteOptionID(BaseDocument* doc, Int32 plugid, Int32 subid)
{
	LunchUi();
	return true;
}

Int32 MainCommand::GetState(BaseDocument* doc)
{
	return CMD_ENABLED;
}

Bool RegisterGraphEasyObjectID()
{
	MainCommand* g_cmd = NewObjClear(MainCommand);
	return RegisterCommandPlugin(ID_CPP_EASYOBJECTID, String("Easy Object ID v2"), PLUGINFLAG_COMMAND_OPTION_DIALOG , nullptr, String("Easy Object ID v2"), g_cmd);
}