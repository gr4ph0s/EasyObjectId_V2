#ifndef GRAPH_EASYOBJECTID_ARNOLD_RENDER_ENGINE_H__
#define GRAPH_EASYOBJECTID_ARNOLD_RENDER_ENGINE_H__

#include "c4d.h"
#include "DefaultRenderManager.h"

class ArnoldRenderEngine : public DefaultRenderManager
{
	private:
		maxon::BaseArray<String> idInScene;
		BaseObject* driver;

		BaseObject* GetDriver();

	public:
		ArnoldRenderEngine(RenderData* rd_) : DefaultRenderManager(rd_) 
		{
			driver = GetDriver();
		};


		~ArnoldRenderEngine() {};

		virtual void SetRenderOption();
		virtual void GetObjectBufferId(BaseTag* tag, maxon::BaseArray<String>& buffer);

		virtual void GetAllObjectsBufferInRenderData(maxon::BaseArray<String>& buffer);
		void AddObjectBuffer(const maxon::BaseArray<String>& newIDs);


};

#endif // GRAPH_EASYOBJECTID_ARNOLD_RENDER_ENGINE_H__