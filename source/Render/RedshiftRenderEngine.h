#ifndef GRAPH_EASYOBJECTID_REDSHIFT_RENDER_ENGINE_H__
#define GRAPH_EASYOBJECTID_REDSHIFT_RENDER_ENGINE_H__

#include "c4d.h"
#include "DefaultRenderManager.h"
#include "../Options/Struct.h"

class RedshiftRenderEngine : public DefaultRenderManager
{
	private:
		maxon::BaseArray<Int32> idInScene;
		BaseVideoPost* RedshiftVideoPost;
		RedshiftOptions options;

		BaseVideoPost* GetRedshiftVideoPost();

	public:
		RedshiftRenderEngine(RenderData* rd_, RedshiftOptions &opts) : DefaultRenderManager(rd_)
		{
			RedshiftVideoPost = GetRedshiftVideoPost();
			options = opts;
		};

		~RedshiftRenderEngine() {};

		virtual void SetRenderOption();
		virtual void GetObjectBufferId(BaseTag* tag, maxon::BaseArray<Int32>& buffer);

		virtual void GetAllObjectsBufferInRenderData(maxon::BaseArray<Int32>& buffer);
		void AddObjectBuffer(const maxon::BaseArray<Int32>& newIDs);
};

#endif // GRAPH_EASYOBJECTID_REDSHIFT_RENDER_ENGINE_H__