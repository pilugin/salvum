TEMPLATE=subdirs
SUBDIRS+=picojpeg src lib tests #apps
src.depends = picojpeg
lib.depends = src picojpeg
#apps.depends = src
tests.depends = src #apps
