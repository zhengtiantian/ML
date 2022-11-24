#include "kraken_api.h"
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <QCoreApplication>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

#include <QString>
#include <QStringList>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
KrakenApi::KrakenApi(const std::string& api_url, const std::string& api_key,
                     const std::string& api_secret,
                     const std::string& send_order_path,
                     const std::string& cancel_order_path) :
    API_URL{ api_url },
    API_KEY{ api_key },
    API_SECRET{ api_secret },
    SEND_ORDER_PATH{ send_order_path },
    CANCEL_ORDER_PATH{ cancel_order_path } {
    QObject::connect(&http_client_, &QNetworkAccessManager::finished, this,
                     &KrakenApi::OnFinished);
    QObject::connect(&http_client_cancel, &QNetworkAccessManager::finished,
                     this, &KrakenApi::OnFinishedCancel);
    // QObject::connect(&http_client_balance, &QNetworkAccessManager::finished,
    //                  this, &KrakenApi::OnFinishedGetBalance);
}

std::string KrakenApi::newClientId() {
    unsigned long milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count();
    return "Guowen-" + std::to_string(milliseconds);
}


std::pair<std::string, std::string> KrakenApi::authentication(
    const std::string& endpoint, const std::string& post_data) {
    unsigned long milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count();
    milliseconds += rand() % 100 + 1;
    std::string nonce = std::to_string(milliseconds);
    std::string a = post_data + nonce + endpoint;

    unsigned char b[SHA256_DIGEST_LENGTH];
    sha256((char*)a.c_str(), b);

    std::string c = base64_decode(API_SECRET);
    std::string d = C_hmac(c, b);
    std::string e = base64_encode(d);
    return { nonce, e };
}


void KrakenApi::SendOrder(double price, double size, bool is_buy) {
    const auto client_id = newClientId();
    const auto post_data =
        "orderType=lmt&symbol=PI_XBTUSD&side="
        + std::string(is_buy ? "buy" : "sell") + "&size=" + std::to_string(size)
        + "&limitPrice=" + std::to_string(price)
        + "&triggerSignal=mark&cliOrdId=" + client_id + "&reduceOnly=false";
    auto [nonce, authent] = authentication(SEND_ORDER_PATH, post_data);
    http_request_.setRawHeader("APIKey", API_KEY.c_str());
    http_request_.setRawHeader("Nonce", nonce.c_str());
    http_request_.setRawHeader("Authent", authent.c_str());

    std::string url = API_URL + API_PREFIX + SEND_ORDER_PATH + "?" + post_data;

    http_request_.setUrl(QUrl(url.c_str()));
    http_client_.get(http_request_);
}

// void KrakenApi::GetBalance(KrakenApi* ka) {
//     qDebug() << "Get balance thread runing";
//     while (true) {
//         QEventLoop l_event_loop;

//         auto [nonce, authent] = ka->authentication("/api/v3/openpositions", "");
//         ka->http_request_balance.setRawHeader("APIKey", ka->API_KEY.c_str());
//         ka->http_request_balance.setRawHeader("Nonce", nonce.c_str());
//         ka->http_request_balance.setRawHeader("Authent", authent.c_str());
//         std::string url = ka->API_URL + ka->API_PREFIX + ka->API_PREFIX_TWO
//                           + ka->API_POSITIONS_PREFIX;


//         // http_request_balance.setUrl(QUrl(url.c_str()));
//         // http_client_balance.get(http_request_balance);

//         ka->http_request_balance.setUrl(QUrl(url.c_str()));
//         QNetworkReply* l_reply =
//             ka->http_client_balance.get(ka->http_request_balance);
//         QObject::connect(l_reply, SIGNAL(finished()), &l_event_loop,
//                          SLOT(quit()));

//         l_event_loop.exec();

//         ka->OnFinishedGetBalance(l_reply);

//         usleep(1);
//     }
// }
void KrakenApi::CancelOrder(std::string order_id, std::string client_id) {
    const auto post_data = "order_id=" + order_id + "&cliOrdId=" + client_id;
    auto [nonce, authent] = authentication(CANCEL_ORDER_PATH, post_data);
    http_request_cancel.setRawHeader("APIKey", API_KEY.c_str());
    http_request_cancel.setRawHeader("Nonce", nonce.c_str());
    http_request_cancel.setRawHeader("Authent", authent.c_str());
    std::string url =
        API_URL + API_PREFIX + CANCEL_ORDER_PATH + "?" + post_data;


    http_request_cancel.setUrl(QUrl(url.c_str()));
    http_client_cancel.get(http_request_cancel);
}
void KrakenApi::OnFinished(QNetworkReply* reply) {
    QString answer = reply->readAll();
    if (answer.contains("success")) {
        QJsonParseError jsonerror;
        QJsonDocument doc =
            QJsonDocument::fromJson(answer.toLatin1(), &jsonerror);
        QJsonObject root_Obj = doc.object();
        QJsonValue status_value = root_Obj.value("sendStatus");
        QJsonObject obj = status_value.toObject();

        QJsonValue order_id = obj.value("order_id");
        QJsonValue cliOrdId = obj.value("cliOrdId");
        CancelOrder(order_id.toString().toStdString(),
                    cliOrdId.toString().toStdString());
        //GetBalance();
        return;
    }
}
void KrakenApi::OnFinishedGetBalance(QNetworkReply* reply) {
    QString answer = reply->readAll();
    if (answer.contains("success")) {
        QJsonParseError jsonerror;
        QJsonDocument doc =
            QJsonDocument::fromJson(answer.toLatin1(), &jsonerror);
        QJsonObject root_Obj = doc.object();
        QJsonValue status_value = root_Obj.value("openPositions");
        if (status_value.isArray()) {
            QJsonArray value = status_value.toArray();
            if (value.size() == 0) {
                balance_ = 0;
                return;
            }
            foreach (const QJsonValue& v, value) {
                QString side = v.toObject().value("side").toString();
                QJsonValue size = v.toObject().value("size");
                if (side == "long") {
                    balance_ = size.toInt();
                } else if (side == "short") {
                    balance_ = -(size.toInt());
                }
            }


            return;
        }
    }


    //qDebug() << answer;
}
void KrakenApi::OnFinishedCancel(QNetworkReply* reply) {
    QString answer = reply->readAll();
    if (answer.contains("nonceDuplicate")) {
        // qDebug() << "nonceDuplicate(failed) - "
        //          << "balance : " << balance_
        //          << "\r\n------------------------------\r\n";
        std::cout << "failed\r\n" << std::flush;
        ;
    } else if (answer.contains("notFound")) {
        // qDebug() << "notFound(success) - "
        //          << "balance : " << balance_
        //          << "\r\n------------------------------\r\n";
        std::cout << "success\r\n" << std::flush;
        ;
    } else if (answer.contains("cancelled")) {
        if (answer.contains("buy")) {
            // qDebug() << "Buy - "
            //          << "cancelled(failed) - "
            //          << "balance : " << balance_
            //          << "\r\n------------------------------\r\n";
            std::cout << "failed\r\n" << std::flush;
            ;
        }
        if (answer.contains("sell")) {
            // qDebug() << "Sell - "
            //          << "cancelled(failed) - "
            //          << "balance : " << balance_
            //          << "\r\n------------------------------\r\n";
            std::cout << "failed\r\n" << std::flush;
            ;
        }


    } else {
        // qDebug() << "Error"
        //          << "";
        std::cout << " nError\r\n" << std::flush;
    }
}

std::string KrakenApi::C_hmac(std::string key, unsigned char* data) {
    /**
   *   man 3 HMAC
   * */
    unsigned int hash_sz = EVP_MAX_MD_SIZE;
    HMAC_CTX* ctx = HMAC_CTX_new();
    unsigned char* digest =
        HMAC(EVP_sha512(), key.c_str(), key.size(), data, 32, NULL, &hash_sz);

    std::stringstream ss;
    ss << std::setfill('0');
    for (int i = 0; i < hash_sz; ++i) {
        ss << digest[i];
    }
    std::string final_hash = ss.str();
    return final_hash;
}

std::string KrakenApi::base64_encode(const ::std::string& bindata) {
    using ::std::numeric_limits;
    using ::std::string;

    if (bindata.size() > (numeric_limits<string::size_type>::max() / 4u) * 3u) {
        throw ::std::length_error("Converting too large a string to base64.");
    }

    const ::std::size_t binlen = bindata.size();
    string retval((((binlen + 2) / 3) * 4), '=');
    ::std::size_t outpos = 0;
    int bits_collected = 0;
    unsigned int accumulator = 0;
    const string::const_iterator binend = bindata.end();

    for (string::const_iterator i = bindata.begin(); i != binend; ++i) {
        accumulator = (accumulator << 8) | (*i & 0xffu);
        bits_collected += 8;
        while (bits_collected >= 6) {
            bits_collected -= 6;
            retval[outpos++] =
                b64_table[(accumulator >> bits_collected) & 0x3fu];
        }
    }
    if (bits_collected > 0) {  // Any trailing bits that are missing.
        assert(bits_collected < 6);
        accumulator <<= 6 - bits_collected;
        retval[outpos++] = b64_table[accumulator & 0x3fu];
    }
    assert(outpos >= (retval.size() - 2));
    assert(outpos <= retval.size());
    return retval;
}
std::string KrakenApi::base64_decode(const ::std::string& ascdata) {
    using ::std::string;
    string retval;
    const string::const_iterator last = ascdata.end();
    int bits_collected = 0;
    unsigned int accumulator = 0;

    for (string::const_iterator i = ascdata.begin(); i != last; ++i) {
        const int c = *i;
        if (::std::isspace(c) || c == '=') {
            // Skip whitespace and padding. Be liberal in what you accept.
            continue;
        }
        if ((c > 127) || (c < 0) || (reverse_table[c] > 63)) {
            throw ::std::invalid_argument("This contains characters not legal "
                                          "in a base64 encoded string.");
        }
        accumulator = (accumulator << 6) | reverse_table[c];
        bits_collected += 6;
        if (bits_collected >= 8) {
            bits_collected -= 8;
            retval +=
                static_cast<char>((accumulator >> bits_collected) & 0xffu);
        }
    }
    return retval;
}
void KrakenApi::sha256(char* str, unsigned char* obuf) {
    SHA256((const unsigned char*)str, strlen(str), obuf);
}
KrakenApi::~KrakenApi() = default;