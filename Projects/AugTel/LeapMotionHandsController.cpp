

#include "stdafx.h"
#include "LeapMotionHandsController.h"
#include "LeapMotionController.h"
#include "ParsedShaderPP.h"
#include "TextureRTWrap.h"
#include "AugTelSceneContext.h"
#include "HeadMount.h"
#include "SceneComponent.h"
#include "CameraComponent.h"
#include "HeadCameraComponent.h"
#include "BoneComponent.h"
#include "MeshBufferData.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "LeapMotionImageRetrival.h"

#include "LeapHandController.h"
#include "LeapHand.h"
#include "LeapFinger.h"

#include "FontResourceManager.h"
#include "GUIBatchRenderer.h"
#include "ShaderResourceManager.h"
#include "ICameraVideoSource.h"
#include "LeapHandComponent.h"
#include "FingerTipComponent.h"

#include "TextureResourceManager.h"

namespace mray
{
namespace AugTel
{

	class VirtualHandsLightImpl
	{

	protected:

		void GenerateBRDF()
		{
			BRDFTexture = gEngine.getDevice()->createEmptyTexture2D(false);
			BRDFTexture->setMipmapsFilter(false);
			BRDFTexture->createTexture(math::vector3d(512, 512, 1), video::EPixel_Float16_R);
			video::IRenderTargetPtr rt = gEngine.getDevice()->createRenderTarget("", BRDFTexture, 0, 0, 0);

			video::IGPUShaderProgramPtr BRDF_FP = gShaderResourceManager.loadShaderFromFile("BRDF_PreComp.cg", video::EShader_FragmentProgram, "main_fp_PHBeckmann", video::ShaderPredefList(), "cg");
			gEngine.getDevice()->set2DMode();
			gEngine.getDevice()->setRenderTarget(rt);
			gEngine.getDevice()->setFragmentShader(BRDF_FP);
			gEngine.getDevice()->draw2DImage(math::rectf(0, 0, 512, 512), 1);
			gEngine.getDevice()->setRenderTarget(0);
			gEngine.getDevice()->setFragmentShader(0);
			rt = 0;
		}
	public:
		video::ITexturePtr BRDFTexture;

		AugTelSceneContext* context;

		int m_lightMapTimer;

		video::ParsedShaderPP* m_blurShader;
		std::vector<game::GameEntity*> entLst;

		LeapHandComponent* leftHand;
		LeapHandComponent* rightHand;

	public:
		VirtualHandsLightImpl()
		{
			m_lightMapTimer = 0;
		}
		~VirtualHandsLightImpl()
		{
		}

		void Init()
		{
			video::ParsedShaderPP* pp = new video::ParsedShaderPP(gEngine.getDevice());
			pp->LoadXML(gFileSystem.openFile("blur.peff"));
			m_blurShader = pp;

			GenerateBRDF();
		}

		void LoadHands(const core::string& path)
		{
			context->entManager->loadFromFile(path, &entLst);
			game::SceneComponent* modelComp = game::IGameComponent::RetriveComponent<game::SceneComponent>(entLst[0], "SkinnedArms");
			if (modelComp)
			{
				for (int i = 0; i < modelComp->GetSceneNode()->GetAttachedNodesCount(); ++i)
				{
					scene::IRenderable* r = modelComp->GetSceneNode()->GetAttachedNode(i);
					r->getMaterial(0)->GetTechniqueAt(0)->GetPassAt(0)->setTexture(m_blurShader->getOutput()->GetColorTexture(0), 3);
					//r->getMaterial(0)->GetTechniqueAt(0)->GetPassAt(0)->setTexture(m_data->BRDFTexture, 3);
				}
			}

			leftHand = game::IGameComponent::RetriveComponent<LeapHandComponent>(entLst[0], "LeftArm");
			rightHand = game::IGameComponent::RetriveComponent<LeapHandComponent>(entLst[0], "RightArm");
		}

		void Update()
		{
			return;
			if (gEngine.getTimer()->getSeconds() - m_lightMapTimer < 200)
				return;
			m_lightMapTimer = gEngine.getTimer()->getSeconds();
			m_blurShader->Setup(math::rectf(0, 0, 256, 256));
			ulong currTime = gEngine.getTimer()->getMilliseconds();
			/*
			if (m_takeScreenShot)
			{

			core::string name = gFileSystem.getAppPath() + "screenShots/Image_";
			name += core::StringConverter::toString(currTime);
			name += ".png";
			gTextureResourceManager.writeResourceToDist(m_camVideoSrc->GetEyeTexture(0), name);
			}*/
			video::IRenderTarget* prevRT = gEngine.getDevice()->getRenderTarget();
			video::ITexture* t = context->videoSource->GetEyeTexture(0);
			for (int i = 0; i < 10; ++i)
			{
				m_blurShader->GetParam("Offset")->SetValue((i + 1) * 4);
				m_blurShader->render(&video::TextureRTWrap(t));
				t = m_blurShader->getOutput()->GetColorTexture();
				/*

				if (m_takeScreenShot)
				{
				time_t rawtime;
				struct tm  timeinfo;

				ulong currTime = gEngine.getTimer()->getMilliseconds();

				core::string name = gFileSystem.getAppPath() + "screenShots/ImageDownSample" + core::StringConverter::toString(i) + "_";
				name += core::StringConverter::toString(currTime);
				name += ".png";
				gTextureResourceManager.writeResourceToDist(m_blurShader->getOutput()->GetColorTexture(0), name);

				}*/
			}

			gEngine.getDevice()->setRenderTarget(prevRT, 0, 0);
		}

	};
	class LeapMotionHandsControllerImpl:public Leap::Listener,public ILeapHandControllerListener
	{
	public:

		 const int DEFAULT_TEXTURE_WIDTH = 640;
		 const int DEFAULT_TEXTURE_HEIGHT = 240;
		 const int DEFAULT_DISTORTION_WIDTH = 64;
		 const int DEFAULT_DISTORTION_HEIGHT = 64;


		LeapMotionController* m_controller;
		LeapHandController* m_handController;

		VirtualHandsLightImpl* m_handsImpl;
		
		std::vector<AugTel::FingerTipComponent*> m_leftFingers;
		std::vector<AugTel::FingerTipComponent*> m_rightFingers;
		AugTelSceneContext* context;
		LeapMotionImageRetrival* images[2];

		core::string m_model;

		bool m_screenShot;

		bool enabled;
		scene::MeshRenderableNode* m_surface[2];
		scene::ISceneNode* m_screenNode[2];
		video::RenderPass* m_screenMtrl[2];

		LeapMotionHandsControllerImpl()
		{
			enabled = true;
			m_controller = new LeapMotionController();
			images[0] = new LeapMotionImageRetrival(0);
			images[1] = new LeapMotionImageRetrival(1);
			m_handController = new LeapHandController(m_controller);
			m_handController->AddListener(this);

			m_handsImpl = new VirtualHandsLightImpl();

			m_screenShot = false;
		}
		~LeapMotionHandsControllerImpl()
		{
			delete m_handController;
			delete m_controller;
			delete images[0];
			delete images[1];
			delete m_handsImpl;
		}

		void setEnabled(bool e)
		{
			enabled = e;
			if (enabled)
			{
				m_controller->AddListener(this);
			}
			else
			{
				m_controller->RemoveListener(this);
			}
			for (int i = 0; i < m_handsImpl->entLst.size(); ++i)
			{
				m_handsImpl->entLst[i]->SetEnabled(e);
			}

			m_screenNode[0]->setVisible(e,true);
			m_screenNode[1]->setVisible(e, true);
		}

		void GenerateSurface()
		{
			int segments = 1;
			int vertCount = (segments + 1)*(segments + 1);
			int indCount = 6 * segments*segments;

			for (int i = 0; i < 2; ++i)
			{
				if (m_surface[i])
				{
					m_screenNode[i]->RemoveNode(m_surface[i]);
					m_surface[i] = 0;
				}
			}

			//Create Screen Node
			for (int i = 0; i < 2; ++i)
			{
				m_screenNode[i] = context->entManager->GetSceneManager()->createSceneNode("ScreenNode");
				scene::MeshRenderableNode* rnode = new scene::MeshRenderableNode(new scene::SMesh());
				m_surface[i] = rnode;

				scene::MeshBufferData* bdata = rnode->getMesh()->addNewBuffer();
				scene::IMeshBuffer* buffer = bdata->getMeshBuffer();

				video::IHardwareStreamBuffer* pos = buffer->createStream(0, video::EMST_Position, video::ESDT_Point3f, 4, video::IHardwareBuffer::EUT_WriteOnly, false, false);
				video::IHardwareStreamBuffer* tc = buffer->createStream(0, video::EMST_Texcoord, video::ESDT_Point2f, 4, video::IHardwareBuffer::EUT_WriteOnly, false, false);
				video::IHardwareIndexBuffer* idx = buffer->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit, 6, video::IHardwareBuffer::EUT_WriteOnly, false, false);

				math::vector3d posPtr[4] =
				{
					math::vector3d(-1, -1, 0),
					math::vector3d(+1, -1, 0),
					math::vector3d(+1, +1, 0),
					math::vector3d(-1, +1, 0)
				};
				math::vector2d tcPtr[4] = {
					math::vector2d(1, 0),
					math::vector2d(0, 0),
					math::vector2d(0, 1),
					math::vector2d(1, 1),
				};

				math::matrix3x3 rotMat;

			//	for (int j = 0; j < 4; ++j)
			//		tcPtr[j] = (rotMat*(tcPtr[j] * 2 - 1))*0.5 - 0.5f;
				ushort idxPtr[6] = { 0, 1, 2, 0, 2, 3 };

				pos->writeData(0, 4 * sizeof(math::vector3d), posPtr, true);
				tc->writeData(0, 4 * sizeof(math::vector2d), tcPtr, true);
				idx->writeData(0, 6 * sizeof(ushort), idxPtr, true);

				video::RenderMaterialPtr mtrl = new video::RenderMaterial();
				m_screenMtrl[i] = mtrl->CreateTechnique("Default")->CreatePass("ScreenPass");
				bdata->setMaterial(mtrl);

// 				rnode->SetHasCustomRenderGroup(true);
// 				rnode->SetTargetRenderGroup(scene::RGH_Solid - 5);

				m_screenMtrl[i]->setRenderState(video::RS_Lighting, video::ES_DontUse);
				m_screenMtrl[i]->setRenderState(video::RS_ZWrite, video::ES_DontUse);
				m_screenMtrl[i]->setRenderState(video::RS_ZTest, video::ES_DontUse);
				m_screenMtrl[i]->setRenderState(video::RS_CullFace, video::ES_DontUse);
				//m_screenMtrl[i]->setMaterialRenderer(video::MRT_TRANSPARENT);
				m_screenMtrl[i]->SetAlphaFunction(video::EAF_GreaterEqual);
				m_screenMtrl[i]->SetAlphaReferenceValue(0.5);

				rnode->SetTargetRenderGroup(scene::RGH_Transparent + 10);
				rnode->SetHasCustomRenderGroup(true);
				m_screenNode[i]->AttachNode(rnode);
				context->headNode -> addChild(m_screenNode[i]);
				m_screenNode[i]->setPosition(math::vector3d(0, 0, 1));
				m_screenNode[i]->setScale(math::vector3d(2, 2, 1.55));
				m_screenNode[i]->setVisible(false);
				m_screenNode[i]->setCullingType(scene::SCT_NONE);
			}

// 			context->headNode->GetLeftCamera()->addChild(m_screenNode[0]);
// 			context->headNode->GetRightCamera()->addChild(m_screenNode[1]);


		}
		void _loadHands()
		{

			//context->entManager->loadFromFile(m_model, &entLst);
			GenerateSurface();

			m_handsImpl->Init();
			m_handsImpl->context = context;
			m_handsImpl->LoadHands(m_model);
			game::GameEntity* ent = m_handsImpl->entLst[0];
			VT::CameraComponent* eyesComponent = ent->RetriveComponent<VT::CameraComponent>(ent, "eyes");
			LeapHand* hands[] = {
				m_handController->GetleftHand(),
				m_handController->GetRightHand()
			};

			if (m_handsImpl->leftHand)
				m_handsImpl->leftHand->SetHand(m_handController->GetleftHand());
			if (m_handsImpl->rightHand)
				m_handsImpl->rightHand->SetHand(m_handController->GetRightHand());

			core::string LRStr[] = { "L", "R" };
			for (int i = 0; i < 2; ++i)
			{
				std::stringstream str;
				str << LRStr[i] << "Wrist" ;
				game::SceneComponent*wrist = ent->RetriveComponent<game::SceneComponent>(ent, str.str());
				str.str(std::string());
				str.clear();
				str << LRStr[i] << "Elbow";
				game::SceneComponent*elbow = ent->RetriveComponent<game::SceneComponent>(ent, str.str());

				if (wrist)
				{
					hands[i]->SetWristJoint(wrist->GetSceneNode());
				}
				if (elbow)
				{
					hands[i]->SetForeamNode(elbow->GetSceneNode());
				}

				for (int j = 0; j < 5; ++j)
				{
					for (int k = 0; k < 3; ++k)
					{
						str.str(std::string());
						str.clear();
						str << LRStr[i] << "Finger_" << j << "_" << k;
						game::SceneComponent*comp= ent->RetriveComponent<game::SceneComponent>(ent, str.str());
						if (comp)
						{
						//	comp->GetSceneNode()->setScale(100);
							hands[i]->GetFinger((ELeapFinger)j)->SetNode(comp->GetSceneNode(), k+1);
						}
					}
				}
			}

			{
				const std::list<IObjectComponent* >& lst= ent->GetComponentsList();
				std::list<IObjectComponent*>::const_iterator it = lst.begin();
				for (; it != lst.end(); ++it)
				{
					IObjectComponent* o = *it;
					AugTel::FingerTipComponent* c = dynamic_cast<FingerTipComponent*>(o);
					if (c)
					{
						if (c->GetIsLeftHand())
						{
							m_leftFingers.push_back(c);
						}
						else
							m_rightFingers.push_back(c);
					}
				}

			}

			 if (eyesComponent)
			{

				scene::ISceneNode* handRoot = context->entManager->GetSceneManager()->createSceneNode();
				handRoot->setPosition(math::vector3d(0, 0.08, 0));
				context->headNode->addChild(handRoot);
				 eyesComponent->GetNode()->addChild(context->headNode);
				 context->headNode->setPosition(eyesComponent->GetOffset());
				 m_handController->SetTransform(handRoot);
				eyesComponent->MountCamera(context->headNode, 0);
			}


		}

		virtual void OnHandCreated(LeapHandController* c, LeapHand* hand)
		{
			printf("Hand Created: %s\n", hand->GetHand().isLeft()?"Left":"Right");
		}
		virtual void OnHandRemoved(LeapHandController* c, LeapHand* hand)
		{
			printf("Hand Removed: %s\n", hand->GetHand().isLeft() ? "Left" : "Right");
		}
		void Init(AugTelSceneContext* c)
		{
			context = c;
			m_controller->Init();
			m_controller->GetController()->setPolicyFlags(Leap::Controller::POLICY_IMAGES);

			_loadHands();
		}

		void Start()
		{
			if (enabled)
				m_controller->AddListener(this);
		}
		void End()
		{
			m_controller->RemoveListener(this);
		}

		void OnRender(const math::rectf& rc, TBee::ETargetEye eye)
		{
			if (!enabled)
				return;
			if (!m_controller->GetController()->isConnected())
			{

				m_screenNode[0]->setVisible(false);
				m_screenNode[1]->setVisible(false);
				return;
			}
			Leap::Frame frame = m_controller->GetController()->frame();

			if (images[eye]->Capture(frame))
			{
				float palmY = frame.hands().rightmost().palmPosition().y;
//  				m_screenNode[eye]->setPosition(math::vector3d(0, 0, palmY*0.01));
//  				m_screenNode[eye]->setScale(math::vector3d(palmY, palmY, palmY)*0.01);

			}
			m_screenMtrl[eye]->setTexture(images[eye]->GetResult(), 0);


			m_screenNode[eye]->setVisible(true);
			m_screenNode[1-eye]->setVisible(false);
		}

		void Update(float dt)
		{
			if (!enabled)
				return;
			Leap::Frame frame = m_controller->GetController()->frame();

			m_handController->Update(dt);

			m_handsImpl->Update();

			bool enable = false;
			if (m_handController->GetleftHand()->GetHand().isValid())
			{
				enable = true;
			}
			for (int i = 0; i < m_leftFingers.size(); ++i)
				m_leftFingers[i]->SetEnabled(enable);
			enable = false;
			if (m_handController->GetRightHand()->GetHand().isValid())
			{
				enable = true;
			}
			for (int i = 0; i < m_rightFingers.size(); ++i)
				m_rightFingers[i]->SetEnabled(enable);

			if (gAppData.inputMngr->getKeyboard()->getKeyState(KEY_F12) )
			{
				if (!m_screenShot)
				{
					m_screenShot = true;
					int secs = gEngine.getTimer()->getMilliseconds();
					core::string fname = gFileSystem.getAppPath() + "/Screenshots/HandsOriginal_" + core::StringConverter::toString(secs) + ".png";
					gTextureResourceManager.writeResourceToDist(images[0]->GetCapturedTexture(), fname);
					fname = gFileSystem.getAppPath() + "/Screenshots/HandsCorrect_" + core::StringConverter::toString(secs) + ".png";
					gTextureResourceManager.writeResourceToDist(images[0]-> GetResult(), fname);

				}
			}
			else
			{
				m_screenShot = false;
			}
		}
	};

LeapMotionHandsController::LeapMotionHandsController()
{
	m_data = new LeapMotionHandsControllerImpl();
	m_enabled = true;
}
LeapMotionHandsController::~LeapMotionHandsController()
{
	delete m_data;
}


void LeapMotionHandsController::Init(AugTelSceneContext* context)
{
	m_data->Init(context);
}

void LeapMotionHandsController::Start(AugTelSceneContext* context)
{
	m_data->Start();
}

void LeapMotionHandsController::End(AugTelSceneContext* context)
{
	m_data->End();
}

void LeapMotionHandsController::Update(float dt)
{
	if (!m_enabled)
		return;

	m_data->Update(dt);

	controllers::IKeyboardController* kb = gAppData.inputMngr->getKeyboard();

	math::vector3d s;
	if (kb->getKeyState(KEY_RCONTROL))
	{
		s.x = (kb->getKeyState(KEY_LEFT) - kb->getKeyState(KEY_RIGHT))*dt*0.5f;
		s.y = (kb->getKeyState(KEY_UP) - kb->getKeyState(KEY_DOWN))*dt*0.5f;

		m_data->m_screenNode[0]->setScale(m_data->m_screenNode[0]->getScale() + s);
		m_data->m_screenNode[1]->setScale(m_data->m_screenNode[0]->getScale() + s);
	}
}

void LeapMotionHandsController::RenderStart(const math::rectf& rc, TBee::ETargetEye eye)
{
	if (!m_enabled)
		return;
	m_data->OnRender(rc, eye);
}

void LeapMotionHandsController::DebugRender(scene::IDebugDrawManager* dbg, const math::rectf& rc, TBee::ETargetEye eye)
{
	if (!m_enabled)
		return;

	LeapHand* leftHand = m_data->m_handController->GetleftHand();
	LeapHand* rightHand = m_data->m_handController->GetRightHand();
	/*
	gEngine.getDevice()->set2DMode();
	video::TextureUnit tu;
	tu.SetTexture(m_data->images[eye]->GetResult());
	gEngine.getDevice()->useTexture(0,&tu);
	//gEngine.getDevice()->draw2DImage(rc, 1);

	gEngine.getDevice()->useTexture(0,0);


	for (int i = 0; i < 5; ++i)
	{
		std::stringstream str;
		math::vector3d p = rightHand->GetFinger((ELeapFinger)i)->GetTipPosition();

	}



	math::vector2d pos;
	GUI::IFont* font = gFontResourceManager.getDefaultFont();
	GUI::FontAttributes attr;
	GUI::GUIBatchRenderer m_guiRenderer;
	m_guiRenderer.SetDevice(gEngine.getDevice());
	pos.x = 20;
	pos.y = 200;
	if (font)
	{
		attr.fontColor.Set(1, 1, 1, 1);
		attr.fontAligment = GUI::EFA_MiddleLeft;
		attr.fontSize = 18;
		attr.hasShadow = true;
		attr.shadowColor.Set(0, 0, 0, 1);
		attr.shadowOffset = math::vector2d(2);
		attr.spacing = 2;
		attr.wrap = 0;
		attr.RightToLeft = 0;

		math::rectf r = rc;
		r.ULPoint = pos;

		core::string msg = mT("Hand Position:") + core::StringConverter::toString(rightHand->GetWristPosition());
		font->print(r, &attr, 0, msg, &m_guiRenderer);
		r.ULPoint.y += attr.fontSize + 5;
		for (int i = 0; i < 5; ++i)
		{
			std::stringstream str;
			math::vector3d p = rightHand->GetFinger((ELeapFinger)i)->GetTipPosition();
			str << "Finger[" << i << "] Position: " << core::StringConverter::toString(p);
			core::string msg = str.str();
			font->print(r, &attr, 0, msg, &m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
		}
		m_guiRenderer.Flush();
	}*/
	leftHand->DrawDebug(dbg);
	rightHand->DrawDebug(dbg);
}


math::vector3d LeapMotionHandsController::GetHandPosition(EHandType hand)
{
	return 0;
}

math::quaternion LeapMotionHandsController::GetHandRotation(EHandType hand)
{
	return 0;
}

math::vector3d LeapMotionHandsController::GetFingerPosition(EHandType hand, EFingerType finger)
{
	return 0;
}


void LeapMotionHandsController::LoadFromXML(xml::XMLElement* e)
{
	m_data->m_model = e->getValueString("Model");

}


void LeapMotionHandsController::SetEnabled(bool e)
{
	m_enabled = e;
	m_data->setEnabled(e);
}

}
}



