

#include "stdafx.h"
#include "ConnectionLineRenderer.h"

#include "MeshBufferData.h"
#include "IMeshBuffer.h"

namespace mray
{
namespace scene
{

ConnectionLineRenderer::ConnectionLineRenderer() :MeshRenderableNode(0)
{
	m_dirty = false;
	m_buffer = m_mesh->addNewBuffer();
	m_buffer->getMeshBuffer()->setRenderType(video::MR_LINES);
	m_buffer->getMeshBuffer()->createStream(0, video::EMST_Position, video::ESDT_Point3f, 2, video::IHardwareBuffer::EUT_Dynamic, true, true);
	m_buffer->getMeshBuffer()->createStream(0, video::EMST_Color, video::ESDT_Point4f, 2, video::IHardwareBuffer::EUT_Dynamic, true, true);
	m_buffer->getMeshBuffer()->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit, 2, video::IHardwareBuffer::EUT_Dynamic, true, true);
}

ConnectionLineRenderer::~ConnectionLineRenderer()
{
}


void ConnectionLineRenderer::Reset()
{
	m_connections.clear();
	m_dirty = true;
}

void ConnectionLineRenderer::AddConnection(const math::vector3d& a, const math::vector3d& b, const video::SColor& aclr, const video::SColor& bclr)
{
	ConnectionIfo c;
	c.start = a;
	c.end = b;
	c.clrs[0] = aclr;
	c.clrs[1] = bclr;
	m_connections.push_back(c);
	m_dirty = true;
}


void ConnectionLineRenderer::update(float dt)
{
	if (!m_dirty)
		return;
	m_dirty = false;


	scene::IMeshBuffer* buff = m_buffer->getMeshBuffer();
	video::IHardwareStreamBuffer* posStream = buff->getStream(0, video::EMST_Position);
	video::IHardwareStreamBuffer* clrStream = buff->getStream(0, video::EMST_Color);
	video::IndexData* indexD = buff->getIndexData();

	int count = m_connections.size() * 2;
	posStream->SetLength(count);
	clrStream->SetLength(count);
	indexD->indexCount = count;
	indexD->indexBuffer->resize(count*sizeof(ushort));
	math::vector3d* posPtr = (math::vector3d*)posStream->lock(0, 0, video::IHardwareBuffer::ELO_Discard);
	video::SColor* clrPtr = (video::SColor*)clrStream->lock(0, 0, video::IHardwareBuffer::ELO_Discard);
	ushort* indPtr = (ushort*)indexD->indexBuffer->lock(0, 0, video::IHardwareBuffer::ELO_Discard);


	std::list<ConnectionIfo>::iterator it = m_connections.begin();
	for (ushort i = 0; it != m_connections.end(); ++it, i += 2)
	{
		*posPtr++ = (*it).start;
		*posPtr++ = (*it).end;
		*clrPtr++ = (*it).clrs[0];
		(*clrPtr++) = (*it).clrs[1];
		*indPtr++ = i;
		*indPtr++ = i + 1;
	}
	posStream->unlock();
	clrStream->unlock();
	indexD->indexBuffer->unlock();
}

}
}

