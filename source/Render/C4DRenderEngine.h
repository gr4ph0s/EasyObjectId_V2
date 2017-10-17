#ifndef GRAPH_EASYOBJECTID_C4D_RENDER_ENGINE_H__
#define GRAPH_EASYOBJECTID_C4D_RENDER_ENGINE_H__

#include "c4d.h"
#include "DefaultRenderManager.h"

class C4DRenderEngine : public DefaultRenderManager
{
	private:
		maxon::BaseArray<Int32> idInScene;

	public:
		C4DRenderEngine(RenderData* rd_) : DefaultRenderManager(rd_) {};
		~C4DRenderEngine() {};

		virtual void SetRenderOption();
		virtual void GetObjectBufferId(BaseTag* tag, maxon::BaseArray<Int32>& buffer);

		virtual void GetAllObjectsBufferInRenderData(maxon::BaseArray<Int32>& buffer);
		void AddObjectBuffer(const maxon::BaseArray<Int32>& newIDs);


};

#endif // GRAPH_EASYOBJECTID_C4D_RENDER_ENGINE_H__