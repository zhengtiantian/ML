#include "send_order_thread.h"
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
SendOrderThread::SendOrderThread(KrakenApi* Kr) {
    Kr_ = Kr;
    for (auto& http : Kr_->http_client_send_oder_) {
        QObject::connect(&http, &QNetworkAccessManager::finished, Kr_,
                         &KrakenApi::OnFinished);
    }
}

void SendOrderThread::run() {
    QEventLoop l_event_loop;
    std::string url = API_URL + API_PATH;

    // http_request_balance.setUrl(QUrl(url.c_str()));
    // http_client_balance.get(http_request_balance);
    QNetworkRequest http_request_test;
    http_request_test.setUrl(QUrl(url.c_str()));
    while (true) {
        int last_id = 0;
        int best_id = 0;
        for (int i = 0; i < Kr_->max_client_number_; i++) {
            auto open_time =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::time_point_cast<std::chrono::nanoseconds>(
                        std::chrono::high_resolution_clock::now())
                        .time_since_epoch())
                    .count();
            QNetworkReply* l_reply =
                Kr_->http_client_send_oder_[i].get(http_request_test);
            QObject::connect(l_reply, SIGNAL(finished()), &l_event_loop,
                             SLOT(quit()));
            l_event_loop.exec();
            l_reply->readAll();
            auto close_time =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::time_point_cast<std::chrono::nanoseconds>(
                        std::chrono::high_resolution_clock::now())
                        .time_since_epoch())
                    .count();
        }
        //1ms
        //std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    }
}