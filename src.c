
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "src.h"

#define gst_namedpipesrc_parent_class parent_class
G_DEFINE_TYPE (GstNamedPipeSrc, gst_namedpipesrc, GST_TYPE_PUSH_SRC);

static GstStaticPadTemplate srcfactory = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS_ANY);

enum
{
  PROP_0,
  PROP_LOCATION,
  PROP_CAPS,
};

static void
gst_namedpipesrc_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstNamedPipeSrc *src = GST_NAMEDPIPESRC (object);

  switch (prop_id) {
    case PROP_LOCATION:
      g_free (src->location);
      src->location = g_value_dup_string (value);
      break;

    case PROP_CAPS:
    {
      const GstCaps *new_caps_val = gst_value_get_caps (value);
      GstCaps *new_caps;
      GstCaps *old_caps;

      if (new_caps_val == NULL) {
        new_caps = gst_caps_new_any ();
      } else {
        new_caps = gst_caps_copy (new_caps_val);
      }

      GST_OBJECT_LOCK (src);
      old_caps = src->caps;
      src->caps = new_caps;
      GST_OBJECT_UNLOCK (src);
      if (old_caps) {
        gst_caps_unref (old_caps);
      }

      gst_pad_mark_reconfigure (GST_BASE_SRC_PAD (src));
      break;
    }

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static gboolean
gst_namedpipesrc_start (GstBaseSrc * base)
{
  GstNamedPipeSrc *src = GST_NAMEDPIPESRC (base);
  src->fd = open (src->location, O_RDWR);
  if (src->fd < 0) {
    GST_ELEMENT_ERROR (src, LIBRARY, FAILED, (NULL),
        ("unable to open named pipe"));
    return FALSE;
  }

  return TRUE;
}

static gboolean
gst_namedpipesrc_stop (GstBaseSrc * base)
{
  GstNamedPipeSrc *src = GST_NAMEDPIPESRC (base);
  close (src->fd);
  return TRUE;
}

static GstCaps *
gst_namedpipesrc_getcaps (GstBaseSrc * base, GstCaps * filter)
{
  GstNamedPipeSrc *src = GST_NAMEDPIPESRC (base);
  GstCaps *caps, *result;

  GST_OBJECT_LOCK (base);
  if ((caps = src->caps)) {
    gst_caps_ref (caps);
  }
  GST_OBJECT_UNLOCK (base);

  if (caps) {
    if (filter) {
      result = gst_caps_intersect_full (filter, caps, GST_CAPS_INTERSECT_FIRST);
      gst_caps_unref (caps);
    } else {
      result = caps;
    }
  } else {
    result = (filter) ? gst_caps_ref (filter) : gst_caps_new_any ();
  }
  return result;
}

static GstFlowReturn
gst_namedpipesrc_create (GstPushSrc * base, GstBuffer ** buf)
{
  GstNamedPipeSrc *src = GST_NAMEDPIPESRC (base);

  guint32 size = 0;
  size_t n = read (src->fd, &size, sizeof (guint32));
  if (n != sizeof (guint32)) {
    GST_ELEMENT_ERROR (src, LIBRARY, FAILED, (NULL), ("unable to read size"));
    return GST_FLOW_ERROR;
  }

  *buf = gst_buffer_new_allocate (NULL, size, NULL);

  GstMapInfo map;
  gst_buffer_map (*buf, &map, GST_MAP_WRITE);

  n = read (src->fd, map.data, map.size);
  if (n != map.size) {
    GST_ELEMENT_ERROR (src, LIBRARY, FAILED, (NULL),
        ("unable to read content"));
    return GST_FLOW_ERROR;
  }

  gst_buffer_unmap (*buf, &map);
  return GST_FLOW_OK;
}

static void
gst_namedpipesrc_class_init (GstNamedPipeSrcClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstElementClass *gstelement_class = GST_ELEMENT_CLASS (klass);
  GstBaseSrcClass *gstbasesrc_class = GST_BASE_SRC_CLASS (klass);
  GstPushSrcClass *gstpushsrc_class = GST_PUSH_SRC_CLASS (klass);

  gobject_class->set_property = gst_namedpipesrc_set_property;

  g_object_class_install_property (gobject_class, PROP_LOCATION,
      g_param_spec_string ("location", "File Location",
          "Location of the named pipe to write",
          NULL, G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_CAPS,
      g_param_spec_boxed ("caps", "Caps",
          "The caps of the source pad", GST_TYPE_CAPS,
          G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

  gst_element_class_add_static_pad_template (gstelement_class, &srcfactory);

  gstbasesrc_class->start = gst_namedpipesrc_start;
  gstbasesrc_class->stop = gst_namedpipesrc_stop;
  gstbasesrc_class->get_caps = gst_namedpipesrc_getcaps;
  gstpushsrc_class->create = GST_DEBUG_FUNCPTR (gst_namedpipesrc_create);

  gst_element_class_set_static_metadata (gstelement_class,
      "Src name", "Src", "My Src element", "The author <my.src@my.email>");
}

static void
gst_namedpipesrc_init (GstNamedPipeSrc * src)
{
}
