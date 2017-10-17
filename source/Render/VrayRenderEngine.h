#ifndef GRAPH_EASYOBJECTID_VRAY_RENDER_ENGINE_H__
#define GRAPH_EASYOBJECTID_VRAY_RENDER_ENGINE_H__

#include "c4d.h"
#include "DefaultRenderManager.h"

#include "../Options/Struct.h"

class VrayRenderEngine : public DefaultRenderManager
{
	private:
		maxon::BaseArray<Int32> idInScene;
		BaseVideoPost* vp;
		VrayOptions options;
		BaseObject* masterNode;

		BaseVideoPost* GetVideoPost();
		BaseObject* GetMasterNode();

		void removeAllVrayBufferId();

	public:
		VrayRenderEngine(RenderData* rd_, VrayOptions &opt) : DefaultRenderManager(rd_)
		{
			options = opt;
			vp = GetVideoPost();
			masterNode = GetMasterNode();
			if (masterNode)
				removeAllVrayBufferId();
		};


		~VrayRenderEngine() {};

		virtual void SetRenderOption();
		virtual void GetObjectBufferId(BaseTag* tag, maxon::BaseArray<Int32>& buffer);

		virtual void GetAllObjectsBufferInRenderData(maxon::BaseArray<Int32>& buffer);
		void AddObjectBuffer(const maxon::BaseArray<Int32>& newIDs);


};

#endif // GRAPH_EASYOBJECTID_VRAY_RENDER_ENGINE_H__