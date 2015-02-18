

/********************************************************************
	created:	2008/12/24
	created:	24:12:2008   0:34
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RenderGroup.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RenderGroup
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___RenderGroup___
#define ___RenderGroup___



#include "RenderOperation.h"
#include "IHardwareStreamBuffer.h"
#include "IVideoDevice.h"
#include "RenderMaterial.h"
#include "GPUShader.h"
#include "RenderManager.h"
#include "IRenderGroup.h"

namespace mray
{
	class BenchmarkItem;
namespace scene
{


class MRAY_DLL RenderGroup:public IRenderGroup
{
public:


private:

	enum ESortGroupType
	{
		ESGT_None,
		ESGT_BaseSortGroup,
		ESGT_Material,
		ESGT_Streams,
		ESGT_RenderOperation,
		ESGT_SortGroupCount
	};

	static const ESortGroupType s_SortGroupCreation[ESGT_SortGroupCount];


	typedef std::vector<RenderOperation> RenderOperationList;
	typedef std::vector<RenderOperation*> PRenderOperationList;

	class ISortGroup
	{
	public:
		virtual ~ISortGroup(){}
		virtual void addOperation(RenderOperation*op)=0;
		virtual void render(RenderGroup*grp)=0;
		virtual bool CanBeAdded(RenderOperation*op)=0;

		virtual ESortGroupType GetType()=0;

		virtual void Clear()=0;

		static ISortGroup* CreateSortGroup(ESortGroupType type);
	};

	typedef std::list<ISortGroup*> SortGroupList;

	class MRAY_DLL SRenderOpGroup:public ISortGroup
	{
	public:
		virtual ~SRenderOpGroup();

		PRenderOperationList m_renderOperations;

		void addOperation(RenderOperation*op);
		void render(RenderGroup*grp);
		bool CanBeAdded(RenderOperation*op){return true;}
		void Clear();

		ESortGroupType GetType()
		{
			return ESGT_RenderOperation;
		}
	};

	class MRAY_DLL RenderPassGroup:public ISortGroup
	{
	public:
		RenderPassGroup():isNew(true){}
		virtual ~RenderPassGroup();

		bool isNew;
		video::RenderPass* mtrl;
		//PRenderOperationList m_renderOperations;
		SortGroupList m_subGroup;

		void addOperation(RenderOperation*op);
		void render(RenderGroup*grp);
		bool CanBeAdded(RenderOperation*op);
		ESortGroupType GetType()
		{
			return ESGT_Material;
		}
		void Clear();
	};
	class MRAY_DLL SVBGroup:public ISortGroup
	{
	public:
		SVBGroup():isNew(true){}
		virtual ~SVBGroup();

		bool isNew;
		video::StreamsData* stream;

		//typedef std::map<video::RenderMaterial*,ISortGroup*> SubGroupMap;

		SortGroupList m_subGroup;
		 

		void addOperation(RenderOperation*op);
		void render(RenderGroup*grp);
		bool CanBeAdded(RenderOperation*op);
		ESortGroupType GetType()
		{
			return ESGT_Streams;
		}
		void Clear();
	};

	class MRAY_DLL SBaseSortGroup:public ISortGroup
	{
	public:
		virtual ~SBaseSortGroup();

		SortGroupList m_subGroup;

		void addOperation(RenderOperation*op);
		void render(RenderGroup*grp);
		bool CanBeAdded(RenderOperation*op);
		ESortGroupType GetType()
		{
			return ESGT_BaseSortGroup;
		}
		void Clear();
	};


	SBaseSortGroup m_sortGroup;

	int m_index;

//	vbGroup m_vbGroup;
	RenderOperationList m_unsortedOperations;
	PRenderOperationList m_sortedOperations;

	RenderOperationList m_customRenderOP;

	RenderManager* m_renderManager;

	ESortType m_sortType;


	BenchmarkItem* m_benchmarkItem;
	BenchmarkItem* m_sortBI;
	BenchmarkItem* m_renderBI;
public:
	RenderGroup(RenderManager*mgr,int idx);
	virtual ~RenderGroup();

	void addRenderOperation(const RenderOperation& op);

	RenderManager* getRenderManager(){return m_renderManager;}

	int getIndex(){return m_index;}

	void sort();

	void setSortType(ESortType s);
	ESortType getSortType();

	video::IVideoDevice*getDevice();

	void performRender(IRenderPass*pass);

	void clear();

	virtual BenchmarkItem* getBenchmarkItem(){
		return m_benchmarkItem;
	}
};

}
}


#endif //___RenderGroup___
