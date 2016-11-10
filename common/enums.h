#ifndef ENUMS_H
#define ENUMS_H

#include <QMetaType>

class FuotenAppEnums {
    Q_GADGET
public:

    enum Context {
        StartPage       = 0,
        Folders         = 1,
        Feeds           = 2,
        AllItems        = 3,
        FolderItems     = 4,
        FeedItems       = 5,
        StarredItems    = 6,
        SingleItem      = 7
    };
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(Context)
#else
    Q_ENUMS(Context)
#endif

    enum OpenIn {
        OpenInternal    = 0,
        OpenWebView     = 1,
        OpenExternal    = 2
    };
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(OpenIn)
#else
    Q_ENUMS(OpenIn)
#endif

private:
    FuotenAppEnums();
    ~FuotenAppEnums();
};

#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
Q_DECLARE_METATYPE(FuotenAppEnums::Context)
Q_DECLARE_METATYPE(FuotenAppEnums::OpenIn)
#endif

#endif // ENUMS_H
