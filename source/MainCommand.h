#ifndef GRAPH_EASYOBJECTID_MAIN_COMMAND_H__
#define GRAPH_EASYOBJECTID_MAIN_COMMAND_H__

#define ID_CPP_EASYOBJECTID 1039895

#include "Render\DefaultRenderManager.h"

#include "Options\OptionIO.h"
#include "Options\Struct.h"
#include "Options\Ui.h"

class MainCommand : public CommandData
{
	private:
		Ui* dlg;
		RENDER_ENGINE_ID render_engine_id;
		TAG_ID tag_id;

		OptionIO _optIO;

		DefaultRenderManager* renderManager;

		Bool SetRenderEngineId(BaseDocument* doc);
		void LunchUi();

		void IntObjectIDAction(BaseDocument* doc);
		void StringObjectIDAction(BaseDocument* doc);

	public:
		~MainCommand();

		virtual Bool Execute(BaseDocument* doc);
		virtual Bool ExecuteOptionID(BaseDocument* doc, Int32 plugid, Int32 subid);
		virtual Int32 GetState(BaseDocument* doc);
};

#endif // GRAPH_EASYOBJECTID_MAIN_COMMAND_H__