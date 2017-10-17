#ifndef GRAPH_EASYOBJECTID_DEFAULT_RENDER_MANAGER_H__
#define GRAPH_EASYOBJECTID_DEFAULT_RENDER_MANAGER_H__

#include "c4d.h"


class DefaultRenderManager
{
	protected:
		RenderData* rd;

		DefaultRenderManager(RenderData* rd_) : rd(rd_)  {};


		void RemoveTwice(const maxon::BaseArray<Int32>& newIDs, maxon::BaseArray<Int32>& finalIDs);
		void RemoveTwice(const maxon::BaseArray<String>& newIDs, maxon::BaseArray<String>& finalIDs);

		Bool IsInBaseArray(Int32 IntToSearch, maxon::BaseArray<Int32>& ArrayToSearch);
		Bool IsInBaseArray(String StringToSearch, maxon::BaseArray<String>& ArrayToSearch);

		// !! PURE VIRTUAL FUNCTION !!
		virtual void GetObjectBufferId(BaseTag* tag, maxon::BaseArray<Int32>& buffer) {};
		virtual void GetObjectBufferId(BaseTag* tag, maxon::BaseArray<String>& buffer) {};
		virtual void GetAllObjectsBufferInRenderData(maxon::BaseArray<Int32>& buffer) {};
		virtual void GetAllObjectsBufferInRenderData(maxon::BaseArray<String>& buffer) {};

	public:
		virtual ~DefaultRenderManager() {};
		virtual void RemoveDefaultObjectBuffer();
		virtual void SetRenderOption() = 0;

		virtual void AddObjectBuffer(const maxon::BaseArray<Int32>& newIDs) {};
		virtual void AddObjectBuffer(const maxon::BaseArray<String>& newIDs) {};
		virtual void SearchObjectIDInScene(BaseObject* obj, Int32 tagId, maxon::BaseArray<Int32>& TagInScene);
		virtual void SearchObjectIDInScene(BaseObject* obj, Int32 tagId, maxon::BaseArray<String>& TagInScene);

};


#endif // GRAPH_EASYOBJECTID_DEFAULT_RENDER_MANAGER_H__