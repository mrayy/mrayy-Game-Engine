#
#include <string.h>
#include <math.h>

#include <gst/gst.h>

#define  assert(x) if(!(x))printf("Failed to %s\n",#x);

static void
	pad_added_cb( GstElement* rtpbin, GstPad* new_pad, GstElement* depay )
{
	GstPad* sinkpad;
	GstPadLinkReturn lres;
	g_print( "new payload on pad: %s\n", GST_PAD_NAME( new_pad ) );
	sinkpad = gst_element_get_static_pad( depay, "sink" );
	assert( sinkpad );
	lres = gst_pad_link( new_pad, sinkpad );
	assert( lres == GST_PAD_LINK_OK );
	gst_object_unref( sinkpad );
}


static void
	gstreamer_bus_callback( GstElement* rtpbin, GstPad* new_pad, GstElement* depay )
{
}



static void
	on_ssrc_active_cb( GstElement * rtpbin, guint sessid, guint ssrc,
	GstElement * depay )
{
}


struct Call
{
	int local_video_port;
	int remote_video_port;
	char remote_ip[256];

};

int rtp_init_video_rx( Call* call )
{
	GstElement* pipeline, *rtp_src, *rtcp_src, *rtcp_sink, *depayloader, *h264_decoder, *csp_filter, *screen_sink, *rtp_bin;
	GstPadLinkReturn lres;
	GstPad* srcpad,
		*sinkpad;
	GstCaps* caps;
	GstBus* bus;

	// Create pipeline and attach a callback to it's message bus
	pipeline = gst_pipeline_new( "pipeline0" );
	g_signal_connect( pipeline, "deep-notify", G_CALLBACK( gst_object_default_deep_notify ), NULL );
	bus = gst_pipeline_get_bus( GST_PIPELINE( pipeline ) );
	gst_bus_add_watch( bus, ( GstBusFunc )gstreamer_bus_callback, pipeline );
	gst_object_unref( GST_OBJECT( bus ) );

	// Create elements
	rtp_src                         = gst_element_factory_make( "udpsrc", "rtp_src" );
	rtcp_src                        = gst_element_factory_make( "udpsrc", "rtcp_src" );
	rtcp_sink                       = gst_element_factory_make( "udpsink", "rtcpsink" );
	depayloader                     = gst_element_factory_make( "rtph264depay", "h264_deplayloader" );
	h264_decoder            = gst_element_factory_make( "ffdec_h264", "h264_decoder" );
	csp_filter                      = gst_element_factory_make( "ffmpegcolorspace", "csp_filter" );
	screen_sink                     = gst_element_factory_make( "autovideosink", "screen_sink" );
	rtp_bin                         = gst_element_factory_make( "gstrtpbin", "rtpbin" );

	assert( rtp_src );
	assert( rtcp_src );
	assert( rtcp_sink );
	assert( depayloader );
	assert( h264_decoder );
	assert( csp_filter );
	assert( screen_sink );
	assert( rtp_bin );

#if 0
	g_object_set( G_OBJECT( rtp_src ), "sockfd", call->rtp_socket, NULL );
	g_object_set( G_OBJECT( rtp_src ), "port", ( int )call->local_video_port, NULL );
	g_object_set( G_OBJECT( rtp_src ), "multicast-group", call->local_ip, NULL );

	g_object_set( G_OBJECT( rtcp_src ), "sockfd", call->rtcp_socket, NULL );
	g_object_set( G_OBJECT( rtcp_src ), "multicast-group", call->local_ip, NULL );
	g_object_set( G_OBJECT( rtcp_sink ), "sockfd", call->rtcp_socket, NULL );
	g_object_set( G_OBJECT( rtcp_sink ), "port", call->remote_video_port+1, "host", call->remote_ip, NULL );
	g_object_set( G_OBJECT( rtcp_sink ), "async", FALSE, "sync", FALSE, NULL );
#else

	g_object_set( rtp_src, "port", call->local_video_port, NULL );
	g_object_set( G_OBJECT( rtcp_src ), "port", call->local_video_port+1, NULL );
	g_object_set( G_OBJECT( rtcp_sink ), "port", call->remote_video_port, "host", call->remote_ip, NULL );
	g_object_set( G_OBJECT( rtcp_sink ), "async", FALSE, "sync", FALSE, NULL );
#endif
	//g_object_set( G_OBJECT( rtp_bin ), "latency", 100, NULL );

	// Add elements to the pipeline. This has to be done prior to linking them
	gst_bin_add_many( GST_BIN( pipeline ), rtp_src, rtcp_src, rtcp_sink, depayloader, h264_decoder, csp_filter, screen_sink, NULL );
#if 0
	if( !gst_element_link_filtered( rtp_src, depayloader, caps ) )
	{
		assert( !"error linking CAPS\n" );
	}

	gst_caps_unref( caps );
#endif

	/* Specify the RTP stream that we expect */
	caps = gst_caps_new_simple( "application/x-rtp",
		"clock-rate", G_TYPE_INT, 90000,
		"payload", G_TYPE_INT, 96,
		"encoding-name", G_TYPE_STRING, "H264",
		NULL );
	g_object_set( rtp_src, "caps", caps, NULL );
	gst_caps_unref( caps );

	if( !gst_element_link_many( depayloader, h264_decoder, csp_filter, screen_sink, NULL ) )
	{
		assert( !"error linking elements\n" );
	}

	gst_bin_add( GST_BIN( pipeline ), rtp_bin );

	// now link all to the rtpbin, start by getting an RTP sinkpad for session 0
	srcpad = gst_element_get_static_pad( rtp_src, "src" );
	sinkpad = gst_element_get_request_pad( rtp_bin, "recv_rtp_sink_0" );
	lres = gst_pad_link( srcpad, sinkpad );
	assert( lres == GST_PAD_LINK_OK );
	gst_object_unref( srcpad );

	// get an RTCP sinkpad in session 0
	srcpad = gst_element_get_static_pad( rtcp_src, "src" );
	sinkpad = gst_element_get_request_pad( rtp_bin, "recv_rtcp_sink_0" );
	lres = gst_pad_link( srcpad, sinkpad );
	assert( lres == GST_PAD_LINK_OK );
	gst_object_unref( srcpad );
	gst_object_unref( sinkpad );

	// get an RTCP srcpad for sending RTCP back to the sender
	srcpad = gst_element_get_request_pad( rtp_bin, "send_rtcp_src_0" );
	sinkpad = gst_element_get_static_pad( rtcp_sink, "sink" );
	lres = gst_pad_link( srcpad, sinkpad );
	assert( lres == GST_PAD_LINK_OK );
	gst_object_unref( sinkpad );

	g_signal_connect( rtp_bin, "pad-added", G_CALLBACK( pad_added_cb ), depayloader );
	/* give some stats when we receive RTCP */
	g_signal_connect( rtp_bin, "on-ssrc-active", G_CALLBACK( on_ssrc_active_cb ),depayloader );

	gst_element_set_state( pipeline, GST_STATE_PLAYING );

	return TRUE;
}



int main (void)//av_play
{
	GstElement *rtpbin;
	GstElement *vrtpsrc, *vrtcpsrc, *vrtcpsink;
	GstElement *artpsrc, *artcpsrc, *artcpsink;
	GstElement *videodepay, *videodec, *videoparse, *colorspace, *deinterlacer,*videosink;//h264parse ! ffdec_h264 ! ffmpegcolorspace ! deinterlace
	GstElement *audiodepay, *audiodec, *audiores, *audioconv, *audiosink;
	GstElement *pipeline;
	GstElement *queueA,* queueV; 
	GMainLoop *loop;
	GstCaps *vcaps,*acaps;
	gboolean res,res1;
	GstPadLinkReturn lres,lres1;
	GstPad *asrcpad, *asinkpad,*vsrcpad, *vsinkpad;


	/* always init first */
	gst_init (NULL, NULL);

	Call c;
	c.remote_video_port=170;
	c.local_video_port=5000;
	strcpy(c.remote_ip,"192.168.10.170");
	rtp_init_video_rx(&c);


	/* we need to run a GLib main loop to get the messages */
	loop = g_main_loop_new (NULL, FALSE);
	g_main_loop_run (loop);

	g_print ("stopping receiver pipeline\n");
	gst_element_set_state (pipeline, GST_STATE_NULL);

	gst_object_unref (pipeline);

}