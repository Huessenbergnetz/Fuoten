#ifndef CONTEXTCONFIG_H
#define CONTEXTCONFIG_H

#include <QObject>
#include <QSettings>
#include <Fuoten/FuotenEnums>
#include "enums.h"

/*!
 * \brief The ContextConfig class.
 */
class ContextConfig : public QSettings
{
    Q_OBJECT
    /*!
     * \brief The context the configuration is used for.
     *
     * \par Access functions:
     * <TABLE><TR><TD>FuotenAppEnums::Context</TD><TD>contextType() const</TD></TR><TR><TD>void</TD><TD>setContextType(FuotenAppEnums::Context nContextType)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>contextTypeChanged(FuotenAppEnums::Context contextType)</TD></TR></TABLE>
     */
    Q_PROPERTY(FuotenAppEnums::Context contextType READ contextType WRITE setContextType NOTIFY contextTypeChanged)
    /*!
     * \brief The ID of the context.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>contextId() const</TD></TR><TR><TD>void</TD><TD>setContextId(qint64 nId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>contextIdChanged(qint64 id)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 contextId READ contextId WRITE setContextId NOTIFY contextIdChanged)
    /*!
     * \brief The role/value used for sorting.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Fuoten::FuotenEnums::SortingRole</TD><TD>sorting() const</TD></TR><TR><TD>void</TD><TD>setSorting(Fuoten::FuotenEnums::SortingRole nSorting)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>sortingChanged(Fuoten::FuotenEnums::SortingRole sorting)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::FuotenEnums::SortingRole sorting READ sorting WRITE setSorting NOTIFY sortingChanged)
    /*!
     * \brief True if read folders/feeds/articles should be hidden.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>hideRead() const</TD></TR><TR><TD>void</TD><TD>setHideRead(bool nHideRead)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>hideReadChanged(bool hideRead)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool hideRead READ hideRead WRITE setHideRead NOTIFY hideReadChanged)
    /*!
     * \brief The sort order.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Qt::SortOrder</TD><TD>sortOrder() const</TD></TR><TR><TD>void</TD><TD>setSortOrder(Qt::SortOrder nSortOrder)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>sortOrderChanged(Qt::SortOrder sortOrder)</TD></TR></TABLE>
     */
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)
    /*!
     * \brief If true, feed will be sorted by folder names.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>showFolderSections() const</TD></TR><TR><TD>void</TD><TD>setShowFolderSections(bool nShowFolderSections)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>showFolderSectionsChanged(bool showFolderSections)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool showFolderSections READ showFolderSections WRITE setShowFolderSections NOTIFY showFolderSectionsChanged)
    /*!
     * \brief True if the server side setted pinning of feeds should be respected.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>respectPinned() const</TD></TR><TR><TD>void</TD><TD>setRespectPinned(bool nRespectPinned)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>respectPinnedChanged(bool respectPinned)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool respectPinned READ respectPinned WRITE setRespectPinned NOTIFY respectPinnedChanged)
    /*!
     * \brief If true, article lists will show an excerpt of the body text.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>showExcerpt() const</TD></TR><TR><TD>void</TD><TD>setShowExcerpt(bool nShowExcerpt)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>showExcerptChanged(bool showExcerpt)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool showExcerpt READ showExcerpt WRITE setShowExcerpt NOTIFY showExcerptChanged)
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    Q_ENUMS(FuotenAppEnums::Context)
    Q_ENUMS(Fuoten::FuotenEnums::SortingRole)
#endif
public:
    explicit ContextConfig(QObject *parent = nullptr);
    ~ContextConfig();

    FuotenAppEnums::Context contextType() const;
    qint64 contextId() const;
    Fuoten::FuotenEnums::SortingRole sorting() const;
    bool hideRead() const;
    Qt::SortOrder sortOrder() const;
    bool showFolderSections() const;
    bool respectPinned() const;
    bool showExcerpt() const;


    void setContextType(FuotenAppEnums::Context nContextType);
    void setContextId(qint64 nId);
    void setSorting(Fuoten::FuotenEnums::SortingRole nSorting);
    void setHideRead(bool nHideRead);
    void setSortOrder(Qt::SortOrder nSortOrder);
    void setShowFolderSections(bool nShowFolderSections);
    void setRespectPinned(bool nRespectPinned);
    void setShowExcerpt(bool nShowExcerpt);


signals:
    void contextTypeChanged(FuotenAppEnums::Context contextType);
    void contextIdChanged(qint64 id);
    void sortingChanged(Fuoten::FuotenEnums::SortingRole sorting);
    void hideReadChanged(bool hideRead);
    void sortOrderChanged(Qt::SortOrder sortOrder);
    void showFolderSectionsChanged(bool showFolderSections);
    void respectPinnedChanged(bool respectPinned);
    void showExcerptChanged(bool showExcerpt);

private:
    Q_DISABLE_COPY(ContextConfig)
    FuotenAppEnums::Context m_contextType;
    qint64 m_contextId;
    Fuoten::FuotenEnums::SortingRole m_sorting;
    bool m_hideRead;
    Qt::SortOrder m_sortOrder;
    bool m_showFolderSections;
    bool m_respectPinned;
    bool m_showExcerpt;

    QString path(const QString &key) const;

    void load();

};

#endif // CONTEXTCONFIG_H
