

#include "stdafx.h"
#include "FluidSolver.h"
#include "IThreadFunction.h"
#include "IThreadManager.h"
#include "IThread.h"
#include "IMutex.h"

namespace mray
{
namespace FX
{
	class FluidSolverThread:public OS::IThreadFunction
	{
	public:
		FluidSolver* m_solver;
	public:
		FluidSolverThread(FluidSolver*s)
		{
			m_solver=s;
		}
		virtual void execute(OS::IThread*caller,void*arg)
		{
			float dt=1.0f/60.0f;
			while(caller->isActive())
			{
				if(m_solver->IsReady())
				{
					m_solver->Lock();
					m_solver->Update(dt);
					m_solver->Unlock();
					OS::IThreadManager::getInstance().sleep(1000*dt);
				}
			}
		}
	};
	class FluidSolverImpl
	{
	public:
		FluidSolverDesc desc;
		FluidSolver* m_solver;
		OS::IThread *m_thread;
		OS::IMutex* m_mutex;

		int cellCount;
		float invCellCount;
		math::vector2d invResolution;
		math::vector2d invSize;

		bool inited;

		float* density,*oldDensity;
		video::SColor* color,*oldColor;
		math::vector2d* uv,*oldUV;
		float* curl;

		float avgDensity ;
		float avgSpeed ;
		float uniformity ;

#define FLUID_INDEX(i,j) ((j)*(desc.resolution.x+2)+(i))
		FluidSolverImpl(FluidSolver*solver)
		{
			density=0;
			oldDensity=0;
			oldColor=0;
			color=0;
			uv=0;
			oldUV=0;
			curl=0;
			inited=false;
			cellCount=0;
			m_solver=solver;
			m_mutex=OS::IThreadManager::getInstance().createMutex();
		}
		~FluidSolverImpl()
		{
			Clean();
			delete m_mutex;
		}
		inline int GetIndexForCell(int x,int y)const
		{
			x=math::clamp(x,1,desc.resolution.x);
			y=math::clamp(y,1,desc.resolution.y);
			return FLUID_INDEX(x,y);
		}

		void Clean()
		{
			if(inited)
			{
				Lock();
				OS::IThreadManager::getInstance().killThread(m_thread);
				m_thread->terminate();
				delete []density;
				delete []color;
				delete []uv;
				delete []oldDensity;
				delete []oldColor;
				delete []oldUV;
				delete []curl;
				density=0;
				oldDensity=0;
				oldColor=0;
				color=0;
				uv=0;
				oldUV=0;
				curl=0;
				inited=false;
				Unlock();
			}

		}
		void randomizeColor() {
			for (int i = GetWidth()-1; i > 0; --i)
			{
				for (int j = GetHeight()-1; j > 0; --j)
				{
					int index = FLUID_INDEX(i, j);
					color[index] = video::SColor(math::Randomizer::rand01(), math::Randomizer::rand01(), math::Randomizer::rand01(),1);
					density[index] = math::Randomizer::rand01();
				}
			}
		}
		void Lock()
		{
			m_mutex->lock();
		}
		void Unlock()
		{
			m_mutex->unlock();
		}



		void Reset()
		{
			Clean();
			inited=true;
			density=new float[cellCount];
			oldDensity=new float[cellCount];
			uv=new math::vector2d[cellCount];
			oldUV=new math::vector2d[cellCount];
			curl=new float[cellCount];
			color=new video::SColor[cellCount];
			oldColor=new video::SColor[cellCount];
			for(int i=0;i<cellCount;++i)
			{
				density[i]=0;
				color[i]=0;
				uv[i]=0;
				oldDensity[i]=0;
				oldColor[i]=0;
				oldUV[i]=0;
				curl[i]=0;
			}

			m_thread=OS::IThreadManager::getInstance().createThread(new FluidSolverThread(m_solver));
			m_thread->start(0);

		}
		void SetupFromDesc(const FluidSolverDesc& d)
		{
			desc=d;
			Resize(d.resolution.x,d.resolution.y);
		}

		void Resize(int xRes,int yRes)
		{
			desc.resolution.set(xRes,yRes);
			cellCount=(xRes+2)*(yRes+2);
			invResolution.x=1.0f/xRes;
			invResolution.y=1.0f/yRes;
			invCellCount=1.0f/cellCount;
			Reset();
		}

		void SetBoundary(int bound,float* x)
		{
			int dst1, dst2, src1, src2;
			int step = FLUID_INDEX(0, 1) - FLUID_INDEX(0, 0);

			dst1 = FLUID_INDEX(0, 1);
			src1 = FLUID_INDEX(1, 1);
			dst2 = FLUID_INDEX(desc.resolution.x+1, 1);
			src2 = FLUID_INDEX(desc.resolution.x, 1);
			if(desc.wrapX)
				math::Swap(src1, src2);
			if(bound == 1 && !desc.wrapX)
				for (int i = desc.resolution.y; i > 0; --i)
				{
					x[dst1] = -x[src1];	dst1 += step;	src1 += step;	
					x[dst2] = -x[src2];	dst2 += step;	src2 += step;	
				}
			else
				for (int i = desc.resolution.y; i > 0; --i)
				{
					x[dst1] = x[src1];	dst1 += step;	src1 += step;	
					x[dst2] = x[src2];	dst2 += step;	src2 += step;	
				}

				dst1 = FLUID_INDEX(1, 0);
				src1 = FLUID_INDEX(1, 1);
				dst2 = FLUID_INDEX(1, desc.resolution.y+1);
				src2 = FLUID_INDEX(1, desc.resolution.y);
				if(desc.wrapY)
					math::Swap(src1, src2);
				if(bound == 2 && !desc.wrapY)
					for (int i = desc.resolution.x; i > 0; --i)
					{
						x[dst1++] = -x[src1++];	
						x[dst2++] = -x[src2++];	
					}
				else
					for (int i = desc.resolution.x; i > 0; --i)
					{
						x[dst1++] = x[src1++];
						x[dst2++] = x[src2++];	
					}

					x[FLUID_INDEX(0,   0)] = 0.5f * (x[FLUID_INDEX(1, 0)] + x[FLUID_INDEX(0, 1)]);
					x[FLUID_INDEX(0, desc.resolution.y+1)] = 0.5f * (x[FLUID_INDEX(1, desc.resolution.y+1)] + x[FLUID_INDEX(0, desc.resolution.y)]);
					x[FLUID_INDEX(desc.resolution.x+1,   0)] = 0.5f * (x[FLUID_INDEX(desc.resolution.x, 0)] + x[FLUID_INDEX(desc.resolution.x+1, 1)]);
					x[FLUID_INDEX(desc.resolution.x+1, desc.resolution.y+1)] = 0.5f * (x[FLUID_INDEX(desc.resolution.x, desc.resolution.y+1)] + x[FLUID_INDEX(desc.resolution.x+1, desc.resolution.y)]);
		}
		void SetBoundaryRGB()
		{
			int dst1, dst2, src1, src2;
			int step = FLUID_INDEX(0, 1) - FLUID_INDEX(0, 0);

			dst1 = FLUID_INDEX(0, 1);
			src1 = FLUID_INDEX(1, 1);
			dst2 = FLUID_INDEX(desc.resolution.x+1, 1);
			src2 = FLUID_INDEX(desc.resolution.x, 1);
			if(desc.wrapX)
				math::Swap(src1, src2);
			for (int i = desc.resolution.y; i > 0; --i)
			{
				color[dst1] = color[src1];
				dst1 += step;
				src1 += step;	

				color[dst2] = color[src2];
				dst2 += step;
				src2 += step;	
			}

			dst1 = FLUID_INDEX(1, 0);
			src1 = FLUID_INDEX(1, 1);
			dst2 = FLUID_INDEX(1, desc.resolution.y+1);
			src2 = FLUID_INDEX(1, desc.resolution.y);
			if(desc.wrapY)
				math::Swap(src1, src2);
			for (int i = desc.resolution.x; i > 0; --i)
			{
				color[dst1] = color[src1];
				++dst1;
				++src1;	

				color[dst2] = color[src2];
				++dst2;
				++src2;	
			}
		}
		void SetBoundary2d(int bound,math::vector2d* xy)
		{
			int dst1, dst2, src1, src2;
			int step = FLUID_INDEX(0, 1) - FLUID_INDEX(0, 0);

			dst1 = FLUID_INDEX(0, 1);
			src1 = FLUID_INDEX(1, 1);
			dst2 = FLUID_INDEX(desc.resolution.x+1, 1);
			src2 = FLUID_INDEX(desc.resolution.x, 1);
			if(desc.wrapX)
				math::Swap(src1, src2);
			if(bound == 1 && !desc.wrapX)
				for (int i = desc.resolution.y; i > 0; --i)
				{
					xy[dst1].x = -xy[src1].x;	dst1 += step;	src1 += step;	
					xy[dst2].x = -xy[src2].x;	dst2 += step;	src2 += step;	
				}
			else
				for (int i = desc.resolution.y; i > 0; --i)
				{
					xy[dst1].x = xy[src1].x;	dst1 += step;	src1 += step;	
					xy[dst2].x = xy[src2].x;	dst2 += step;	src2 += step;	
				}

				dst1 = FLUID_INDEX(1, 0);
				src1 = FLUID_INDEX(1, 1);
				dst2 = FLUID_INDEX(1, desc.resolution.y+1);
				src2 = FLUID_INDEX(1, desc.resolution.y);
				if(desc.wrapY)
					math::Swap(src1, src2);
				if(bound == 2 && !desc.wrapY)
					for (int i = desc.resolution.x; i > 0; --i)
					{
						xy[dst1++].y = -xy[src1++].y;	
						xy[dst2++].y = -xy[src2++].y;	
					}
				else
					for (int i = desc.resolution.x; i > 0; --i)
					{
						xy[dst1++].y = xy[src1++].y;
						xy[dst2++].y = xy[src2++].y;	
					}

					((float*)&xy[FLUID_INDEX(0,   0)])[bound-1] = 0.5f * (((float*)&xy[FLUID_INDEX(1, 0)])[bound-1] + ((float*)&xy[FLUID_INDEX(0, 1)])[bound-1]);
					((float*)&xy[FLUID_INDEX(0, desc.resolution.y+1)])[bound-1] = 0.5f * (((float*)&xy[FLUID_INDEX(1, desc.resolution.y+1)])[bound-1] +
						((float*)&xy[FLUID_INDEX(0, desc.resolution.y)])[bound-1]);
					((float*)&xy[FLUID_INDEX(desc.resolution.x+1,   0)])[bound-1] = 0.5f * (((float*)&xy[FLUID_INDEX(desc.resolution.x, 0)])[bound-1] + 
						((float*)&xy[FLUID_INDEX(desc.resolution.x+1, 1)])[bound-1]);
					((float*)&xy[FLUID_INDEX(desc.resolution.x+1, desc.resolution.y+1)])[bound-1] = 0.5f * (((float*)&xy[FLUID_INDEX(desc.resolution.x, desc.resolution.y+1)])[bound-1] +
						((float*)&xy[FLUID_INDEX(desc.resolution.x+1, desc.resolution.y)])[bound-1]);
		}
		void SetBoundary02d(math::vector2d* x)
		{
			int dst1, dst2, src1, src2;
			int step = FLUID_INDEX(0, 1) - FLUID_INDEX(0, 0);

			dst1 = FLUID_INDEX(0, 1);
			src1 = FLUID_INDEX(1, 1);
			dst2 = FLUID_INDEX(desc.resolution.x+1, 1);
			src2 = FLUID_INDEX(desc.resolution.x, 1);
			if(desc.wrapX)
				math::Swap(src1, src2);
			for (int i = desc.resolution.y; i > 0; --i)
			{
				x[dst1].x = x[src1].x;	dst1 += step;	src1 += step;	
				x[dst2].x = x[src2].x;	dst2 += step;	src2 += step;	
			}

			dst1 = FLUID_INDEX(1, 0);
			src1 = FLUID_INDEX(1, 1);
			dst2 = FLUID_INDEX(1, desc.resolution.y+1);
			src2 = FLUID_INDEX(1, desc.resolution.y);
			if(desc.wrapY)
				math::Swap(src1, src2);
			for (int i = desc.resolution.x; i > 0; --i)
			{
				x[dst1++] = x[src1++];
				x[dst2++] = x[src2++];	
			}

			x[FLUID_INDEX(0,   0)].x = 0.5f * (x[FLUID_INDEX(1, 0)].x + x[FLUID_INDEX(0, 1)].x);
			x[FLUID_INDEX(0, desc.resolution.y+1)].x = 0.5f * (x[FLUID_INDEX(1, desc.resolution.y+1)].x + x[FLUID_INDEX(0, desc.resolution.y)].x);
			x[FLUID_INDEX(desc.resolution.x+1,   0)].x = 0.5f * (x[FLUID_INDEX(desc.resolution.x, 0)].x + x[FLUID_INDEX(desc.resolution.x+1, 1)].x);
			x[FLUID_INDEX(desc.resolution.x+1, desc.resolution.y+1)].x = 0.5f * (x[FLUID_INDEX(desc.resolution.x, desc.resolution.y+1)].x + x[FLUID_INDEX(desc.resolution.x+1, desc.resolution.y)].x);
		}


		int GetWidth()const{return desc.resolution.x+2;}
		int GetHeight()const{return desc.resolution.y+2;}


		float GetInvWidth()const{return invSize.x;}
		float GetInvHeight()const{return invSize.y;}

		template<class T>
		void AddSource(T* v,T* oldV,float dt)
		{
			for(int i=cellCount-1;i>=0;--i)
			{
				v[i]=v[i]+oldV[i]*dt;
			}
		}

		void Update(float dt)
		{
			AddSource(uv,oldUV,dt);
			if(desc.doVorticityConfinement)
			{
				VorticityConfinement(oldUV);
				AddSource(uv,oldUV,dt);
			}
			math::Swap(uv,oldUV);
			diffuseUV(desc.viscocity,dt);
			project(uv,oldUV);
			math::Swap(uv,oldUV);
			advect2D(uv,oldUV,dt);
			project(uv,oldUV);

			if(desc.doRGB)
			{
				AddSource(color,oldColor,dt);
				math::Swap(color,oldColor);

				if(desc.colorDiffusion!=0)
				{
					diffuseRGB(0,desc.colorDiffusion,dt);
					math::Swap(color,oldColor);
				}
				advectRGB(0,uv,dt);
				fadeRGB();
			}else
			{
				AddSource(density,oldDensity,dt);
				math::Swap(density,oldDensity);
				if(desc.colorDiffusion!=0)
				{
					diffuse(0,density,oldDensity,desc.colorDiffusion,dt);
					math::Swap(density,oldDensity);
				}
				advect(0,density,oldDensity,uv,dt);
				fadeDensity();
			}

		}
		void VorticityConfinement(math::vector2d*vec)
		{
			float dw_dx,dw_dy,len,v;

			for(int j=desc.resolution.y;j>0;--j)
			{
				for(int i=desc.resolution.x;i>0;--i)
				{
					curl[FLUID_INDEX(i,j)]=	fabs(calcCurl(i,j));
				}
			}
			for(int j=desc.resolution.y-1;j>1;--j)
			{
				for(int i=desc.resolution.x-1;i>1;--i)
				{
					dw_dx=(curl[FLUID_INDEX(i+1,j)]-curl[FLUID_INDEX(i-1,j)]);
					dw_dy=(curl[FLUID_INDEX(i,j+1)]-curl[FLUID_INDEX(i,j-1)]);

					len=(float)sqrt(dw_dx*dw_dx+dw_dy*dw_dy)+0.000001;
					len=2.0f/len;
					dw_dy*=len;
					dw_dx*=len;
					v=calcCurl(i,j);

					vec[FLUID_INDEX(i,j)].set(-dw_dy*v,dw_dx*v);

				}
			}
		}

		float calcCurl(int i,int j)
		{
			float dy=uv[FLUID_INDEX(i,j+1)].x-uv[FLUID_INDEX(i,j-1)].x;
			float dx=uv[FLUID_INDEX(i+1,j)].y-uv[FLUID_INDEX(i-1,j)].y;
			return (dy-dx)*0.5f;
		}

#define ZERO_THRESH		1e-9			// if value falls under this, set to zero (to avoid denormal slowdown)
#define CHECK_ZERO(p)	if(fabsf(p)<ZERO_THRESH) p = 0
		void diffuseUV(float diff,float dt)
		{
			float a = dt * diff * desc.resolution.x * desc.resolution.y;
			linearSolverUV(a, 1.0 + 4 * a);
		}
		void project(math::vector2d*xy,math::vector2d*pDiv)
		{
			float	h;
			int		index;
			int		step_x = desc.resolution.x + 2;

			h = - 0.5f / desc.resolution.x;
			for (int j = desc.resolution.y; j > 0; --j)
			{
				index = FLUID_INDEX(desc.resolution.x, j);
				for (int i = desc.resolution.x; i > 0; --i)
				{
					pDiv[index].x = h * (xy[index+1].x - xy[index-1].x + xy[index+step_x].y - xy[index-step_x].y);
					pDiv[index].y = 0;
					--index;
				}
			}

			SetBoundary02d(reinterpret_cast<math::vector2d*>(&pDiv[0].x));
			SetBoundary02d(reinterpret_cast<math::vector2d*>(&pDiv[0].y));

			linearSolverProject(pDiv);

			float fx = 0.5f * desc.resolution.x;
			float fy = 0.5f * desc.resolution.y;	//maa	change it from desc.resolution.x to desc.resolution.Y
			for (int j = desc.resolution.y; j > 0; --j)
			{
				index = FLUID_INDEX(desc.resolution.x, j);
				for (int i = desc.resolution.x; i > 0; --i)
				{
					xy[index].x -= fx * (pDiv[index+1].x - pDiv[index-1].x);
					xy[index].y -= fy * (pDiv[index+step_x].x - pDiv[index-step_x].x);
					--index;
				}
			}

			SetBoundary2d(1, xy);
			SetBoundary2d(2, xy);
		}
		void advect2D(math::vector2d*uv,math::vector2d*duv,float dt)
		{
			int i0, j0, i1, j1;
			float s0, t0, s1, t1;
			int	index;

			const float dt0x = dt * desc.resolution.x;
			const float dt0y = dt * desc.resolution.y;

			for (int j = desc.resolution.y; j > 0; --j)
			{
				for (int i = desc.resolution.x; i > 0; --i)
				{
					index = FLUID_INDEX(i, j);
					float x = i - dt0x * duv[index].x;
					float y = j - dt0y * duv[index].y;

					if (x > desc.resolution.x + 0.5) x = desc.resolution.x + 0.5f;
					if (x < 0.5)     x = 0.5f;

					i0 = (int) x;
					i1 = i0 + 1;

					if (y > desc.resolution.y + 0.5) y = desc.resolution.y + 0.5f;
					if (y < 0.5)     y = 0.5f;

					j0 = (int) y;
					j1 = j0 + 1;

					s1 = x - i0;
					s0 = 1 - s1;
					t1 = y - j0;
					t0 = 1 - t1;

					uv[index].x = s0 * (t0 * duv[FLUID_INDEX(i0, j0)].x + t1 * duv[FLUID_INDEX(i0, j1)].x)
						+ s1 * (t0 * duv[FLUID_INDEX(i1, j0)].x + t1 * duv[FLUID_INDEX(i1, j1)].x);
					uv[index].y = s0 * (t0 * duv[FLUID_INDEX(i0, j0)].y + t1 * duv[FLUID_INDEX(i0, j1)].y)
						+ s1 * (t0 * duv[FLUID_INDEX(i1, j0)].y + t1 * duv[FLUID_INDEX(i1, j1)].y);

				}
			}
			SetBoundary2d(1, uv);
			SetBoundary2d(2, uv);
		}
		void diffuseRGB(int b,float diff,float dt)
		{
			float a = dt * diff * desc.resolution.x * desc.resolution.y;
			linearSolverRGB(a, 1.0 + 4 * a);
		}
		void advectRGB(int b,math::vector2d*duv,float dt)
		{
			int i0, j0;
			float x, y, s0, t0, s1, t1, dt0x, dt0y;
			int	index;

			dt0x = dt * desc.resolution.x;
			dt0y = dt * desc.resolution.y;

			for (int j = desc.resolution.y; j > 0; --j)
			{
				for (int i = desc.resolution.x; i > 0; --i)
				{
					index = FLUID_INDEX(i, j);
					x = i - dt0x * duv[index].x;
					y = j - dt0y * duv[index].y;

					if (x > desc.resolution.x + 0.5) x = desc.resolution.x + 0.5f;
					if (x < 0.5)     x = 0.5f;

					i0 = (int) x;

					if (y > desc.resolution.y + 0.5) y = desc.resolution.y + 0.5f;
					if (y < 0.5)     y = 0.5f;

					j0 = (int) y;

					s1 = x - i0;
					s0 = 1 - s1;
					t1 = y - j0;
					t0 = 1 - t1;

					i0 = FLUID_INDEX(i0, j0);	//we don't need col/row index any more but index in 1 dimension
					j0 = i0 + (desc.resolution.x + 2);
					color[index] = (oldColor[i0] * t0 + oldColor[j0] * t1) * s0 + (oldColor[i0+1] * t0 + oldColor[j0+1] * t1) * s1;
				}
			}
			SetBoundaryRGB();
		}

		void fadeRGB()
		{
			// I want the fluid to gradually fade out so the screen doesn't fill. the amount it fades out depends on how full it is, and how uniform (i.e. boring) the fluid is...
			//		float holdAmount = 1 - avgDensity * avgDensity * fadeSpeed;	// this is how fast the density will decay depending on how full the screen currently is
			float holdAmount = 1 - desc.fadeSpeed;

			avgDensity = 0;
			avgSpeed = 0;

			float totalDeviations = 0;
			float currentDeviation;
			video::SColor tmp;
			avgSpeed = 0;

			for (int i = cellCount-1; i >=0; --i)
			{
				// clear old values
				oldUV[i] = math::vector2d::Zero;
				oldColor[i].Set(0,0,0,1);

				// calc avg speed
				avgSpeed += uv[i].x * uv[i].x + uv[i].y * uv[i].y;

				// calc avg density
				tmp.R = math::Min(1.0f, color[i].R);
				tmp.G = math::Min(1.0f, color[i].G);
				tmp.B = math::Min(1.0f, color[i].B);
				//			tmp.a = min(1.0f, color[i].a);

				//			float density = max(tmp.a, max(tmp.r, max(tmp.g, tmp.b)));
				//			float density = max(tmp.r, max(tmp.g, tmp.b));
				float density = math::Max(tmp.R, math::Max(tmp.G, tmp.B));
				avgDensity += density;	// add it up

				// calc deviation (for _uniformity)
				currentDeviation = density - avgDensity;
				totalDeviations += currentDeviation * currentDeviation;

				// fade out old
				color[i] = tmp * holdAmount;

				CHECK_ZERO(color[i].R);
				CHECK_ZERO(color[i].G);
				CHECK_ZERO(color[i].B);
				//			CHECK_ZERO(color[i].a);
				CHECK_ZERO(uv[i].x);
				CHECK_ZERO(uv[i].y);
				if(desc.doVorticityConfinement) CHECK_ZERO(curl[i]);
			}
			avgDensity *= invCellCount;
			avgSpeed *= invCellCount;

			uniformity = 1.0f / (1 + totalDeviations * invCellCount);		// 0: very wide distribution, 1: very uniform
		}
		void diffuse(int bound,float* c,float* c0,float diff,float dt)
		{
			float a = dt * diff * desc.resolution.x * desc.resolution.y;	//todo find the exact strategy for using desc.resolution.x and desc.resolution.Y in the factors
			linearSolver(bound, c, c0, a, 1.0 + 4 * a);
		}
		void advect(int b,float* c,float* old,const math::vector2d *uv,float dt)
		{
			int i0, j0, i1, j1;
			float x, y, s0, t0, s1, t1;
			int	index;

			const float dt0x = dt * desc.resolution.x;
			const float dt0y = dt * desc.resolution.y;

			for (int j = desc.resolution.y; j > 0; --j)
			{
				for (int i = desc.resolution.x; i > 0; --i)
				{
					index = FLUID_INDEX(i, j);
					x = i - dt0x * uv[index].x;
					y = j - dt0y * uv[index].y;

					if (x > desc.resolution.x + 0.5) x = desc.resolution.x + 0.5f;
					if (x < 0.5)     x = 0.5f;

					i0 = (int) x;
					i1 = i0 + 1;

					if (y > desc.resolution.y + 0.5) y = desc.resolution.y + 0.5f;
					if (y < 0.5)     y = 0.5f;

					j0 = (int) y;
					j1 = j0 + 1;

					s1 = x - i0;
					s0 = 1 - s1;
					t1 = y - j0;
					t0 = 1 - t1;

					c[index] = s0 * (t0 * old[FLUID_INDEX(i0, j0)] + t1 * old[FLUID_INDEX(i0, j1)])
						+ s1 * (t0 * old[FLUID_INDEX(i1, j0)] + t1 * old[FLUID_INDEX(i1, j1)]);

				}
			}
			SetBoundary(b, c);
		}

		void fadeDensity()
		{
			float holdAmount = 1 - desc.fadeSpeed;

			avgDensity = 0;
			avgSpeed = 0;

			float totalDeviations = 0;
			float currentDeviation;
			float tmp;
			avgSpeed = 0;

			for (int i = cellCount-1; i >=0; --i) {

				// clear old values
				oldUV[i] = math::vector2d::Zero;
				oldDensity[i] = 0;

				// calc avg speed
				avgSpeed += uv[i].x * uv[i].x + uv[i].y * uv[i].y;

				// calc avg density
				tmp = math::Min(1.0f, density[i]);
				avgDensity += tmp;	// add it up

				// calc deviation (for uniformity)
				currentDeviation = tmp - avgDensity;
				totalDeviations += currentDeviation * currentDeviation;

				// fade out old
				density[i] = tmp * holdAmount;

				CHECK_ZERO(density[i]);
				CHECK_ZERO(uv[i].x);
				CHECK_ZERO(uv[i].y);
				if(desc.doVorticityConfinement) CHECK_ZERO(curl[i]);

			}
			avgDensity *= invCellCount;
			avgSpeed *= invCellCount;

			uniformity = 1.0f / (1 + totalDeviations * invCellCount);		// 0: very wide distribution, 1: very uniform
		}

		//--------------------------------------------------------------
		//	Gauss-Seidel relaxation
		void linearSolver(int bound, float* __restrict x, const float* __restrict x0, float a, float c)
		{
			int	step_x = desc.resolution.x + 2;
			int index;
			c = 1. / c;
			for (int k = desc.solverIterations; k > 0; --k)	// MEMO
			{
				for (int j = desc.resolution.y; j > 0 ; --j)
				{
					index = FLUID_INDEX(desc.resolution.x, j);
					for (int i = desc.resolution.x; i > 0 ; --i)
					{
						x[index] = ((x[index-1] + x[index+1] + x[index - step_x] + x[index + step_x]) * a + x0[index]) * c;
						--index;
					}
				}
				SetBoundary(bound, x);
			}
		}

		//--------------------------------------------------------------
		void linearSolverProject(math::vector2d* __restrict pdiv)
		{
			int	step_x = desc.resolution.x + 2;
			int index;
			for (int k = desc.solverIterations; k > 0; --k) {
				for (int j = desc.resolution.y; j > 0 ; --j) {
					index = FLUID_INDEX(desc.resolution.x, j);
					float prev = pdiv[index+1].x;
					for (int i = desc.resolution.x; i > 0 ; --i)
					{
						prev = (pdiv[index-1].x + prev + pdiv[index - step_x].x + pdiv[index + step_x].x + pdiv[index].y) * .25;
						pdiv[index].x = prev;
						--index;
					}
				}
				SetBoundary02d(reinterpret_cast<math::vector2d*>(&pdiv[0].x));
			}
		}

		//--------------------------------------------------------------
		void linearSolverRGB(float a, float c)
		{
			int index3, index4, index;
			int	step_x = desc.resolution.x + 2;
			c = 1. / c;
			for (int k = desc.solverIterations; k > 0; --k)	// MEMO
			{           
				for (int j = desc.resolution.y; j > 0 ; --j)
				{
					index = FLUID_INDEX(desc.resolution.x, j);
					//index1 = index - 1;		//FLUID_INDEX(i-1, j);
					//index2 = index + 1;		//FLUID_INDEX(i+1, j);
					index3 = index - step_x;	//FLUID_INDEX(i, j-1);
					index4 = index + step_x;	//FLUID_INDEX(i, j+1);
					for (int i = desc.resolution.x; i > 0 ; --i)
					{	
						color[index] = ((color[index-1] + color[index+1]  +  color[index3] + color[index4]) * a  +  oldColor[index]) * c;                                
						--index;
						--index3;
						--index4;
					}
				}
				SetBoundaryRGB();	
			}
		}

		//--------------------------------------------------------------
		void linearSolverUV(float a, float c)
		{
			int index;
			int	step_x = desc.resolution.x + 2;
			c = 1. / c;
			math::vector2d* __restrict localUV = uv;
			const math::vector2d* __restrict localOldUV = oldUV;

			for (int k = desc.solverIterations; k > 0; --k)	// MEMO
			{           
				for (int j = desc.resolution.y; j > 0 ; --j)
				{
					index = FLUID_INDEX(desc.resolution.x, j);
					float prevU = localUV[index+1].x;
					float prevV = localUV[index+1].y;
					for (int i = desc.resolution.x; i > 0 ; --i)
					{
						prevU = ((localUV[index-1].x + prevU + localUV[index - step_x].x + localUV[index + step_x].x) * a  + localOldUV[index].x) * c;
						prevV = ((localUV[index-1].y + prevV + localUV[index - step_x].y + localUV[index + step_x].y) * a  + localOldUV[index].y) * c;
						localUV[index].x = prevU;
						localUV[index].y = prevV;
						--index;
					}
				}
				SetBoundary2d(1, uv);
			}
		}
	};

FluidSolver::FluidSolver()
{
	m_impl=new FluidSolverImpl(this);
}

FluidSolver::~FluidSolver()
{
	delete m_impl;
}
bool FluidSolver::IsReady()
{
	return m_impl->inited;
}

void FluidSolver::SetupFromDesc(const FluidSolverDesc& d)
{
	m_impl->SetupFromDesc(d);
}
const FluidSolverDesc& FluidSolver::GetDesc()const
{
	return m_impl->desc;
}
FluidSolverDesc& FluidSolver::GetEditDesc()
{
	return m_impl->desc;
}
void FluidSolver::Resize(int xRes,int yRes)
{
	m_impl->Resize(xRes,yRes);
}
void FluidSolver::Reset()
{
	m_impl->Reset();
}


void FluidSolver::Update(float dt)
{
	m_impl->Update(dt);
}

void FluidSolver::Clean()
{
	m_impl->Clean();
}


int FluidSolver::GetWidth()const
{
	return m_impl->GetWidth();
}
int FluidSolver::GetHeight()const
{
	return m_impl->GetHeight();
}

int FluidSolver::GetIndexForCell(int x,int y)const 
{
	return m_impl->GetIndexForCell(x,y);
}

int FluidSolver::GetIndexForPos(float x,float y)const
{
	return GetIndexForCell((int)floor(x*m_impl->GetWidth()),(int)floor(y*m_impl->GetHeight()));
}

math::vector2d FluidSolver::GetVelocityAtIndex(int idx)const
{
	return m_impl->uv[idx];
}

video::SColor FluidSolver::GetColorAtIndex(int idx)const
{
	if(m_impl->desc.doRGB)
		return m_impl->color[idx];
	return video::SColor(m_impl->density[idx],m_impl->density[idx],m_impl->density[idx],1);
}

void FluidSolver::Lock()
{
	m_impl->Lock();
}

void FluidSolver::Unlock()
{
	m_impl->Unlock();
}

void FluidSolver::AddForceAtIndex(int idx,const math::vector2d&f)
{
	m_impl->uv[idx]+=f;
}

void FluidSolver::AddColorAtIndex(int idx,const video::SColor&c)
{
	if(m_impl->desc.doRGB)
	{
		m_impl->color[idx]+=c;
		m_impl->color[idx].saturate();
	}
	else
		m_impl->density[idx]+=c.R;
}


}
}
