TEMPLATE=subdirs
SUBDIRS+=picojpeg src #tests apps
src.depends = picojpeg
#apps.depends = src
#tests.depends = src apps
