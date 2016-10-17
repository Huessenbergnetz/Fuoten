#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QStringList>

/*!
 * \brief Contains information about supported UI language.
 */
struct Language {
    QString code;   /**< Language code in form language_Country */
    QString name;   /**< Language display name */
};

/*!
 * \brief Model containing available UI languages.
 * \todo Move to BT_SFOS_Components and make the model suitable for every project.
 */
class LanguageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new LanguageModel.
     */
    explicit LanguageModel(QObject *parent = nullptr);

    /*!
     * \brief Destroys the LanguageModel and all model items.
     */
    ~LanguageModel();

    /*!
     * \brief The model roles.
     *
     * For access from QML, use the enum name starting lowercase.
     */
    enum Roles {
        Code = Qt::UserRole + 1,    /**< Langauge code in form lang_Country */
        Name                        /**< Language display name */
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QVariant data(const QModelIndex &index, int role = Qt::UserRole) const Q_DECL_OVERRIDE Q_DECL_FINAL;

    /*!
     * \brief Returns the index of the language identified by \a langCode.
     */
    Q_INVOKABLE int findIndex(const QString &langCode) const;

private:
    QList<Language*> m_langs;

    /*!
     * \brief Clears the model data and deletes any entry.
     */
    void clear();


    /*!
     * \brief Initializes the model data.
     *
     * This is called directly in the constructor.
     */
    void init();

    const QStringList m_supportedLangs;
};

#endif // LANGUAGEMODEL_H
