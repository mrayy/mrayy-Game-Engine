

#include "stdafx.h"
#include "cMyListener.h"
#include "ILogManager.h"

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gst/gst.h>
#include <gst/base/gstpushsrc.h>
#include <gst/video/video-info.h>
#include <gio/gio.h>

GST_DEBUG_CATEGORY_STATIC(gst_my_listener_debug);
#define GST_CAT_DEFAULT gst_my_listener_debug

/* Filter signals and args */
enum
{
	/* FILL ME */
	LAST_SIGNAL
};

enum
{
	PROP_0,
	PROP_CAPS,
	PROP_SILENT
};

/* the capabilities of the inputs and outputs.
*
* describe the real formats here.
*/
static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE("sink",
	GST_PAD_SINK,
	GST_PAD_ALWAYS,
	GST_STATIC_CAPS_ANY
	);

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE("src",
	GST_PAD_SRC,
	GST_PAD_ALWAYS,
	GST_STATIC_CAPS_ANY);
/*
static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE("src",
	GST_PAD_SRC,
	GST_PAD_ALWAYS,
	GST_STATIC_CAPS("ANY")
	);*/

#define gst_my_listener_parent_class parent_class
G_DEFINE_TYPE(GstMyListener, gst_my_listener, GST_TYPE_ELEMENT);

static void gst_my_listener_set_property(GObject * object, guint prop_id,
	const GValue * value, GParamSpec * pspec);
static void gst_my_listener_get_property(GObject * object, guint prop_id,
	GValue * value, GParamSpec * pspec);

static gboolean gst_my_listener_sink_event(GstPad * pad, GstObject * parent, GstEvent * event);
static gboolean gst_my_listener_src_event(GstPad * pad, GstObject * parent, GstEvent * event);
static GstFlowReturn gst_my_listener_chain(GstPad * pad, GstObject * parent, GstBuffer * buf);

static void gst_my_listener_finalize(GObject * object);

static GstCaps *
gst_mylistener_getcaps(GstBaseSrc * src, GstCaps * caps)
{
	GstMyListener *filter = GST_MyListener(src);
	/*
	if (!udpsrc->caps)
	{
		udpsrc->caps = gst_caps_new_any();
	}*/

	return gst_caps_ref(filter->caps);

}

static gboolean
gst_mylistener_setcaps(GstBaseSrc * src, GstCaps * caps)
{
	GstMyListener *filter = GST_MyListener(src);
	gst_pad_set_caps(filter->srcpad, caps);

	return true;

}
/* GObject vmethod implementations */

/* initialize the MyListener's class */
static void
gst_my_listener_class_init(GstMyListenerClass * klass)
{
	GObjectClass *gobject_class;
	GstElementClass *gstelement_class;
	GstBaseSrcClass *gstbasesrc_class;

	gobject_class = (GObjectClass *)klass;
	gstelement_class = (GstElementClass *)klass;
	gstbasesrc_class = (GstBaseSrcClass *)klass;

	gobject_class->set_property = gst_my_listener_set_property;
	gobject_class->get_property = gst_my_listener_get_property;
	gobject_class->finalize = gst_my_listener_finalize;

	gstbasesrc_class->get_caps = gst_mylistener_getcaps;
	gstbasesrc_class->set_caps = gst_mylistener_setcaps;

	g_object_class_install_property(gobject_class, PROP_SILENT,
		g_param_spec_boolean("silent", "Silent", "Produce verbose output ?",
		FALSE, G_PARAM_READWRITE));

	gst_element_class_set_details_simple(gstelement_class,
		"MyListener",
		"Source",
		"Simple Listener",
		"mrayyamen@gmail.com");

	g_object_class_install_property(gobject_class, PROP_CAPS,
		g_param_spec_boxed("caps", "Caps",
		"The caps of the source pad", GST_TYPE_CAPS,
		GParamFlags(G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));

	gst_element_class_add_pad_template(gstelement_class,
		gst_static_pad_template_get(&src_factory));
	gst_element_class_add_pad_template(gstelement_class,
		gst_static_pad_template_get(&sink_factory));
}

/* initialize the new element
* instantiate pads and add them to element
* set pad calback functions
* initialize instance structure
*/
static void
gst_my_listener_init(GstMyListener * filter)
{
	filter->sinkpad = gst_pad_new_from_static_template(&sink_factory, "sink");
	gst_pad_set_event_function(filter->sinkpad,
		GST_DEBUG_FUNCPTR(gst_my_listener_sink_event));
	gst_pad_set_chain_function(filter->sinkpad,
		GST_DEBUG_FUNCPTR(gst_my_listener_chain));
	GST_PAD_SET_PROXY_CAPS(filter->sinkpad);
	gst_element_add_pad(GST_ELEMENT(filter), filter->sinkpad);

	filter->srcpad = gst_pad_new_from_static_template(&src_factory, "src");
	//GST_OBJECT_FLAG_SET(filter->srcpad, GST_PAD_FLAG_FIXED_CAPS);
	GST_PAD_SET_PROXY_CAPS(filter->sinkpad);
	//gst_pad_use_fixed_caps(filter->srcpad);
	gst_pad_set_event_function(filter->srcpad,
		GST_DEBUG_FUNCPTR(gst_my_listener_src_event));
	gst_element_add_pad(GST_ELEMENT(filter), filter->srcpad);
	/*
	GstCaps* caps = gst_caps_new_simple("video/x-raw",
		"format", G_TYPE_STRING, "RGB",
		"width", G_TYPE_INT, 1920,
		"height", G_TYPE_INT, 950, NULL);
	if (!gst_pad_set_caps(filter->srcpad, caps)) {
		gLogManager.log("Failed to set caps", mray::ELogLevel::ELL_WARNING);
// 		GST_ELEMENT_ERROR(element, CORE, NEGOTIATION, (NULL),
// 			("Some debug information here"));
// 		return GST_FLOW_ERROR;
	}*/



	filter->silent = FALSE;

	filter->listeners = new MyListenerContainer();
}


static void
gst_my_listener_finalize(GObject * object)
{
	GstMyListener *sink;

	sink = GST_MyListener(object);

	if (sink->caps)
		gst_caps_unref(sink->caps);
	sink->caps = NULL;
	if (sink->listeners)
	{
		delete sink->listeners;
	}
	G_OBJECT_CLASS(parent_class)->finalize(object);
}


static void
gst_my_listener_set_property(GObject * object, guint prop_id,
const GValue * value, GParamSpec * pspec)
{
	GstMyListener *filter = GST_MyListener(object);

	switch (prop_id) {
	case PROP_SILENT:
		filter->silent = g_value_get_boolean(value);
		break;
	case PROP_CAPS:
	{
		const GstCaps *new_caps_val = gst_value_get_caps(value);

		GstCaps *new_caps;

		GstCaps *old_caps;

		if (new_caps_val == NULL) {
			new_caps = gst_caps_new_any();
		}
		else {
			new_caps = gst_caps_copy(new_caps_val);
		}

		GstVideoInfo ifo;
		gst_video_info_init(&ifo);
		gst_video_info_from_caps(&ifo, new_caps);

		old_caps = filter->caps;
		filter->caps = new_caps;
		if (old_caps)
			gst_caps_unref(old_caps);
		//		gst_pad_set_caps(filter->srcpad, new_caps);
// 		else
// 			GST_PAD_UNSET_PROXY_CAPS(filter->srcpad);
 //		gst_pad_use_fixed_caps(filter->srcpad);
		
	//	GstPad* pads=gst_element_get_static_pad(&filter->element, "sink");
		if (!gst_pad_set_caps(filter->srcpad, filter->caps))
		{
			GST_ELEMENT_ERROR(filter, CORE, NEGOTIATION, (NULL),
				("Failed to set pads for mylistener"));
			//	gLogManager.log("Failed to set pads for mylistener", mray::ELL_WARNING);
		}/**/
		break;
	}
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}

static void
gst_my_listener_get_property(GObject * object, guint prop_id,
GValue * value, GParamSpec * pspec)
{
	GstMyListener *filter = GST_MyListener(object);

	switch (prop_id) {
	case PROP_SILENT:
		g_value_set_boolean(value, filter->silent);
		break;
	case PROP_CAPS:
		gst_value_set_caps(value, filter->caps);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}

/* GstElement vmethod implementations */
static gboolean
gst_my_filter_setcaps(GstMyListener *filter,
	GstCaps *caps)
{
	if (gst_pad_set_caps(filter->srcpad, caps)) {
		//filter->passthrough = TRUE;
	}	else {
	}

	return true;
}
/* this function handles sink events */
static gboolean
gst_my_listener_src_event(GstPad * pad, GstObject * parent, GstEvent * event)
{
	gboolean ret;
	GstMyListener *filter;

	filter = GST_MyListener(parent);

	switch (GST_EVENT_TYPE(event)) {
	case GST_EVENT_CAPS:
	{
						   GstCaps * caps;
						   gst_event_parse_caps(event, &caps);
						   /* do something with the caps */

						   /* and forward */
						   ret = gst_my_filter_setcaps(filter , caps);// gst_pad_event_default(pad, parent, event);
						   break;
	}
	default:
		ret = gst_pad_push_event(filter->sinkpad, event);
		break;
	}
	return ret;
}
/* this function handles sink events */
static gboolean
gst_my_listener_sink_event(GstPad * pad, GstObject * parent, GstEvent * event)
{
	gboolean ret;
	GstMyListener *filter;

	filter = GST_MyListener(parent);

	switch (GST_EVENT_TYPE(event)) {
	case GST_EVENT_CAPS:
	{
						   GstCaps * caps;
						   gst_event_parse_caps(event, &caps);
						   /* do something with the caps */

						   /* and forward */
						   ret =  gst_pad_event_default(pad, parent, event);
						   break;
	}
	default:
		ret = gst_pad_event_default(pad, parent, event);
		break;
	}
	return ret;
}

/* chain function
* this function does the actual processing
*/
static GstFlowReturn
gst_my_listener_chain(GstPad * pad, GstObject * parent, GstBuffer * buf)
{
	GstMyListener *filter;

	filter = GST_MyListener(parent);

	if (filter->silent == FALSE)
	{
		filter->listeners->__FIRE_ListenerOnDataChained(filter, buf);
	}


	/* just push out the incoming buffer without touching it */
	return gst_pad_push(filter->srcpad, buf);
}


/* entry point to initialize the plug-in
* initialize the plug-in itself
* register the element factories and other features
*/
static gboolean
MyListener_init(GstPlugin * MyListener)
{
	/* debug category for fltering log messages
	*
	* exchange the string 'Template MyListener' with your description
	*/
	GST_DEBUG_CATEGORY_INIT(gst_my_listener_debug, "MyListener",
		0, "Template MyListener");

	return gst_element_register(MyListener, "MyListener", GST_RANK_NONE,
		GST_TYPE_MyListener);
}

/* PACKAGE: this is usually set by autotools depending on some _INIT macro
* in configure.ac and then written into and defined in config.h, but we can
* just set it ourselves here in case someone doesn't use autotools to
* compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
*/
#ifndef PACKAGE
#define PACKAGE "MyListener"
#endif

/* gstreamer looks for this structure to register MyListeners
*
* exchange the string 'Template MyListener' with your MyListener description
*/
/*
GST_PLUGIN_DEFINE(
	GST_VERSION_MAJOR,
	GST_VERSION_MINOR,
	MyListener,
	"Template MyListener",
	MyListener_init,
	"1.0",
	"LGPL",
	"GStreamer",
	"http://gstreamer.net/"
	)
	
	*/

/*** GSTURIHANDLER INTERFACE *************************************************/

static GstURIType
gst_udpsrc_uri_get_type(GType type)
{
	return GST_URI_SRC;
}
gboolean
_GstMyListenerClass::plugin_init(GstPlugin * plugin)
{
	if (!gst_element_register(plugin, "mylistener", GST_RANK_NONE,
		GST_TYPE_MyListener))
		return FALSE;

	return TRUE;
}