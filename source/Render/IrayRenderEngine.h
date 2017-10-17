#ifndef GRAPH_EASYOBJECTID_IRAY_RENDER_ENGINE_H__
#define GRAPH_EASYOBJECTID_IRAY_RENDER_ENGINE_H__

#include "c4d.h"
#include "DefaultRenderManager.h"

class IrayRenderEngine : public DefaultRenderManager
{
	private:
		maxon::BaseArray<String> idInScene;
		BaseVideoPost* IrayVideoPost;

		BaseVideoPost* GetIrayVideoPost();
		String GetNameObjectBuffer(const String original);
		void GetEmptyPassInRenderData(const Int32& count, maxon::BaseArray<Int32>& buffer);
		void GetBcIDWhereToInsertObjectBuffer(maxon::BaseArray<String>& newIds, maxon::BaseArray<Int32>& aviaibleIds);

	public:
		IrayRenderEngine(RenderData* rd_) : DefaultRenderManager(rd_)
		{
			IrayVideoPost = GetIrayVideoPost();
		};

		~IrayRenderEngine() {};

		virtual void SetRenderOption();
		virtual void GetObjectBufferId(BaseTag* tag, maxon::BaseArray<String>& buffer);

		virtual void GetAllObjectsBufferInRenderData(maxon::BaseArray<String>& buffer);
		void AddObjectBuffer(const maxon::BaseArray<String>& newIDs);


};

#endif // GRAPH_EASYOBJECTID_IRAY_RENDER_ENGINE_H__