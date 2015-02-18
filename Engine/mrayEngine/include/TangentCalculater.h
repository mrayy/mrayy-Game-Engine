
/********************************************************************
	created:	2008/10/28
	created:	28:10:2008   19:11
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\TangentCalculater.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	TangentCalculater
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef ___TangentCalculater___
#define ___TangentCalculater___

#include "IMeshBuffer.h"
#include "SMesh.h"

namespace mray
{
namespace scene
{

class MRAY_DLL TangentCalculater
{
public:
	// helper to get order for CVertexLoadHelper
	struct CBaseIndexOrder: public std::binary_function< uint, uint, bool>
	{
		bool operator() ( const uint &a, const uint &b ) const
		{
			if(a<b)return true;
			if(a>b)return false;

			return false;
		}
	};
	class CBase33
	{
	public:
		CBase33(){}
		CBase33(const math::vector3d& u,const math::vector3d& v,const math::vector3d& n):
		U(u),V(v),N(n)
		{
		}
		math::vector3d U;
		math::vector3d V;
		math::vector3d N;
	};
	class CTriBaseIndex
	{
	public:
		uint p[3];
	};

	class CRedundantVerts
	{
	public:
		CRedundantVerts(){}
		CRedundantVerts(int i,int n):idx(i),newIdx(n)
		{
		}
		int idx;
		int newIdx;
	};

	std::vector<CBase33> m_baseVecs;
	std::vector<CRedundantVerts> m_redundantVerts;
	std::vector<CTriBaseIndex> m_triBaseAssigment;

	bool m_generateBinormal;

	uint _AddUV2Base(std::multimap<uint,uint,CBaseIndexOrder> &inMap, const uint indwPosNo,  const math::vector3d &inU, const math::vector3d &inV, const math::vector3d &inNormN );
	void _AddNormal2Base( std::multimap<uint,uint,CBaseIndexOrder> &inMap, const uint indwPosNo, const math::vector3d &inNormal );

	float _CalcAngleBetween(const math::vector3d&a,const math::vector3d&b);

	void _RecreateBuffers(IMeshBuffer* buffer);

#if 0
	void pass1(IMeshBuffer* buffer);
	void pass2(IMeshBuffer* buffer);

	void calcTangent(const math::vector3d&v1,const math::vector3d&v2,const math::vector3d&v3,
		const math::vector2d&tc1,const math::vector2d&tc2,const math::vector2d&tc3,int i1,int i2,int i3);
#endif
public:
	TangentCalculater(bool generateBinormal);
	virtual~TangentCalculater();

	void GetTriangleBaseInd(uint id,uint outInd[3]);
	void GetBase(const uint indwPosNo ,math::vector3d&U,math::vector3d&V,math::vector3d&N);

	void calculateTangents(IMeshBuffer* buffer);
	void calculateTangents(SMesh* mesh);
};

}
}


#endif //___TangentCalculater___


