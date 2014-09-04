TEMPLATE=subdirs
SUBDIRS+=picojpeg src lib tests #apps
src.depends = picojpeg
lib.depends = src picojpeg
#apps.depends = lib
tests.depends = lib #apps
