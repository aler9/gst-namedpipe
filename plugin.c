
#include "sink.h"
#include "src.h"

static gboolean
plugin_init (GstPlugin * plugin)
{
  if (!gst_element_register (plugin, "namedpipesink", GST_RANK_NONE,
          GST_TYPE_NAMEDPIPESINK)) {
    return FALSE;
  }

  if (!gst_element_register (plugin, "namedpipesrc", GST_RANK_NONE,
          GST_TYPE_NAMEDPIPESRC)) {
    return FALSE;
  }

  return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    namedpipe,
    "Write/read data to/from named pipes",
    plugin_init,
    PACKAGE_VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
