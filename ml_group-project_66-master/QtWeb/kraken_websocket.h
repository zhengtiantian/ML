#pragma once

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <array>
#include <memory>
#include "Balance_thread.h"
#include "price_ladder_structure.h"
#include "send_order_thread.h"
class KrakenWebsocket : public QObject {
    Q_OBJECT
public:
    explicit KrakenWebsocket(const QUrl& url, bool debug = false,
                             QObject* parent = nullptr);

Q_SIGNALS:
    void closed();

private slots:
    void onConnected();
    void onclosed();
    void onTextMessageReceived(QString message);

public:
private:
private:
    int64_t min_time_{ INT64_MAX };
    int64_t open_time_{ 0 };
    int number_{ 0 };
    int max_try_times_{ 3 };
    int best_idx_{ 0 };
    QWebSocket* m_webSocket{ nullptr };
    std::array<QWebSocket, 3> sockets_;
    QUrl m_url;
    int updata_midprice = 0;
    int MAX_UPDATA_MIDPRICE = 100;
    int64_t last_time_{ 0 };
    int64_t maximum_time{ 10 };
    bool m_debug;
    int m_recevieAmount = 0;
    BalanceThread* blance_thread_{ nullptr };
    SendOrderThread* send_order_thread_{ nullptr };

    PriceLadderStructure price_ladder;
};
