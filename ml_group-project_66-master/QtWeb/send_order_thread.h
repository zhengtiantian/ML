#pragma once
#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtCore/QRunnable>
#include <QtCore/QThread>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <string>
#include <string_view>
#include "kraken_api.h"

class SendOrderThread : public QThread {
public:
    // constructor
    // set name using initializer
    explicit SendOrderThread(KrakenApi* Kr);
    const std::string API_URL = "https://api.futures.kraken.com";
    const std::string API_PATH =
        "/derivatives/api/v3/orderbook?symbol=fi_xbtusd_180615";
    // overriding the QThread's run() method
    void run();
    KrakenApi* Kr_{ nullptr };
};