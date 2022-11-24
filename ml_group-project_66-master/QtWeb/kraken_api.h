#pragma once
#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <queue>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
class KrakenApi : public QObject {
public:
    KrakenApi(const std::string& ApiUrl, const std::string& ApiKey,
              const std::string& ApiSecret, const std::string& send_order_path,
              const std::string& cancel_order_path);
    ~KrakenApi();
    void SendOrder(double price, double size, bool is_buy);
    void CancelOrder(std::string order_id, std::string client_id);
    //static void GetBalance(KrakenApi* ka);

public:
    void OnFinished(QNetworkReply* reply);
    void OnFinishedCancel(QNetworkReply* reply);
    void OnFinishedGetBalance(QNetworkReply* reply);
    std::pair<std::string, std::string> authentication(
        const std::string& endpoint, const std::string& post_data);
    static std::string newClientId();
    std::string C_hmac(std::string key, unsigned char* data);

    std::string base64_encode(const ::std::string& bindata);

    std::string base64_decode(const ::std::string& ascdata);

    void sha256(char* str, unsigned char* obuf);

public:
    int balance_{ 0 };
    int last_balance_{ 0 };
    FILE* m_fp = nullptr;

public:
    const std::string API_URL;
    const std::string API_KEY;
    const std::string API_SECRET;
    const std::string SEND_ORDER_PATH;

    const std::string CANCEL_ORDER_PATH;
    std::queue<std::tuple<double, double, bool>> order_queue_;
    std::array<QNetworkAccessManager, 10> http_client_send_oder_;
    int max_client_number_{ 10 };

    static constexpr char API_PREFIX[]{ "/derivatives" };
    static constexpr char API_PREFIX_TWO[]{ "/api/v3" };
    static constexpr char API_CANCEL_PREFIX[]{ "/cancelorder" };
    static constexpr char API_POSITIONS_PREFIX[]{ "/openpositions" };

    std::string client_id_;
    QNetworkAccessManager http_client_;
    QNetworkAccessManager http_client_cancel;
    QNetworkAccessManager http_client_balance;
    int best_rest_ide{ 0 };
    QNetworkRequest http_request_;
    QNetworkRequest http_request_cancel;
    QNetworkRequest http_request_balance;

    inline static const char b64_table[65]{
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
    };

    inline static const char reverse_table[128]{
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
        64, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
        64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
    };
};