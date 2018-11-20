/* Fuoten - ownCloud/Nextcloud News App Client
 * Copyright (C) 2016 Buschtrommel/Matthias Fehring
 * https://www.buschmann23.de/entwicklung/anwendungen/fuoten/
 * https://github.com/Buschtrommel/Fuoten
 *
 * sailfishos/src/sfosconfig.cpp
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

#include "sfosconfig.h"
#include <string>
#include <memory>
#include <limits>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <QStringBuilder>
#include "../../ADVobfuscator/Lib/MetaString.h"

using namespace andrivet::ADVobfuscator;


static const unsigned int KEY_SIZE = 32;
static const unsigned int BLOCK_SIZE = 16;

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

    pointer allocate (size_type n, const void* hint = 0) {
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

#if __cpluplus >= 201103L
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

        if (EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key, iv) != 1) {
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

                if (EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key, iv) != 1) {
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
        Q_EMIT passwordChanged(getPassword());
    }
}


QString SfosConfig::getUserAgent() const
{
    return QStringLiteral("Fuoten %1 on SailfishOS, Qt %2").arg(QStringLiteral(VERSION_STRING), QString::fromLatin1(qVersion()));
}

#include "moc_sfosconfig.cpp"
