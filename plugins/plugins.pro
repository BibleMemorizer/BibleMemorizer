include(plugin_vars)
SUBDIRS += lang_en 
CONFIG += warn_on
TEMPLATE = subdirs
sword{
	SUBDIRS += crosswire
}
