

#include "stdafx.h"
#include "IdleShapesEffect.h"
#include "Engine.h"
#include "matrix3x3.h"

namespace mray
{


	IdleShapeBase::IdleShapeBase()
	{
		m_sizeScaler=0;
		m_alphaScaler=0;
		m_time=0;
		m_rand=math::Randomizer::rand01();
		m_lifespan=math::Randomizer::rand01()*10+3;
		m_velocity=math::vector2d(math::Randomizer::rand01()-0.5,math::Randomizer::rand01()-0.5)*2*m_lifespan;
		m_color.setHSL(math::Randomizer::rand01(),math::Randomizer::rand01(),math::Randomizer::rand01());
		m_color.A=1;
		m_fill=math::Randomizer::rand01()>0.5;

		float r=m_rand*30;
		float a=m_rand*math::PI32*2;
		m_pos.x=r*cos(a);
		m_pos.y=r*sin(a);
	}
	void IdleShapeBase::Update(float dt)
	{
		m_sizeScaler+=dt*m_rand*20;
		m_alphaScaler+=dt;
		m_pos+=m_velocity*dt*m_rand*20;
		m_time+=dt;
	}

	class IdleMesh
	{
	public:
		void Init(int n)
		{
			points.resize(n);
			float step=math::PI32*2/(float)n;
			for(int i=0;i<n;++i)
			{
				points[i].x=cos(step*i);
				points[i].y=sin(step*i);
			}
		}
		std::vector<math::vector2d> points;

		class IdleMeshInitiater
		{
		public:
			IdleMeshInitiater()
			{
				int n=32;
				Meshes.resize(n);
				for(int i=0;i<n;++i)
					Meshes[i].Init(i+3);
			}
		
			std::vector<IdleMesh> Meshes;
			static IdleMeshInitiater instance;
		};
	};

	IdleMesh::IdleMeshInitiater  IdleMesh::IdleMeshInitiater::instance;
	class IdleShape:public IdleShapeBase
	{
	protected:
		float m_size;
		IdleMesh* m_mesh;
	public:
		IdleShape(IdleMesh* mesh)
		{
			m_mesh=mesh;
			m_size=math::Randomizer::rand01()+0.1;
		}
		virtual void Draw(const math::rectf& rc)
		{
			float sz=m_size*m_sizeScaler;
			math::rectf box;
			box.ULPoint=m_pos+rc.getCenter()-sz/2;
			box.BRPoint=box.ULPoint+sz;

			math::vector2d center=rc.getCenter();
			float angle=m_time*math::PI32;

			math::matrix3x3 m,m2;
			m.setAngle(angle);
			m2.f11=m2.f22=sz;
			m=m2*m;
			m.translate(m_pos+rc.getCenter());

			std::vector<math::vector2d> m_points;
			m_points.resize(m_mesh->points.size());
			for(int i=0;i<m_points.size();++i)
			{
				m_points[i]=m*math::vector2d(m_mesh->points[i]);
			}
			video::SColor clr=m_color;
			clr.A=1-m_time/m_lifespan;
			Engine::getInstance().getDevice()->draw2DShape(&m_points[0],m_points.size(),clr,m_fill);
		}
		virtual void Update(float dt)
		{
			IdleShapeBase::Update(dt);
		}
	};
IdleShapesEffect::IdleShapesEffect(EffectCreatePack* desc)
{
	m_time=0;
}
IdleShapesEffect::~IdleShapesEffect()
{
}

void IdleShapesEffect::Draw(const math::rectf& rc)
{
	for(std::list<IdleShapeBase*>::iterator it=m_shapes.begin();it!=m_shapes.end();++it)
	{
		(*it)->Draw(rc);
	}
}

void IdleShapesEffect::Update(float dt)
{
	std::list<IdleShapeBase*>::iterator it,it2;
	it=m_shapes.begin();
	while(it!=m_shapes.end())
	{
		(*it)->Update(dt);
		if((*it)->IsDone())
		{
			it2=it;
			++it2;
			delete *it;
			m_shapes.erase(it);
			it=it2;
		}else
			++it;
	}

	m_time-=dt;

	if(m_time<=0)
	{
		m_time=math::Randomizer::rand01()*0.05;
		int n=math::Randomizer::rand(IdleMesh::IdleMeshInitiater::instance.Meshes.size());
		m_shapes.push_back(new IdleShape(&IdleMesh::IdleMeshInitiater::instance.Meshes[n]));
	}
}

bool IdleShapesEffect::IsDone()
{
	return false;
}


}