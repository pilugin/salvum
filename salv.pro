TEMPLATE=subdirs
SUBDIRS+=picojpeg src lib tests tools #apps
src.depends = picojpeg
lib.depends = src picojpeg
#apps.depends = lib
tools.depends = lib
tests.depends = lib #apps
