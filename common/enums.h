#ifndef ENUMS_H
#define ENUMS_H

#include <QMetaType>

class FuotenAppEnums {
    Q_GADGET
public:

    enum Context {
        StartPage   = 0,
        Folders     = 1,
        Feeds       = 2,
        Items       = 3,
        SingleItem  = 4
    };
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(Context)
#else
    Q_ENUMS(Context)
#endif

private:
    FuotenAppEnums();
    ~FuotenAppEnums();
};

#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
Q_DECLARE_METATYPE(FuotenAppEnums::Context)
#endif

#endif // ENUMS_H
