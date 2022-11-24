#include "kraken_websocket.h"
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <QCoreApplication>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QString>
#include <QStringList>
#include <iostream>
#include <thread>

QT_USE_NAMESPACE

//! [constructor]

KrakenWebsocket::KrakenWebsocket(const QUrl& url, bool debug, QObject* parent) :
    QObject(parent), m_url(url), m_debug(debug) {
    blance_thread_ = new BalanceThread(&price_ladder.RestApi);
    blance_thread_->start();
    // std::cout << "updata frequency:";

    // std::cin >> MAX_UPDATA_MIDPRICE;


    if (m_debug)
        qDebug() << "WebSocket server:" << url;

    for (auto& socket : sockets_) {
        connect(&socket, &QWebSocket::connected, this,
                &KrakenWebsocket::onConnected);
        connect(&socket, &QWebSocket::disconnected, this,
                &KrakenWebsocket::onclosed);
    }

    open_time_ = std::chrono::duration_cast<std::chrono::nanoseconds>(
                     std::chrono::time_point_cast<std::chrono::nanoseconds>(
                         std::chrono::high_resolution_clock::now())
                         .time_since_epoch())
                     .count();
    sockets_[number_].open(QUrl(url));
}
//! [constructor]

constexpr std::uint32_t hash_str_to_uint32(const char* data) {
    std::uint32_t h(0);
    for (int i = 0; data && ('\0' != data[i]); i++)
        h = (h << 6) ^ (h >> 26) ^ data[i];
    return h;
}
void KrakenWebsocket::onclosed() {
    QWebSocket* a = (QWebSocket*)QObject::sender();
}

//! [onConnected]
void KrakenWebsocket::onConnected() {
    auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::time_point_cast<std::chrono::nanoseconds>(
                        std::chrono::high_resolution_clock::now())
                        .time_since_epoch())
                    .count();
    auto time_diff = time - open_time_;
    QWebSocket* a = (QWebSocket*)QObject::sender();
    if (time_diff < min_time_) {
        best_idx_ = number_;
        min_time_ = time_diff;
    } else {
        a->close();
    }

    number_++;
    if (number_ == max_try_times_) {
        m_webSocket = &sockets_[best_idx_];
        if (m_debug)
            std::cout << "WebSocket connected \r\n";
        connect(m_webSocket, &QWebSocket::textMessageReceived, this,
                &KrakenWebsocket::onTextMessageReceived);

        //connect(, &QWebSocket::textMessageReceived,this, &KrakenWebsocket::onTextMessageReceived);
        QString KrakenBook_Json = QStringLiteral(
            "{\"event\": \"subscribe\", 	\"feed\": \"book\", 	"
            "\"product_ids\": [ 		\"PI_XBTUSD\" 	] }");
        std::cout << "Send book \r\n";
        m_webSocket->sendTextMessage(KrakenBook_Json);
        qDebug() << "Found God WebSocket " << min_time_ << "nanoseconds";

    } else {
        open_time_ = std::chrono::duration_cast<std::chrono::nanoseconds>(
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                             std::chrono::high_resolution_clock::now())
                             .time_since_epoch())
                         .count();
        sockets_[number_].open(QUrl(m_url));
    }
}
//! [onConnected]

//! [onTextMessageReceived]
void KrakenWebsocket::onTextMessageReceived(QString message) {
    if (m_debug && !message.contains("event")) {
        QJsonParseError jsonerror;
        QJsonDocument doc =
            QJsonDocument::fromJson(message.toLatin1(), &jsonerror);
        QJsonObject root_Obj = doc.object();
        QString feedType = root_Obj.value(QString("feed")).toString();
        switch (hash_str_to_uint32(feedType.toStdString().c_str())) {
            case hash_str_to_uint32("book"): {
                QString feedType = root_Obj.value(QString("side")).toString();

                if (feedType.contains(
                        "buy")) {  //Someone wants to buy it. I'll sell it at that value

                    price_ladder.onUpdataDidsMap(
                        root_Obj.value("price").toDouble(),
                        root_Obj.value("qty").toDouble(),
                        root_Obj.value("timestamp").toDouble(), true);
                } else {  //Someone wants to sell it. I'll buy it at that value
                    price_ladder.onUpdataAsksMap(
                        root_Obj.value("price").toDouble(),
                        root_Obj.value("qty").toDouble(),
                        root_Obj.value("timestamp").toDouble(), true);
                }
                //updata_midprice++;

                int64_t nowTime =
                    std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::time_point_cast<std::chrono::seconds>(
                            std::chrono::high_resolution_clock::now())
                            .time_since_epoch())
                        .count();
                if (last_time_ == 0) {
                    last_time_ = nowTime;
                } else if (nowTime - last_time_ >= maximum_time) {
                    price_ladder.getAllMidpirce();
                    last_time_ = nowTime;
                }
                // if (updata_midprice >= MAX_UPDATA_MIDPRICE) {
                //     price_ladder.getAllMidpirce();
                //     updata_midprice = 0;
                // }

                break;
            }


            case hash_str_to_uint32("book_snapshot"): {
                QJsonValue bidsValue = root_Obj.value("bids");
                QJsonValue asksValue = root_Obj.value("asks");
                if (bidsValue.isArray() && asksValue.isArray()) {
                    QJsonArray bidsArray = bidsValue.toArray();
                    QJsonArray asksArray = asksValue.toArray();
                    foreach (const QJsonValue& v, bidsArray) {
                        price_ladder.onUpdataDidsMap(
                            v.toObject().value("price").toDouble(),
                            v.toObject().value("qty").toDouble(), 0);
                    }
                    foreach (const QJsonValue& v, asksArray) {
                        price_ladder.onUpdataAsksMap(
                            v.toObject().value("price").toDouble(),
                            v.toObject().value("qty").toDouble(), 0);
                    }
                }
                //std::cout << "Now Balance : " << price_ladder.RestApi.balance_
                //         << "\r\n";
                break;
            }
            default:
                break;
        }
    }
}
//! [onTextMessageReceived]
