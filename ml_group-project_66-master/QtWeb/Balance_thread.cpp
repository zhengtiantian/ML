#include "Balance_thread.h"
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
using std::cout;
using std::endl;
BalanceThread::BalanceThread(KrakenApi* Kr) {
    Kr_ = Kr;
}

void BalanceThread::run() {
    KrakenApi Ka_{ "https://api.futures.kraken.com",

                   "This is own key",
                   "This is own key/"
                   "This is own key"
                   "This is own key",
                   "/api/v3/sendorder", "/api/v3/cancelorder" };
    //std::cout << "Get balance thread runing";
    while (true) {
        QEventLoop l_event_loop;

        auto [nonce, authent] = Ka_.authentication("/api/v3/openpositions", "");
        Ka_.http_request_balance.setRawHeader("APIKey", Ka_.API_KEY.c_str());
        Ka_.http_request_balance.setRawHeader("Nonce", nonce.c_str());
        Ka_.http_request_balance.setRawHeader("Authent", authent.c_str());
        std::string url = Ka_.API_URL + Ka_.API_PREFIX + Ka_.API_PREFIX_TWO
                          + Ka_.API_POSITIONS_PREFIX;


        // http_request_balance.setUrl(QUrl(url.c_str()));
        // http_client_balance.get(http_request_balance);

        Ka_.http_request_balance.setUrl(QUrl(url.c_str()));
        QNetworkReply* l_reply =
            Ka_.http_client_balance.get(Ka_.http_request_balance);
        QObject::connect(l_reply, SIGNAL(finished()), &l_event_loop,
                         SLOT(quit()));

        l_event_loop.exec();

        Ka_.OnFinishedGetBalance(l_reply);
        Kr_->balance_ = Ka_.balance_;
        //1ms
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}