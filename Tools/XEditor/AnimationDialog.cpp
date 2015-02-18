// AnimationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AnimationDialog.h"
#include "CEngineWindow.h"
#include "AnimationController.h"

#include "EditorEventMessages.h"

// CAnimationDialog dialog

IMPLEMENT_DYNAMIC(CAnimationDialog, CDialog)

CAnimationDialog::CAnimationDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimationDialog::IDD, pParent)
{
	m_editor=0;
	
	EditorEventManager::getInstance().AddHandler(EEE_OnModelSelected,newClassDelegate1<CAnimationDialog,void,mray::PVOID>(mT(""),this,&CAnimationDialog::OnModelSelected));
}

CAnimationDialog::~CAnimationDialog()
{
}
void CAnimationDialog::SetEditor(mray::CEngineWindow*e)
{
	m_editor=e;
}

void CAnimationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRACKS_LIST, TracksList);
}


BEGIN_MESSAGE_MAP(CAnimationDialog, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_LOADANIM_BTN, &CAnimationDialog::OnBnClickedLoadanimBtn)
	ON_BN_CLICKED(IDC_PLAYANIM_BTN, &CAnimationDialog::OnBnClickedPlayanimBtn)
	ON_BN_CLICKED(IDC_STOPANIM_BTN, &CAnimationDialog::OnBnClickedStopanimBtn)
END_MESSAGE_MAP()


void CAnimationDialog::ShowToolBox(bool s)
{
	this->ShowWindow(s);
}
bool CAnimationDialog::IsToolBoxVisible()
{
	return this->IsWindowVisible();
}

void CAnimationDialog::OnModelSelected(const mray::PVOID&)
{
	RefereshList();
}
void CAnimationDialog::OnShowWindow(BOOL show,UINT nStatus)
{
	CDialog::OnShowWindow(show,nStatus);
	RefereshList();
}

void CAnimationDialog::RefereshList()
{
	TracksList.ResetContent();
	scene::ISceneNode*node=m_editor->GetSelectedNode();
	if(!node)
		return;
	scene::SMesh*mesh= node->getMesh();
	if(!mesh)
		return;
	if(mesh->getSkeleton()){
		core::IteratorPair<animation::IAnimationMixer::ATGroupList> p=mesh->getSkeleton()->getAnimationMixer()->getTrackGroups();
		for(;!p.done();++p)
		{
			TracksList.AddString((*p).second->getName().c_str());
		}
	}
	else{
		for(int i=0;i<mesh->getBuffersCount();++i)
		{
			scene::MeshBufferData* bd=mesh->getBufferData(i);
			core::IteratorPair<animation::IAnimationMixer::ATGroupList> p=bd->getAnimationMixer()->getTrackGroups();
			for(;!p.done();++p)
			{
				TracksList.AddString((*p).second->getName().c_str());
			}
		}
	}
}



void CAnimationDialog::AddAnimationTrack(GCPtr<animation::AnimationTrackGroup> track)
{
	scene::ISceneNode*node=m_editor->GetSelectedNode();
	if(!node)
		return;
	GCPtr<scene::Skeleton> sk= node->getMesh()->getSkeleton();
	if(sk)
	{
		sk->addAnimationTrackGroup(track);
	}
}
void CAnimationDialog::PlayTrack(const core::string&name)
{
	scene::ISceneNode*node=m_editor->GetSelectedNode();
	if(!node)
		return;
	GCPtr<scene::Skeleton> sk= node->getMesh()->getSkeleton();
	if(sk)
	{
		animation::AnimationController* track=sk->getAnimationMixer()->getAnimationController(name);
		if(track){
			track->getGroup()->playAll();
			track->play(10,animation::EAM_Cycle);
		}
	}else
	{
		scene::SMesh*mesh= node->getMesh();
		if(!mesh)
			return;
		for(int i=0;i<mesh->getBuffersCount();++i)
		{
			scene::MeshBufferData* bd=mesh->getBufferData(i);

			animation::AnimationTrackGroup* t=bd->getAnimationMixer()->getTrackGroup(name);
			if(t)
				t->playAll();
		}
	}
}
void CAnimationDialog::StopTrack(const core::string&name)
{
	scene::ISceneNode*node=m_editor->GetSelectedNode();
	if(!node)
		return;
	GCPtr<scene::Skeleton> sk= node->getMesh()->getSkeleton();
	if(sk)
	{
		animation::AnimationController* track=sk->getAnimationMixer()->getAnimationController(name);
		if(track)
			track->stop();
	}
}

//////////////////////////////////////////////////////////////////////////


// CAnimationDialog message handlers

void CAnimationDialog::OnBnClickedLoadanimBtn()
{
	// TODO: Add your control notification handler code here
	CFileDialog openDialog(true,0,0,OFN_OVERWRITEPROMPT,mT("Anim Files *.anim|*.anim||"));

	int iRet=openDialog.DoModal();
	if(iRet==IDOK)
	{
		GCPtr<animation::IAnimationMixer>mixer=gAnimationResourceManager.getAnimation(openDialog.GetPathName().GetString());
		if(!mixer)
		{
			MessageBox(mT("Cann't Open Animation File!"),mT("Load Animation"));
			return;
		}
		animationFiles.push_back(openDialog.GetPathName().GetString());
		core::IteratorPair<animation::IAnimationMixer::ATGroupList> p= mixer->getTrackGroups();
		//TracksList.ResetContent();
		for (;!p.done();++p)
		{
			GCPtr<animation::AnimationTrackGroup> at=(*p).second;
			TracksList.AddString(at->getName().c_str());
			AddAnimationTrack(at);
		}
	}
}

void CAnimationDialog::OnBnClickedPlayanimBtn()
{
	// TODO: Add your control notification handler code here
	int i=TracksList.GetCurSel();
	if(i==-1)
		return;
	CString str;
	TracksList.GetLBText(i,str);
	PlayTrack(str.GetString());
}

void CAnimationDialog::OnBnClickedStopanimBtn()
{
	// TODO: Add your control notification handler code here
	int i=TracksList.GetCurSel();
	if(i==-1)
		return;
	CString str;
	TracksList.GetLBText(i,str);
	StopTrack(str.GetString());
}
