// EntityDialog.cpp : implementation file
//

#include "stdafx.h"
#include "XEditor.h"
#include "EntityDialog.h"
#include "GameActionsDialog.h"
#include "CEngineWindow.h"
#include "EditorEventMessages.h"
#include "GameActionFactory.h"


// CEntityDialog dialog

IMPLEMENT_DYNAMIC(CEntityDialog, CDialog)

CEntityDialog::CEntityDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEntityDialog::IDD, pParent)
	, EntityName(_T(""))
	, EntityID(0)
	, AttributeValue(_T(""))
{
	m_editor=0;
	EditorEventManager::getInstance().AddHandler(EEE_OnModelSelected,newClassDelegate1<CEntityDialog,void,mray::PVOID>(mT(""),this,&CEntityDialog::OnModelSelected));

}

CEntityDialog::~CEntityDialog()
{
}

void CEntityDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ENTITYNAME_TXT, EntityName);
	DDX_Text(pDX, IDC_ENTITY_ID_TXT, EntityID);
	DDX_Control(pDX, IDC_ATTRIBUTE_LIST, AttributesLst);
	DDX_Text(pDX, IDC_MOD_ATTR_VALUE_TXT, AttributeValue);
}


BEGIN_MESSAGE_MAP(CEntityDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON3, &CEntityDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_ACTION_NAME_BTN, &CEntityDialog::OnBnClickedActionNameBtn)
	ON_CBN_SELCHANGE(IDC_ATTRIBUTE_LIST, &CEntityDialog::OnCbnSelchangeAttributeList)
	ON_EN_KILLFOCUS(IDC_MOD_ATTR_VALUE_TXT, &CEntityDialog::OnEnKillfocusModAttrValueTxt)
END_MESSAGE_MAP()


void CEntityDialog::SetEditor(mray::CEngineWindow*e)
{
	m_editor=e;
}
void CEntityDialog::OnModelSelected(const mray::PVOID&)
{
	RefreshAttributes();
	scene::ISceneNode*node=m_editor->GetSelectedNode();
	if(!node)
		return;
	uint id=m_editor->GetLevel()->GetEntityManager()->GetSceneNodeEntID(node->getID());
	if(id==0){
		EntityID=-1;
		EntityName="";
		UpdateData(false);
		return;
	}
	GCPtr<game::GameEntity> ent=m_editor->GetLevel()->GetEntityManager()->GetGameEntity(id);
	EntityName=ent->GetName().c_str();
	EntityID=(int)id;

	UpdateData(false);
}


// CEntityDialog message handlers

void CEntityDialog::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	scene::ISceneNode*node=m_editor->GetSelectedNode();
	if(!node)
		return;
	uint id=m_editor->GetLevel()->GetEntityManager()->GetSceneNodeEntID(node->getID());
	if(id!=0)
		return;

	UpdateData(true);

	GCPtr<game::GameEntity> ent=m_editor->GetLevel()->GetEntityManager()->CreateGameEntity(EntityName.GetString());
	ent->SetShape(node);
	m_editor->GetLevel()->GetEntityManager()->SetSceneNodeEntID(node->getID(),ent->GetId());

	EntityName=ent->GetName().c_str();
	EntityID=(int)ent->GetId();

	UpdateData(false);

}

void CEntityDialog::OnBnClickedActionNameBtn()
{
	// TODO: Add your control notification handler code here
	scene::ISceneNode*node=m_editor->GetSelectedNode();
	if(!node)
		return;
	uint id=m_editor->GetLevel()->GetEntityManager()->GetSceneNodeEntID(node->getID());
	if(id==0)
		return;

	GCPtr<game::GameEntity> ent=m_editor->GetLevel()->GetEntityManager()->GetGameEntity(id);
	
	// TODO: Add your control notification handler code here
	CGameActionsDialog dlg;
	if(dlg.DoModal()==IDOK)
	{
		GCPtr<game::IGameAction> c=game::GameActionFactory::getInstance().CreateAction(dlg.ActionName);
		ent->SetGameAction(c);
		RefreshAttributes();
	}
}

void CEntityDialog::RefreshAttributes()
{
	AttributesLst.ResetContent();
	AttributeValue="";
	UpdateData(true);
	scene::ISceneNode*node=m_editor->GetSelectedNode();
	if(!node)
		return;
	uint id=m_editor->GetLevel()->GetEntityManager()->GetSceneNodeEntID(node->getID());
	if(id==0)
		return;

	game::GameEntity* ent=m_editor->GetLevel()->GetEntityManager()->GetGameEntity(id);

	game::IGameAction*action=ent->GetGameAction();

	if(!action)
		return;
	core::IteratorPair<ValuesMap> p=action->GetAttributes();
	
	for(int i=0;!p.done();++p,++i)
	{
		AttributesLst.AddString((*p).second->getName().c_str());
		AttributesLst.SetItemData(i,(DWORD_PTR)(*p).second);
	}
}


void CEntityDialog::OnCbnSelchangeAttributeList()
{
	// TODO: Add your control notification handler code here
	int i=AttributesLst.GetCurSel();
	if(i==-1)
		return;

	IValue*val=(IValue*)AttributesLst.GetItemData(i);
	if(!val)
		return;

	AttributeValue=val->toString();
	UpdateData(false);
}

void CEntityDialog::OnEnKillfocusModAttrValueTxt()
{
	// TODO: Add your control notification handler code here
	int i=AttributesLst.GetCurSel();
	if(i==-1)
		return;

	IValue*val=(IValue*)AttributesLst.GetItemData(i);
	if(!val)
		return;

	UpdateData();

	val->parse(AttributeValue.GetString());
}
