
#ifndef __GST_NAMEDPIPESINK_H__
#define __GST_NAMEDPIPESINK_H__

#include <gst/gst.h>
#include <gst/base/gstbasesink.h>

#include "config.h"

G_BEGIN_DECLS
/* */
GType gst_namedpipesink_get_type ();
#define GST_TYPE_NAMEDPIPESINK (gst_namedpipesink_get_type ())
#define GST_NAMEDPIPESINK(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj),GST_TYPE_NAMEDPIPESINK,GstNamedPipeSink))
/* */
typedef struct
{
  GstBaseSink parent;

  /* properties */
  gchar *location;

  int fd;

} GstNamedPipeSink;

typedef struct
{
  GstBaseSinkClass parent_class;
} GstNamedPipeSinkClass;

G_END_DECLS
/* */
#endif /* __GST_NAMEDPIPESINK_H__ */
