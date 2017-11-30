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
    /*!
     * \brief The way articles should be opened.
     *
     * \par Access functions:
     * <TABLE><TR><TD>FuotenAppEnums::OpenIn</TD><TD>openArticles() const</TD></TR><TR><TD>void</TD><TD>setOpenArticles(FuotenAppEnums::OpenIn nOpenArticles)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>openArticlesChanged(FuotenAppEnums::OpenIn openArticles)</TD></TR></TABLE>
     */
    Q_PROPERTY(FuotenAppEnums::OpenIn openArticles READ openArticles WRITE setOpenArticles NOTIFY openArticlesChanged)
    /*!
     * \brief Deletion strategy for items per feed.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Fuoten::FuotenEnums::ItemDeletionStrategy</TD><TD>deletionStrategy() const</TD></TR><TR><TD>void</TD><TD>setDeletionStrategy(Fuoten::FuotenEnums::ItemDeletionStrategy nDeletionStrategy)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>deletionStrategyChanged(Fuoten::FuotenEnums::ItemDeletionStrategy deletionStrategy)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::FuotenEnums::ItemDeletionStrategy deletionStrategy READ deletionStrategy WRITE setDeletionStrategy NOTIFY deletionStrategyChanged)
    /*!
     * \brief Value for the deletion strategy for items per feed.
     *
     * \par Access functions:
     * <TABLE><TR><TD>quint16</TD><TD>deletionValue() const</TD></TR><TR><TD>void</TD><TD>setDeletionValue(quint16 nDeletionValue)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>deletionValueChanged(quint16 deletionValue)</TD></TR></TABLE>
     */
    Q_PROPERTY(quint16 deletionValue READ deletionValue WRITE setDeletionValue NOTIFY deletionValueChanged)
    /*!
     * \brief Holds the model index of the currently selected user agent.
     *
     * \par Access functions:
     * \li quint8 userAgentIdx() const
     * \li void setUserAgentIdx(quint8 nUserAgentIdx)
     *
     * \par Notifier signal:
     * \li void userAgentIdxChanged(quint8 userAgentIdx)
     */
    Q_PROPERTY(quint8 userAgentIdx READ userAgentIdx WRITE setUserAgentIdx NOTIFY userAgentIdxChanged)
    /*!
     * \brief Holds the currently used user agent string.
     *
     * \par Access functions:
     * \li QString userAgent() const
     * \li void setUserAgent(const QString &nUserAgent)
     *
     * \par Notifier signal:
     * \li void userAgentChanged(const QString &userAgent)
     */
    Q_PROPERTY(QString userAgent READ userAgent WRITE setUserAgent NOTIFY userAgentChanged)
    /*!
     * \brief Holds the minimum font size for the internal web view.
     *
     * \par Access functions:
     * \li int minimumFontSize() const
     * \li void setMinimumFontSize(int minimumFontSize)
     *
     * \par Notifier signal:
     * \li void minimumFontSizeChanged(int minimumFontSize)
     */
    Q_PROPERTY(int minimumFontSize READ minimumFontSize WRITE setMinimumFontSize NOTIFY minimumFontSizeChanged)
    /*!
     * \brief Holds the default font size for the internal web view.
     *
     * \par Access functions:
     * \li int defaultFontSize() const
     * \li void setDefaultFontSize(int defaultFontSize)
     *
     * \par Notifier signal:
     * \li void defaultFontSizeChanged(int defaultFontSize)
     */
    Q_PROPERTY(int defaultFontSize READ defaultFontSize WRITE setDefaultFontSize NOTIFY defaultFontSizeChanged)
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
    /*!
     * \brief Getter function for the \link ContextConfig::openArticles openArticles \endlink property.
     * \sa ContextConfig::setOpenArticles(), ContextConfig::openArticlesChanged()
     */
    FuotenAppEnums::OpenIn openArticles() const;
    /*!
     * \brief Getter function for the \link ContextConfig::deletionStrategy deletionStrategy \endlink property.
     * \sa ContextConfig::setDeletionStrategy(), ContextConfig::deletionStrategyChanged()
     */
    Fuoten::FuotenEnums::ItemDeletionStrategy deletionStrategy() const;
    /*!
     * \brief Getter function for the \link ContextConfig::deletionValue deletionValue \endlink property.
     * \sa ContextConfig::setDeletionValue(), ContextConfig::deletionValueChanged()
     */
    quint16 deletionValue() const;
    /*!
     * \brief Getter function for the \link ContextConfig::userAgentIdx userAgentIdx \endlink property.
     * \sa setUserAgentIdx(), userAgentIdxChanged()
     */
    quint8 userAgentIdx() const;
    /*!
     * \brief Getter function for the \link ContextConfig::userAgent userAgent \endlink property.
     * \sa setUserAgent(), userAgentChanged()
     */
    QString userAgent() const;
    /*!
     * \brief Getter function for the \link ContextConfig::minimumFontSize minimumFontSize \endlink property.
     * \sa setMinimumFontSize(), minimumFontSizeChanged()
     */
    int minimumFontSize() const;
    /*!
     * \brief Getter function for the \link ContextConfig::defaultFontSize defaultFontSize \endlink property.
     * \sa setDefaultFontSize(), defaultFontSizeChanged()
     */
    int defaultFontSize() const;


    void setContextType(FuotenAppEnums::Context nContextType);
    void setContextId(qint64 nId);
    void setSorting(Fuoten::FuotenEnums::SortingRole nSorting);
    void setHideRead(bool nHideRead);
    void setSortOrder(Qt::SortOrder nSortOrder);
    void setShowFolderSections(bool nShowFolderSections);
    void setRespectPinned(bool nRespectPinned);
    void setShowExcerpt(bool nShowExcerpt);
    /*!
     * \brief Setter function for the \link ContextConfig::openArticles openArticles \endlink property.
     * Emits the openArticlesChanged() signal if \a nOpenArticles is not equal to the stored value.
     * \sa ContextConfig::openArticles(), ContextConfig::openArticlesChanged()
     */
    void setOpenArticles(FuotenAppEnums::OpenIn nOpenArticles);
    /*!
     * \brief Setter function for the \link ContextConfig::deletionStrategy deletionStrategy \endlink property.
     * Emits the deletionStrategyChanged() signal if \a nDeletionStrategy is not equal to the stored value.
     * \sa ContextConfig::deletionStrategy(), ContextConfig::deletionStrategyChanged()
     */
    void setDeletionStrategy(Fuoten::FuotenEnums::ItemDeletionStrategy nDeletionStrategy);
    /*!
     * \brief Setter function for the \link ContextConfig::deletionValue deletionValue \endlink property.
     * Emits the deletionValueChanged() signal if \a nDeletionValue is not equal to the stored value.
     * \sa ContextConfig::deletionValue(), ContextConfig::deletionValueChanged()
     */
    void setDeletionValue(quint16 nDeletionValue);
    /*!
     * \brief Setter function for the \link ContextConfig::userAgentIdx userAgentIdx \endlink property.
     * \sa userAgentIdx(), userAgentIdxChanged()
     */
    void setUserAgentIdx(quint8 nUserAgentIdx);
    /*!
     * \brief Setter function for the \link ContextConfig::userAgent userAgent \endlink property.
     * \sa userAgent(), userAgentChanged()
     */
    void setUserAgent(const QString &nUserAgent);
    /*!
     * \brief Setter function for the \link ContextConfig::minimumFontSize minimumFontSize \endlink property.
     * \sa minimumFontSize(), minimumFontSizeChanged()
     */
    void setMinimumFontSize(int minimumFontSize);
    /*!
     * \brief Setter function for the \link ContextConfig::defaultFontSize defaultFontSize \endlink property.
     * \sa defaultFontSize(), defaultFontSizeChanged()
     */
    void setDefaultFontSize(int defaultFontSize);


signals:
    void contextTypeChanged(FuotenAppEnums::Context contextType);
    void contextIdChanged(qint64 id);
    void sortingChanged(Fuoten::FuotenEnums::SortingRole sorting);
    void hideReadChanged(bool hideRead);
    void sortOrderChanged(Qt::SortOrder sortOrder);
    void showFolderSectionsChanged(bool showFolderSections);
    void respectPinnedChanged(bool respectPinned);
    void showExcerptChanged(bool showExcerpt);
    /*!
     * \brief This is emitted if the value of the \link ContextConfig::openArticles openArticles \endlink property changes.
     * \sa ContextConfig::openArticles(), ContextConfig::setOpenArticles()
     */
    void openArticlesChanged(FuotenAppEnums::OpenIn openArticles);
    /*!
     * \brief This is emitted if the value of the \link ContextConfig::deletionStrategy deletionStrategy \endlink property changes.
     * \sa ContextConfig::deletionStrategy(), ContextConfig::setDeletionStrategy()
     */
    void deletionStrategyChanged(Fuoten::FuotenEnums::ItemDeletionStrategy deletionStrategy);
    /*!
     * \brief This is emitted if the value of the \link ContextConfig::deletionValue deletionValue \endlink property changes.
     * \sa ContextConfig::deletionValue(), ContextConfig::setDeletionValue()
     */
    void deletionValueChanged(quint16 deletionValue);
    /*!
     * \brief Notifier signal for the \link ContextConfig::userAgentIdx userAgentIdx \endlink property.
     * \sa userAgentIdx(), setUserAgentIdx()
     */
    void userAgentIdxChanged(quint8 userAgentIdx);
    /*!
     * \brief Notifier signal for the \link ContextConfig::userAgent userAgent \endlink property.
     * \sa setUserAgent(), userAgent()
     */
    void userAgentChanged(const QString &userAgent);
    /*!
     * \brief Notifier signal for the \link ContextConfig::minimumFontSize minimumFontSize \endlink property.
     * \sa setMinimumFontSize(), minimumFontSize()
     */
    void minimumFontSizeChanged(int minimumFontSize);
    /*!
     * \brief Notifier signal for the \link ContextConfig::defaultFontSize defaultFontSize \endlink property.
     * \sa setDefaultFontSize(), defaultFontSize()
     */
    void defaultFontSizeChanged(int defaultFontSize);

private:
    Q_DISABLE_COPY(ContextConfig)
    qint64 m_contextId;
    QString m_userAgent = QStringLiteral("Mozilla/5.0 (Maemo; Linux; U; Jolla; Sailfish; like Android) AppleWebKit/538.1 (KHTML, like Gecko) Version/5.1 Chrome/30.0.0.0 Mobile Safari/538.1 (compatible)");
    int m_minimumFontSize = 0;
    int m_defaultFontSize = 0;
    Qt::SortOrder m_sortOrder;
    quint16 m_deletionValue;
    quint8 m_userAgentIdx = 0;
    Fuoten::FuotenEnums::ItemDeletionStrategy m_deletionStrategy;
    Fuoten::FuotenEnums::SortingRole m_sorting;
    FuotenAppEnums::OpenIn m_openArticles;
    FuotenAppEnums::Context m_contextType;
    bool m_hideRead;
    bool m_showFolderSections;
    bool m_respectPinned;
    bool m_showExcerpt;

    QString path(const QString &key) const;

    void load();

};

#endif // CONTEXTCONFIG_H
