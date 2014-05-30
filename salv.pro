TEMPLATE=subdirs
SUBDIRS+=picojpeg src tests apps
src.depends = picojpeg
tests.depends = src
apps.depends = src
