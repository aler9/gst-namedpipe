
#ifndef __GST_NAMEDPIPESRC_H__
#define __GST_NAMEDPIPESRC_H__

#include <gst/gst.h>
#include <gst/base/gstpushsrc.h>

#include "config.h"

G_BEGIN_DECLS
/* */
    GType gst_namedpipesrc_get_type ();
#define GST_TYPE_NAMEDPIPESRC (gst_namedpipesrc_get_type ())
#define GST_NAMEDPIPESRC(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj),GST_TYPE_NAMEDPIPESRC,GstNamedPipeSrc))
/* */
typedef struct
{
  GstPushSrc parent;

  /* properties */
  gchar *location;
  GstCaps *caps;

  int fd;

} GstNamedPipeSrc;

typedef struct
{
  GstPushSrcClass parent_class;
} GstNamedPipeSrcClass;

G_END_DECLS
/* */
#endif /* __GST_NAMEDPIPESRC_H__ */
