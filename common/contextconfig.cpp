#include "contextconfig.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

/*!
 * \brief Constructs a new ContextConfig object.
 */
ContextConfig::ContextConfig(QObject *parent) :
    QSettings(parent)
{
    m_contextType = FuotenAppEnums::StartPage;
    m_contextId = -1;
    m_sorting = (Fuoten::FuotenEnums::SortingRole)value(path(QStringLiteral("sorting")), Fuoten::FuotenEnums::Name).toInt();
    m_hideRead = value(path(QStringLiteral("hideRead")), false).toBool();
    m_sortOrder = (Qt::SortOrder)value(path(QStringLiteral("sortOrder")), Qt::AscendingOrder).toInt();
    m_showFolderSections = value(path(QStringLiteral("showFolderSections")), false).toBool();
    m_respectPinned = value(path(QStringLiteral("respectPinned")), true).toBool();
}


/*!
 * \brief Deconstructs the ContextConfig object.
 */
ContextConfig::~ContextConfig()
{
}


void ContextConfig::load()
{
    setSorting((Fuoten::FuotenEnums::SortingRole)value(path(QStringLiteral("sorting")), Fuoten::FuotenEnums::Name).toInt());
    setHideRead(value(path(QStringLiteral("hideRead")), false).toBool());
    setSortOrder((Qt::SortOrder)value(path(QStringLiteral("sortOrder")), Qt::AscendingOrder).toInt());
    setShowFolderSections(value(path(QStringLiteral("showFolderSections")), true).toBool());
    setRespectPinned(value(path(QStringLiteral("respectPinned")), true).toBool());
}


FuotenAppEnums::Context ContextConfig::contextType() const { return m_contextType; }

void ContextConfig::setContextType(FuotenAppEnums::Context nContextType)
{
    if (nContextType != m_contextType) {
        m_contextType = nContextType;
#ifdef QT_DEBUG
        qDebug() << "Changed contextType to" << m_contextType;
#endif
        emit contextTypeChanged(contextType());
        load();
    }
}




qint64 ContextConfig::contextId() const { return m_contextId; }

void ContextConfig::setContextId(qint64 nId)
{
    if (nId != m_contextId) {
        m_contextId = nId;
#ifdef QT_DEBUG
        qDebug() << "Changed id to" << m_contextId;
#endif
        emit contextIdChanged(contextId());
        load();
    }
}




Fuoten::FuotenEnums::SortingRole ContextConfig::sorting() const { return m_sorting; }

void ContextConfig::setSorting(Fuoten::FuotenEnums::SortingRole nSorting)
{
    if (nSorting != m_sorting) {
        m_sorting = nSorting;
#ifdef QT_DEBUG
        qDebug() << "Changed sorting to" << m_sorting;
#endif
        setValue(path("sorting"), m_sorting);
        emit sortingChanged(sorting());
    }
}




bool ContextConfig::hideRead() const { return m_hideRead; }

void ContextConfig::setHideRead(bool nHideRead)
{
    if (nHideRead != m_hideRead) {
        m_hideRead = nHideRead;
#ifdef QT_DEBUG
        qDebug() << "Changed hideRead to" << m_hideRead;
#endif
        setValue(path("hideRead"), m_hideRead);
        emit hideReadChanged(hideRead());
    }
}




Qt::SortOrder ContextConfig::sortOrder() const { return m_sortOrder; }

void ContextConfig::setSortOrder(Qt::SortOrder nSortOrder)
{
    if (nSortOrder != m_sortOrder) {
        m_sortOrder = nSortOrder;
#ifdef QT_DEBUG
        qDebug() << "Changed sortOrder to" << m_sortOrder;
#endif
        setValue(path("sortOrder"), m_sortOrder);
        emit sortOrderChanged(sortOrder());
    }
}




bool ContextConfig::showFolderSections() const { return m_showFolderSections; }

void ContextConfig::setShowFolderSections(bool nShowFolderSections)
{
    if (nShowFolderSections != m_showFolderSections) {
        m_showFolderSections = nShowFolderSections;
#ifdef QT_DEBUG
        qDebug() << "Changed showFolderSections to" << m_showFolderSections;
#endif
        setValue(path(QStringLiteral("showFolderSections")), m_showFolderSections);
        emit showFolderSectionsChanged(showFolderSections());
    }
}

bool ContextConfig::respectPinned() const { return m_respectPinned; }

void ContextConfig::setRespectPinned(bool nRespectPinned)
{
    if (nRespectPinned != m_respectPinned) {
        m_respectPinned = nRespectPinned;
#ifdef QT_DEBUG
        qDebug() << "Changed respectPinned to" << m_respectPinned;
#endif
        setValue(path(QStringLiteral("respectPinned")), m_respectPinned);
        emit respectPinnedChanged(respectPinned());
    }
}



QString ContextConfig::path(const QString &key) const
{
    if (key.isEmpty()) {
        return key;
    }

    switch (contextType()) {
    case FuotenAppEnums::StartPage:
        return QStringLiteral("StartPage/%1").arg(key);
    case FuotenAppEnums::Folders:
        return QStringLiteral("Folders_%1/%2").arg(QString::number(contextId()), key);
    case FuotenAppEnums::Feeds:
        return QStringLiteral("Feeds_%1/%2").arg(QString::number(contextId()), key);
    case FuotenAppEnums::Items:
        return QStringLiteral("Items_%1/%2").arg(QString::number(contextId()), key);
    case FuotenAppEnums::SingleItem:
        return QStringLiteral("Item_%1/%2").arg(QString::number(contextId()), key);
    default:
        return key;
    }
}
