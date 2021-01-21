
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "gstnamedpipesink.h"

#define gst_namedpipesink_parent_class parent_class
G_DEFINE_TYPE (GstNamedPipeSink, gst_namedpipesink, GST_TYPE_BASE_SINK);

static GstStaticPadTemplate sinkfactory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS_ANY);

enum
{
  PROP_0,
  PROP_LOCATION,
};

static void
gst_namedpipesink_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstNamedPipeSink *location = GST_NAMEDPIPESINK (object);

  switch (prop_id) {
    case PROP_LOCATION:
      g_free (location->location);
      location->location = g_value_dup_string (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static GstFlowReturn
gst_namedpipesink_sink_render (GstBaseSink * base, GstBuffer * buffer)
{
  GstNamedPipeSink *sink = GST_NAMEDPIPESINK (base);

  if (sink->fd < 0) {
    int ret = mkfifo (sink->location, 0666);
    if (ret < 0) {
      g_print ("unable to call mkfifo\n");
      return GST_FLOW_EOS;
    }

    sink->fd = open (sink->location, O_WRONLY);
    if (sink->fd < 0) {
      g_print ("unable to open file\n");
      return GST_FLOW_EOS;
    }
  }

  GstMapInfo map;
  gst_buffer_map (buffer, &map, GST_MAP_READ);

  guint32 size = (guint32) map.size;
  write (sink->fd, &size, sizeof (size));
  write (sink->fd, map.data, map.size);

  gst_buffer_unmap (buffer, &map);
  return GST_FLOW_OK;
}

static void
gst_namedpipesink_class_init (GstNamedPipeSinkClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstElementClass *gstelement_class = (GstElementClass *) klass;
  GstBaseSinkClass *gstbasesink_class = GST_BASE_SINK_CLASS (klass);

  gobject_class->set_property = gst_namedpipesink_set_property;

  g_object_class_install_property (gobject_class, PROP_LOCATION,
      g_param_spec_string ("location", "File Location",
          "Location of the file to write",
          NULL, G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

  gst_element_class_add_static_pad_template (gstelement_class, &sinkfactory);

  gstbasesink_class->render = GST_DEBUG_FUNCPTR (gst_namedpipesink_sink_render);

  gst_element_class_set_static_metadata (gstelement_class,
      "Sink name", "Sink", "My Sink element", "The author <my.sink@my.email>");
}

static void
gst_namedpipesink_init (GstNamedPipeSink * sink)
{
  sink->fd = -1;
}

static gboolean
plugin_init (GstPlugin * plugin)
{
  return gst_element_register (plugin, "namedpipesink",
      GST_RANK_NONE, GST_TYPE_NAMEDPIPESINK);
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    namedpipesink,
    "Write incoming data to fifo or named pipes",
    plugin_init,
    PACKAGE_VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
