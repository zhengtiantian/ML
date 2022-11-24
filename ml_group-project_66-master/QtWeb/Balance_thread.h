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

class BalanceThread : public QThread {
public:
    // constructor
    // set name using initializer
    explicit BalanceThread(KrakenApi* Kr);

    // overriding the QThread's run() method
    void run();
    KrakenApi* Kr_{ nullptr };
};