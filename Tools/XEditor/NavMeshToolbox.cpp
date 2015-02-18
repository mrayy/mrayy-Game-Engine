// NavMeshToolbox.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "NavMeshToolbox.h"
#include "CEngineWindow.h"
#include "EditorEventManager.h"
#include "EditorEventMessages.h"

#include "EditorGlobals.h"
#include "AISystem.h"

// CNavMeshToolbox dialog

IMPLEMENT_DYNAMIC(CNavMeshToolbox, CDialog)

CNavMeshToolbox::CNavMeshToolbox(CWnd* pParent /*=NULL*/)
	: CDialog(CNavMeshToolbox::IDD, pParent)
	, MaxStepSize(0)
	, CellSize(0)
	, MaxSlope(0)
{
	m_selectedMesh=-1;
	m_renderDev=0;

	//m_navMesh=new AI::NavMesh();

	EditorEventManager::getInstance().AddHandler(EEE_OnModelSelected,newClassDelegate1<CNavMeshToolbox,void,mray::PVOID>(mT(""),this,&CNavMeshToolbox::OnModelSelected));
	EditorEventManager::getInstance().AddHandler(EEE_OnEngineInitiated,newClassDelegate1<CNavMeshToolbox,void,mray::PVOID>(mT(""),this,&CNavMeshToolbox::OnEngineInitiated));
	EditorGlobals::RegisterToolBox(this);

	if(EditorGlobals::IsEngineInitiated)
		OnEngineInitiated(0);
}

CNavMeshToolbox::~CNavMeshToolbox()
{
	//delete m_renderDev;
}

void CNavMeshToolbox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUBMESH_LIST, SubMeshList);
	DDX_Text(pDX, IDC_NAVMESH_MAXSTEP, MaxStepSize);
	DDX_Text(pDX, IDC_NAVMESH_GCELLSZ, CellSize);
	DDX_Text(pDX, IDC_NAVMESH_MAXSLOPE, MaxSlope);
}


BEGIN_MESSAGE_MAP(CNavMeshToolbox, CDialog)
	ON_BN_CLICKED(IDC_GENERATE_NAVMESH_BTN, &CNavMeshToolbox::OnBnClickedGenerateNavmeshBtn)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_IMPORT_NAVMESH_BTN, &CNavMeshToolbox::OnBnClickedImportNavmeshBtn)
	ON_BN_CLICKED(IDC_EXPORT_NAVMESH_BTN, &CNavMeshToolbox::OnBnClickedExportNavmeshBtn)
	ON_BN_CLICKED(IDC_IMPORT_NAVMESH_ATTRS_BTN, &CNavMeshToolbox::OnBnClickedImportNavmeshAttrsBtn)
	ON_BN_CLICKED(IDC_Load_Attributes_BTN, &CNavMeshToolbox::OnBnClickedLoadAttributesBtn)
END_MESSAGE_MAP()

void CNavMeshToolbox::OnSetNavMesh()
{
}

void CNavMeshToolbox::SetEditor(mray::CEngineWindow*e)
{
	m_editor=e;
}

void CNavMeshToolbox::OnExportLevelXML(xml::XMLElement*e)
{
}
bool CNavMeshToolbox::OnImportLevelXML(xml::XMLElement*e)
{
	return false;
}

void CNavMeshToolbox::ShowToolBox(bool s)
{
	this->ShowWindow(s);
}
bool CNavMeshToolbox::IsToolBoxVisible()
{
	return this->IsWindowVisible();
}

void CNavMeshToolbox::OnEngineInitiated(const mray::PVOID&)
{
	m_navMesh=m_editor->GetLevel()->GetAISystem()->GetNavigationWorld();
	//m_renderDev=new AI::AIRenderDevice(Engine::getInstance().getDevice(),0);
	//AI::AISystem::getInstance().SetNavigationWorld(m_navMesh);
}
void CNavMeshToolbox::OnModelSelected(const mray::PVOID&)
{

	scene::ISceneNode*node=m_editor->GetSelectedNode();
	SubMeshList.ResetContent();
	if(node)
	{
		scene::SMesh*mesh= node->getMesh();
		if(mesh)
		{
			for(int i=0;i<mesh->getBuffersCount();++i)
			{
				CString val=mesh->getBuffer(i)->getMeshName().c_str();
				SubMeshList.AddString(val);
			}
		}
	}

}

void CNavMeshToolbox::OnRender()
{
	/*if(m_renderDev)
		m_navMesh->Visualize(m_renderDev);*/
}


//////////////////////////////////////////////////////////////////////////
// CNavMeshToolbox message handlers

void CNavMeshToolbox::OnBnClickedGenerateNavmeshBtn()
{
	// TODO: Add your control notification handler code here
	int i=SubMeshList.GetCurSel();

	scene::ISceneNode*node=m_editor->GetSelectedNode();
	if(i==-1 || !node)
		return;
	scene::SMesh*mesh= node->getMesh();
	scene::IMeshBuffer* buffer= mesh->getBuffer(i);
	if(!buffer)
		return;

	UpdateData(true);

	if(CellSize==0)
		CellSize=1;

	m_navMesh->SetGridCellSize(CellSize);
	m_navMesh->SetMaxStepSize(MaxStepSize);
	m_navMesh->SetMaxSlope(MaxSlope);

	GCPtr<video::IHardwareStreamBuffer> vStream=buffer->getStream(0,video::EMST_Position);
	GCPtr<video::IHardwareIndexBuffer> iBuffer=buffer->getIndexData()->indexBuffer;
	math::vector3d*vptr=(math::vector3d*)vStream->lock(0,vStream->getSizeInBytes(),video::IHardwareBuffer::ELO_ReadOnly);
	void*iptr=iBuffer->lock(0,iBuffer->getSizeInBytes(),video::IHardwareBuffer::ELO_ReadOnly);

	int*ptr=0;
	if(m_navAttrs.size()!=0)
		ptr=m_navAttrs.pointer();

	m_navMesh->Generate(vptr,vStream->length(),iptr,iBuffer->getIndexCount(),ptr,
		iBuffer->getIndexType()==video::IHardwareIndexBuffer::EIT_16Bit,&node->getAbsoluteTransformation());

	vStream->unlock();
	iBuffer->unlock();
}

void CNavMeshToolbox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
}

void CNavMeshToolbox::OnBnClickedImportNavmeshBtn()
{
	// TODO: Add your control notification handler code here
	CFileDialog fd(true,0,0,OFN_OVERWRITEPROMPT,mT("Navigation Mesh File *.nav|*.nav||"));
	if(fd.DoModal()==IDOK)
	{
		m_navMesh->Deserialize(fd.GetFileName().GetString());
	}
}

void CNavMeshToolbox::OnBnClickedExportNavmeshBtn()
{
	// TODO: Add your control notification handler code here
	CFileDialog fd(false,0,0,OFN_OVERWRITEPROMPT,mT("Navigation Mesh File *.nav|*.nav||"));
	if(fd.DoModal()==IDOK)
	{
		GCPtr<OS::IStream> stream=gFileSystem.openFile(fd.GetPathName().GetString(),OS::BIN_WRITE);
		m_navMesh->Serialize(stream);
		stream->close();
	}
}

void CNavMeshToolbox::OnBnClickedImportNavmeshAttrsBtn()
{
	// TODO: Add your control notification handler code here
	
	CFileDialog fd(true,0,0,OFN_OVERWRITEPROMPT,mT("Navigation Mesh Description File *.xml|*.xml||"));
	if(fd.DoModal()==IDOK)
	{
		m_navMesh->LoadAttributeDescFile(fd.GetPathName().GetString());
	}
}


void CNavMeshToolbox::OnBnClickedLoadAttributesBtn()
{
	// TODO: Add your control notification handler code here
	CFileDialog fd(true,0,0,OFN_OVERWRITEPROMPT,mT("Attribute File *.attr|*.attr||"));
	if(fd.DoModal()==IDOK)
	{
		OS::IStream* stream=gFileSystem.openFile(fd.GetPathName().GetString(),OS::BIN_READ);
		int meshCount;
		stream->read(&meshCount,sizeof(meshCount));
		for(int i=0;i<meshCount;++i)
		{
			int triCount=0;
			stream->read(&triCount,sizeof(triCount));
			m_navAttrs.resize(triCount);
			stream->read(m_navAttrs.pointer(),triCount*sizeof(int));
		}
		stream->close();
	}
}
