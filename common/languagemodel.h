/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * common/languagemodel.h
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

    int rowCount(const QModelIndex &parent = QModelIndex()) const override Q_DECL_FINAL;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override Q_DECL_FINAL;
    QHash<int, QByteArray> roleNames() const override Q_DECL_FINAL;
    QVariant data(const QModelIndex &index, int role = Qt::UserRole) const override Q_DECL_FINAL;

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
