/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "sfosconfig.h"
#include <string>
#include <memory>
#include <limits>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <QStringBuilder>
#include <QFile>
#include <QTextStream>
#include "../../ADVobfuscator/Lib/MetaString.h"

using namespace andrivet::ADVobfuscator;

static const unsigned int KEY_SIZE = 32;
static const unsigned int BLOCK_SIZE = 16;

#define CONF_KEY_PUSHUPONARTICLE "behavior/pushUpOnArticle"
#define CONF_KEY_SHOWSEARCH "behavior/showSearch"

template <typename T>
struct zallocator
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    pointer address (reference v) const {return &v;}
    const_pointer address (const_reference v) const {return &v;}

    pointer allocate (size_type n, const void* hint = nullptr) {
        Q_UNUSED(hint)
        if (n > std::numeric_limits<size_type>::max() / sizeof(T))
            throw std::bad_alloc();
        return static_cast<pointer> (::operator new (n * sizeof (value_type)));
    }

    void deallocate(pointer p, size_type n) {
        OPENSSL_cleanse(p, n*sizeof(T));
        ::operator delete(p);
    }

    size_type max_size() const {
        return std::numeric_limits<size_type>::max() / sizeof (T);
    }

    template<typename U>
    struct rebind
    {
        typedef zallocator<U> other;
    };

    void construct (pointer ptr, const T& val) {
        new (static_cast<T*>(ptr) ) T (val);
    }

    void destroy(pointer ptr) {
        static_cast<T*>(ptr)->~T();
    }

#if __cplusplus >= 201103L
    template<typename U, typename... Args>
    void construct (U* ptr, Args&&  ... args) {
        ::new (static_cast<void*> (ptr) ) U (std::forward<Args> (args)...);
    }

    template<typename U>
    void destroy(U* ptr) {
        ptr->~U();
    }
#endif
};

typedef unsigned char byte;
typedef std::basic_string<char, std::char_traits<char>, zallocator<char> > secure_string;
using EVP_CIPHER_CTX_free_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;


SfosConfig::SfosConfig(QObject *parent) : Configuration(parent)
{
    EVP_add_cipher(EVP_aes_256_cbc());
    m_password = value(QStringLiteral("account/password")).toString();
    m_pushUpOnArticle = value(QStringLiteral(CONF_KEY_PUSHUPONARTICLE), false).toBool();
}


SfosConfig::~SfosConfig()
{

}


QString SfosConfig::encPw(const QString &pw) const
{
    QString enc;

    const QByteArray pwba = pw.toUtf8();
    if (!pwba.isEmpty()) {
        secure_string in;
        secure_string out;
        in.reserve(pwba.size());
        for (char ch : pwba) {
            in.push_back(ch);
        }

        byte key[KEY_SIZE];

        const QByteArray keyBa(OBFUSCATED(AES256_KEY));
        Q_ASSERT_X(keyBa.size() == KEY_SIZE, "encrypting password", "key size mismatch");
        for (unsigned int i = 0; i < KEY_SIZE; ++i) {
            key[i] = keyBa.at(i);
        }

        byte iv[BLOCK_SIZE];
        RAND_bytes(iv, BLOCK_SIZE);

        EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);

        if (EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr, key, iv) != 1) {
            qFatal("EVP_EncryptInit_ex failed");
        }

        out.resize(in.size() + BLOCK_SIZE);
        int out_len1 = (int)out.size();

        if (EVP_EncryptUpdate(ctx.get(), (byte*)&out[0], &out_len1, (const byte*)&in[0], (int)in.size()) != 1) {
            qFatal("EVP_EncryptUpdate failed");
        }

        int out_len2 = (int)out.size() - out_len1;

        if (EVP_EncryptFinal_ex(ctx.get(), (byte*)&out[0]+out_len1, &out_len2) != 1) {
            qFatal("EVP_EncryptFinal_ex failed");
        }

        out.resize(out_len1 + out_len2);

        QByteArray outba;
        for (const char ch : out) {
            outba.append(ch);
        }

        enc = QString::fromLatin1(QByteArray::fromRawData((const char *)iv, BLOCK_SIZE).toBase64()) % QLatin1Char(':') % QString::fromLatin1(outba.toBase64());

        OPENSSL_cleanse(key, KEY_SIZE);
        OPENSSL_cleanse(iv, BLOCK_SIZE);
    }

    return enc;
}


QString SfosConfig::decPw(const QString &pw) const
{
    QString dec;

    if (Q_LIKELY(!pw.isEmpty())) {
        const int colPos = pw.indexOf(QLatin1Char(':'));
        if (Q_LIKELY(colPos > 0)) {
            const QByteArray pwBa = QByteArray::fromBase64(pw.midRef(colPos + 1).toLatin1());
            const QByteArray ivBa = QByteArray::fromBase64(pw.leftRef(colPos).toLatin1());
            if (!pwBa.isEmpty() && !ivBa.isEmpty()) {
                secure_string in;
                secure_string out;
                in.reserve(pwBa.size());
                for (char ch : pwBa) {
                    in.push_back(ch);
                }

                byte key[KEY_SIZE];
                const QByteArray keyBa(OBFUSCATED(AES256_KEY));
                Q_ASSERT_X(keyBa.size() == KEY_SIZE, "decrypting password", "key size mismatch");
                for (unsigned int i = 0; i < KEY_SIZE; ++i) {
                    key[i] = keyBa.at(i);
                }

                byte iv[BLOCK_SIZE];

                int i = 0;
                for (char ch : ivBa) {
                    iv[i] = (unsigned char)ch;
                    ++i;
                }

                EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);

                if (EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr, key, iv) != 1) {
                    qFatal("EVP_DecryptInit_ex failed");
                }

                out.resize(in.size());
                int out_len1 = (int)out.size();

                if (EVP_DecryptUpdate(ctx.get(), (byte*)&out[0], &out_len1, (const byte*)&in[0], (int)in.size()) != 1) {
                    qFatal("EVP_DecryptUpdate failed");
                }

                int out_len2 = (int)out.size() - out_len1;
                if (EVP_DecryptFinal_ex(ctx.get(), (byte*)&out[0]+out_len1, &out_len2) != 1) {
                    qFatal("EVP_DecryptFinal_ex failed");
                }

                out.resize(out_len1 + out_len2);

                QByteArray outba;
                for (const char ch : out) {
                    outba.append(ch);
                }

                dec = QString::fromUtf8(outba);

                OPENSSL_cleanse(key, KEY_SIZE);
                OPENSSL_cleanse(iv, BLOCK_SIZE);
            }
        } else {
            dec = pw;
        }
    }

    return dec;
}


QString SfosConfig::getPassword() const
{
    return decPw(m_password);
}


void SfosConfig::setPassword(const QString &password)
{
    const QString _epw = encPw(password);
    if (m_password != _epw) {
        m_password = _epw;
        setValue(QStringLiteral("account/password"), m_password);
        qDebug("%s", "Password changed");
        emit passwordChanged(getPassword());
        checkAccountValidity();
    }
}


bool SfosConfig::pushUpOnArticle() const { return m_pushUpOnArticle; }

void SfosConfig::setPushUpOnArticle(bool pushUpOnArticle)
{
    if (m_pushUpOnArticle != pushUpOnArticle) {
        m_pushUpOnArticle = pushUpOnArticle;
        setValue(QStringLiteral(CONF_KEY_PUSHUPONARTICLE), m_pushUpOnArticle);
        qDebug("Changed pushUpOnArticle to %s.", m_pushUpOnArticle ? "true" : "false");
        emit pushUpOnArticleChanged(m_pushUpOnArticle);
    }
}


bool SfosConfig::showSearch() const
{
    return m_showSearch;
}

void SfosConfig::setShowSearch(bool showSearch)
{
    if (m_showSearch != showSearch) {
        qDebug("Changing showSearch from %s to %s", m_showSearch ? "true" : "false", showSearch ? "true" : "false");
        m_showSearch = showSearch;
        setValue(QStringLiteral(CONF_KEY_SHOWSEARCH), m_showSearch);
        emit showSearchChanged(m_showSearch);
    }
}


QString SfosConfig::getUserAgent() const
{
    return QStringLiteral("Fuoten %1 on SailfishOS, Qt %2").arg(QStringLiteral(VERSION_STRING), QString::fromLatin1(qVersion()));
}


QString SfosConfig::getLoginFlowUserAgent() const
{
    QString ua = QStringLiteral("Fuoten (");

    QString hwname = getHwReleasename();
    if (hwname.isEmpty()) {
        hwname = QStringLiteral("Sailfish OS");
    }

    ua += hwname;
    ua += QLatin1Char(')');

    return ua;
}

QString SfosConfig::getHwReleasename() const
{
    QString hwname;

    QFile hwrelease(QStringLiteral("/etc/hw-release"));
    if (hwrelease.exists()) {
        if (hwrelease.open(QFile::ReadOnly|QFile::Text)) {
            QTextStream in(&hwrelease);
            QString line;
            while (in.readLineInto(&line)) {
                const QString trimmedLine = line.trimmed();
                if (trimmedLine.startsWith(QLatin1String("NAME="), Qt::CaseInsensitive)) {
                    QString value = trimmedLine.mid(trimmedLine.indexOf(QLatin1Char('=')) + 1);
                    if (value.startsWith(QLatin1Char('"'))) {
                        value.remove(0, 1);
                    }
                    if (value.endsWith(QLatin1Char('"'))) {
                        value.chop(1);
                    }
                    hwname = value;
                    return hwname;
                }
            }
            qWarning("%s", "Can not find NAME= entry in /etc/hw-release");
        } else {
            qWarning("%s", "Can not open /etc/hw-release for reading");
        }
    } else {
        qWarning("%s", "Can not find /etc/hw-release");
    }

    return hwname;
}

#include "moc_sfosconfig.cpp"
