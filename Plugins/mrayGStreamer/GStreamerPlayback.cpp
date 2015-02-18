

#include "stdafx.h"
#include "GStreamerPlayback.h"




namespace mray
{
namespace video
{
	/*

class GStreamerPlaybackImpl
{
protected:
	core::string m_pipeLineDesc;
	GstElement* m_pipeLine;
	GstBus* m_bus;
	GstMessage* m_msg;
public:
	GStreamerPlaybackImpl();
	virtual~GStreamerPlaybackImpl()
	{
	}

	void SetPipeline(const core::string& desc)
	{
		m_pipeLineDesc=desc;
		GError *e=0;
		m_pipeLine= gst_parse_launch(desc.c_str(),&e);
	}

	void Release()
	{
		gst_element_set_state(m_pipeLine,GST_STATE_NULL);
		gst_object_unref(m_pipeLine);
	}
};*/

}
}


