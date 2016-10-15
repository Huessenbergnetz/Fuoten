TEMPLATE = subdirs

SUBDIRS += libfuoten

contains(CONFIG, sfos) {

SUBDIRS += sailfishos

sailfishos.depends = libfuoten

OTHER_FILES += rpm/sailfishos.changes \
    rpm/harbour-fuoten.spec \
    rpm/sailfishos.yaml
}
