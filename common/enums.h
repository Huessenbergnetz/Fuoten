#ifndef ENUMS_H
#define ENUMS_H

#include <QMetaType>

class FuotenAppEnums {
    Q_GADGET
public:

    enum Context : quint8 {
        StartPage       = 0,
        Folders         = 1,
        Feeds           = 2,
        AllItems        = 3,
        FolderItems     = 4,
        FeedItems       = 5,
        StarredItems    = 6,
        SingleItem      = 7
    };
    Q_ENUM(Context)

    enum OpenIn : quint8 {
        OpenInternal    = 0,
        OpenWebView     = 1,
        OpenExternal    = 2
    };
    Q_ENUM(OpenIn)

private:
    FuotenAppEnums();
    ~FuotenAppEnums();
};

#endif // ENUMS_H
