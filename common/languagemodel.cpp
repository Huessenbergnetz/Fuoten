#include "languagemodel.h"
#include <QLocale>
#include <algorithm>


LanguageModel::LanguageModel(QObject *parent) :
    QAbstractListModel(parent), m_supportedLangs({QStringLiteral("de"), QStringLiteral("en")})
{
    init();
}



LanguageModel::~LanguageModel()
{
    qDeleteAll(m_langs);
    m_langs.clear();
}



QHash<int, QByteArray> LanguageModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Code, QByteArrayLiteral("code"));
    roles.insert(Name, QByteArrayLiteral("name"));
    return roles;
}



int LanguageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_langs.count();
}



QModelIndex LanguageModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    return createIndex(row, column);
}



QVariant LanguageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() > (rowCount()-1)) {
        return QVariant();
    }

    Language *l = m_langs.at(index.row());

    switch(role) {
    case Code:
        return QVariant::fromValue(l->code);
    case Name:
        return QVariant::fromValue(l->name);
    default:
        return QVariant();
    }
}



bool langLessThan(Language *a, Language *b)
{
    return (a->name < b->name);
}



void LanguageModel::init()
{
    clear();

    if (m_supportedLangs.isEmpty()) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, m_supportedLangs.count()-1);

    Q_FOREACH(const QString &lang, m_supportedLangs) {
        QLocale locale(lang);
        Language *l = new Language;
        l->code = lang;
        l->name = locale.nativeLanguageName();
        l->name.append(QLatin1String(" ("));
        l->name.append(QLocale::languageToString(locale.language()));
        l->name.append(QLatin1String(")"));
        m_langs.append(l);
    }

    std::sort(m_langs.begin(), m_langs.end(), langLessThan);

    Language *defLang = new Language;
    defLang->code = QStringLiteral("");
    //% "Default"
    defLang->name = qtTrId("id-default-lang");
    m_langs.prepend(defLang);

    endInsertRows();
}



void LanguageModel::clear()
{
    if (rowCount() == 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, rowCount()-1);

    qDeleteAll(m_langs);
    m_langs.clear();

    endRemoveRows();
}



int LanguageModel::findIndex(const QString &langCode) const
{
    if (rowCount() == 0) {
        return -1;
    }

    int idx = -1;

    for (int i = 0; i < m_langs.count(); ++i) {
        if (m_langs.at(i)->code == langCode) {
            idx = i;
            break;
        }
    }

    return idx;
}
