#pragma once

#include <QtCore/QObject>
#include <chrono>
#include "kraken_api.h"
class PriceLadderStructure : public QObject {
    Q_OBJECT
public:
    struct price {
        double price;
        double weight;
    };
    struct PriceWeight {
        double one_thousand;
        double three_thousand;
        double ten_thousand;
        double thirty_thousand;
        double one_hundred__thousand;
        double level_1;
        double level_2;
        double level_4;
        double level_6;
        double level_8;
    };
    static constexpr int INDEX_ONE_THOUSAND{ 1 };
    static constexpr int INDEX_TEN_THOUSAND{ 2 };
    static constexpr int INDEX_THIRTY_THOUSAND{ 3 };
    static constexpr int INDEX_ONE_HUNDRED_THOUSAND{ 4 };
    static constexpr int INDEX_THREE_THOUSAND{ 5 };
    static constexpr int INDEX_LEVEL_1{ 0 };
    static constexpr int INDEX_LEVEL_2{ 6 };
    static constexpr int INDEX_LEVEL_4{ 7 };
    static constexpr int INDEX_LEVEL_6{ 8 };
    static constexpr int INDEX_LEVEL_8{ 9 };
    double gitAmountMidpirce(double amount);

    std::string getTimestamp();
    explicit PriceLadderStructure(QObject* parent = nullptr);
    void onUpdataAsksMap(double price, double qty, double SeverTimestamp,
                         bool isUpdataBook = false);
    void onUpdataDidsMap(double price, double qty, double SeverTimestamp,
                         bool isUpdataBook = false);
    double* getMidPrice();
    double getThresholdPrice(double mid_price);
    double sellAcceptablePrice();
    double buyAcceptablePrice();
    void addInterestingPrice(double price, double qty, double real_qty,
                             double is_buy, double acceptable_price,
                             int balance);
    std::string getAllFormatLadder(int number);
    double findLevelPrice(int level);
    double findMaxMidpirce(int level);
    double findMinMidpirce(int level);
    void updataRegularWeight(std::vector<price>& price);
    double getSpecifiedMidpirce(double goalSum);
    void setPriceWeight(PriceWeight pw);
    double GetWeightPrice(std::vector<price>& price);
    double regularWeight(std::vector<price> price);
    double getRawMidpirce();
    void getAllMidpirce();
    double getTestMidpirce();
    double updataWeight(double benchmark, double difference);
    KrakenApi RestApi{
        "https://api.futures.kraken.com",

        "mgESONJ7rYoltxWCX7K5BeYOEUHT9umOa7YAEBc40bfwti38IYeEGkne",
        "3wrHnyKlCUTR16tGZEZwgCCj8GzfW+"
        "ttCjfU4W5kUVtLdl6CfJZem1sk0puQFNG9OEIEeTfh2MVMmbvyTnAogAqa",
        "/api/v3/sendorder", "/api/v3/cancelorder"
    };

private:
    // KrakenApi RestApi{
    //     "https://demo-futures.kraken.com",

    //     "ru/hOzAuUTX+X3pHEWEIRiJg0UuNc3hEwRwbopXkDIsdLSF0/A62agBT",
    //     "PpwEEd13dKrwdIaNR+3cX/Z/"
    //     "eaMHXcpK+kQjbjWZka3r6rZXQVb1eYSNmLYyltgN7fW/"
    //     "jSpdEcywb00fgMVRX2TH",
    //     "/api/v3/sendorder", "/api/v3/cancelorder"
    // };


    static constexpr int MID_PRICE_AMOUNT = 10;
    const double MAX_TIME{ 40 };
    double BENCHMARK_RATE{ 0.0001 };
    const double MID_PARAMETER{ 1000 };
    const double MID_PARAMETER_THREE_THOUSAND{ 3000 };
    const double MID_PARAMETER_TEN_THOUSAND{ 10000 };
    const double MID_PARAMETER_THIRTY_THOUSAND{ 30000 };
    const double MID_PARAMETER_ONE_HUNDRED_THOUSAND{ 100000 };
    const double MID_PARAMETER_THREE_HUNDRED_THOUSAND{ 300000 };


    double last_weight_price_{ 0 };
    double mid_price_[MID_PRICE_AMOUNT]{ 0 };
    double mid_price_40_[45]{ 0 };
    double mid_last_price_[MID_PRICE_AMOUNT]{ 0 };
    int average_difference_number{ 0 };
    double average_difference_sum{ 0 };
    std::vector<price> price_{ 0 };
    double AMOUNT{ 2 };
    double real_qty_{ 0 };
    const int MAX_BALANCE{ 1 };
    const int MIN_BALANCE{ -1 };
    std::map<double, double> asks_map_;
    std::map<double, double> bids_map_;
    std::map<double, std::chrono::steady_clock::time_point> interesting_prices_;
};