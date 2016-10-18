TEMPLATE = subdirs

SUBDIRS += libfuoten

sfos {

SUBDIRS += sailfishos BT_SFOS_Components

sailfishos.depends = libfuoten

OTHER_FILES += rpm/sailfishos.changes \
    rpm/harbour-fuoten.spec \
    rpm/sailfishos.yaml
}
