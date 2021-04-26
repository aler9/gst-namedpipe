project('gst-namedpipe', 'c', version : '1.0', license : 'LGPL')

plugins_install_dir = join_paths(get_option('libdir'), 'gstreamer-1.0')

gst_dep = dependency('gstreamer-base-1.0')

plugin_c_args = ['-DHAVE_CONFIG_H']

cdata = configuration_data()
cdata.set_quoted('PACKAGE_VERSION', meson.project_version())
cdata.set_quoted('PACKAGE', meson.project_name())
cdata.set_quoted('GST_LICENSE', meson.project_license())
cdata.set_quoted('GST_PACKAGE_NAME', meson.project_name())
cdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://github.com/aler9/gst-namedpipe')
configure_file(output : 'config.h', configuration : cdata)

plugin_sources = [
    'sink.c',
    'src.c',
    'plugin.c'
    ]

plugin = library('gstnamedpipe',
    plugin_sources,
    c_args: plugin_c_args,
    dependencies : [gst_dep],
    install : true,
    install_dir : plugins_install_dir,
    )
