TEMPLATE = subdirs

sfos {

SUBDIRS += libfuoten
SUBDIRS += sailfishos

sailfishos.depends = libfuoten

OTHER_FILES += rpm/harbour-fuoten.changes \
    rpm/harbour-fuoten.spec \
    rpm/harbour-fuoten.yaml
}

kirigami {

SUBDIRS += kirigami

}
