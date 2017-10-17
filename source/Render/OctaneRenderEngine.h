#ifndef GRAPH_EASYOBJECTID_OCTANE_RENDER_ENGINE_H__
#define GRAPH_EASYOBJECTID_OCTANE_RENDER_ENGINE_H__

#include "c4d.h"
#include "DefaultRenderManager.h"
#include "../Options/Struct.h"

class OctaneRenderEngine : public DefaultRenderManager
{
	private:
		maxon::BaseArray<Int32> idInScene;
		BaseVideoPost* vp;
		OctaneOptions options;

		BaseVideoPost* GetVideoPost();

		void removeAllOctaneBufferId();

	public:
		OctaneRenderEngine(RenderData* rd_, OctaneOptions &opt) : DefaultRenderManager(rd_)
		{
			options = opt;
			vp = GetVideoPost();
			if (vp)
				removeAllOctaneBufferId();
		};


		~OctaneRenderEngine() {
		};

		virtual void SetRenderOption();
		virtual void GetObjectBufferId(BaseTag* tag, maxon::BaseArray<Int32>& buffer);

		virtual void GetAllObjectsBufferInRenderData(maxon::BaseArray<Int32>& buffer);
		void AddObjectBuffer(const maxon::BaseArray<Int32>& newIDs);


};

#endif // GRAPH_EASYOBJECTID_OCTANE_RENDER_ENGINE_H__