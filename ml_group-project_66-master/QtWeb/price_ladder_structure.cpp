#include "price_ladder_structure.h"
#include <stdio.h>
#include <QCoreApplication>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QString>
#include <QStringList>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
// QT_USE_NAMESPACE
using namespace std::chrono;

PriceLadderStructure::PriceLadderStructure(QObject* parent) : QObject(parent) {
    for (int i = 0; i < MID_PRICE_AMOUNT; i++) {
        price_.push_back({ .price = 0, .weight = 0 });
        /* code */
    }
    // std::cout << "benchemark rate:";
    // std::cin >> BENCHMARK_RATE;
}
std::string PriceLadderStructure::getTimestamp() {
    // get a precise timestamp as a string
    const auto now = std::chrono::system_clock::now();
    const auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
    const auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                           now.time_since_epoch())
                       % 1000;
    std::stringstream nowSs;

    nowSs << std::put_time(std::localtime(&nowAsTimeT), "%a %b %d %Y %T") << '.'
          << std::setfill('0') << std::setw(3) << nowMs.count();
    return nowSs.str();
}
void PriceLadderStructure::addInterestingPrice(double price, double qty,
                                               double real_qty, double is_buy,
                                               double acceptable_price,
                                               int balance) {
    auto current_time = std::chrono::steady_clock::now();
    interesting_prices_[price] = current_time;
    QString order_type = is_buy ? "BUY" : "SELL";
    double mid = is_buy ? mid_price_[0] : mid_price_[2];
    // qDebug() << order_type << " NOW - MID PRICE: " << mid << ", WE "
    //          << order_type << " AT: " << acceptable_price
    //          << ", PRICE: " << price << ",Real " << order_type
    //          << " QTY: " << qty;
    // qDebug() << "1k $ mid price: " << mid_price_[0]
    //          << "\t 10k $ mid price: " << mid_price_[2];

    std::string time = getTimestamp();


    std::cout << time << "," << order_type.toStdString() << "," << mid << ","
              << acceptable_price << ",now price:" << price
              << ",real qty:" << real_qty << "," << qty << "," << mid_price_[0]
              << "," << mid_price_[1] << "," << balance << ",";
}
void PriceLadderStructure::onUpdataAsksMap(double price, double qty,
                                           double SeverTimestamp,
                                           bool isUpdataBook) {
    if (qty == 0) {
        asks_map_.erase(price);
    } else {
        asks_map_[price] = qty;
    }

    if (isUpdataBook) {
        if (qty == 0
            && interesting_prices_.find(price) != interesting_prices_.end()) {
            interesting_prices_.erase(price);
            return;
        }
        return;
        getMidPrice();
        double ap = buyAcceptablePrice();  //Buy -----
        //&& interesting_prices_.find(price) == interesting_prices_.end()
        if (RestApi.balance_ < MAX_BALANCE && qty > 0
            && interesting_prices_.find(price) == interesting_prices_.end()
            && price <= ap) {
            double b = MAX_BALANCE - RestApi.balance_;

            real_qty_ = std::min({ b, AMOUNT, qty });
            if (real_qty_ == 0 || RestApi.balance_ >= MAX_BALANCE) {
                return;
            }

            // double ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            //                 std::chrono::system_clock::now().time_since_epoch())
            //                 .count();
            // double i = (ms - SeverTimestamp);
            // if (i > MAX_TIME) {
            //     std::cout << "time : " << i << "\r\n";
            // }
            // RestApi.order_queue_.push(std::make_tuple(price, real_qty_, true));
            RestApi.SendOrder(price, real_qty_, true);
            addInterestingPrice(price, qty, real_qty_, true, ap,
                                RestApi.balance_);
        }
    }
}
void PriceLadderStructure::setPriceWeight(PriceWeight pw) {
    price_[INDEX_ONE_THOUSAND].weight = pw.one_thousand;
    price_[INDEX_TEN_THOUSAND].weight = pw.ten_thousand;
    price_[INDEX_THIRTY_THOUSAND].weight = pw.thirty_thousand;
    price_[INDEX_ONE_HUNDRED_THOUSAND].weight = pw.one_hundred__thousand;
    price_[INDEX_THREE_THOUSAND].weight = pw.three_thousand;
    price_[INDEX_LEVEL_1].weight = pw.level_1;
    price_[INDEX_LEVEL_2].weight = pw.level_2;
    price_[INDEX_LEVEL_4].weight = pw.level_4;
    price_[INDEX_LEVEL_6].weight = pw.level_6;
    price_[INDEX_LEVEL_8].weight = pw.level_8;
}
double PriceLadderStructure::GetWeightPrice(std::vector<price>& price) {
    double weight_price = 0;
    for (auto& it : price) {
        weight_price += it.price * it.weight;
    }
    return weight_price;
}
void PriceLadderStructure::updataRegularWeight(std::vector<price>& price) {
    double sum = 0;
    for (auto it : price) {
        sum += it.weight;
    }
    for (size_t i = 0; i < price.size(); i++) {
        price[i].weight = price[i].weight / sum;
    }
    sum = 0;
    for (auto it : price) {
        sum += it.weight;
    }
    // qDebug() << "Now Weight sum : " << sum;
}
double PriceLadderStructure::regularWeight(std::vector<price> price) {
    double sum = 0;
    for (auto it : price) {
        sum += it.weight;
    }
    for (size_t i = 0; i < price.size(); i++) {
        price[i].weight = price[i].weight / sum;
    }

    return GetWeightPrice(price);
}
double PriceLadderStructure::updataWeight(double benchmark, double difference) {
    if (difference == 0) {
        return -1.0;
    }

    std::vector<price> new_Weight = price_;
    for (int i = 0; i < MID_PRICE_AMOUNT; i++) {
        new_Weight[i].price = mid_last_price_[i];
    }

    double new_difference = 0;
    for (int i = 0; i < new_Weight.size(); i++) {
        if (new_Weight[i].weight == 0) {
            new_Weight[i].weight = new_Weight[i].weight + BENCHMARK_RATE;
            double new_benchmark = regularWeight(new_Weight);
            new_difference = std::abs(new_benchmark - benchmark);
            if (new_difference <= difference) {
                price_[i].weight = new_Weight[i].weight;
            }
            new_Weight[i].weight = 0;
            continue;
        }

        new_Weight[i].weight =
            std::max({ 0.0, new_Weight[i].weight - BENCHMARK_RATE });

        double new_benchmark = regularWeight(new_Weight);
        new_difference = std::abs(new_benchmark - benchmark);
        if (new_difference <= difference) {
            price_[i].weight = new_Weight[i].weight;
        } else {
            price_[i].weight += BENCHMARK_RATE;
        }
        new_Weight[i].weight += BENCHMARK_RATE;
    }
    for (size_t i = 0; i < MID_PRICE_AMOUNT; i++) {
        new_Weight[i].weight = price_[i].weight;
    }
    updataRegularWeight(price_);


    // double new_benchmark = regularWeight(new_Weight);
    // double new_difference = std::abs(new_benchmark - benchmark);
    // qDebug() << "New difference:" << new_difference
    //          << "last difference:" << difference;
    // int i = 0;
    return new_difference;
}
std::string PriceLadderStructure::getAllFormatLadder(int number) {
    int i = 0;
    std::string formate_str = "";
    std::string formate_flg = ",";
    std::string formate_table = "";
    for (auto it : asks_map_) {
        if (i >= number) {
            break;
        }

        formate_str += std::to_string(it.first);
        mid_price_40_[i] = it.first;
        formate_table += std::to_string(i + 1) + "_asks_price";
        formate_table += formate_flg;
        formate_str += formate_flg;
        formate_str += std::to_string(it.second);
        formate_str += formate_flg;

        formate_table += std::to_string(i + 1) + "_asks_qty";
        formate_table += formate_flg;
        i++;
    }
    i = 0;
    for (auto it : bids_map_) {
        formate_str += std::to_string(std::abs(it.first));
        mid_price_40_[20 + i] = std::abs(it.first);
        formate_str += formate_flg;
        formate_str += std::to_string(it.second);
        formate_table += std::to_string(i + 1) + "_bids_price";
        formate_table += formate_flg;
        if (i + 1 >= number) {
            break;
        }
        formate_table += std::to_string(i + 1) + "_bids_qty";
        formate_table += formate_flg;
        formate_str += formate_flg;
        i++;
    }
    //qDebug() << formate_table.c_str();
    return formate_str;
}
double PriceLadderStructure::gitAmountMidpirce(double amount) {
    double sum = 0;
    double ask_price = -1;
    double bid_price = -1;
    for (auto it = asks_map_.begin(); it != asks_map_.end() && sum < amount;
         it++) {
        sum += it->second;

        if (sum >= amount || (sum >= amount && ask_price == -1)) {
            ask_price = it->first;
        }
    }
    sum = 0;
    for (auto it = bids_map_.begin(); it != bids_map_.end() && sum < amount;
         it++) {
        sum += it->second;

        if (sum >= amount || (sum >= amount && bid_price == -1)) {
            bid_price = -(it->first);
        }
    }
    return (ask_price + bid_price) / 2;
}
double PriceLadderStructure::getTestMidpirce() {
    std::vector<double> all_mid_price(11, 0);
    std::vector<double> all_mid_weight(11, 0);
    all_mid_price[0] = gitAmountMidpirce(1000);
    all_mid_price[1] = gitAmountMidpirce(10000);
    all_mid_price[2] = gitAmountMidpirce(30000);
    all_mid_price[3] = gitAmountMidpirce(100000);
    all_mid_price[4] = findLevelPrice(3);
    all_mid_price[5] = findLevelPrice(4);
    all_mid_price[6] = findLevelPrice(5);
    all_mid_price[7] = findLevelPrice(9);
    all_mid_price[8] = findLevelPrice(10);
    all_mid_price[9] = findLevelPrice(17);
    all_mid_price[10] = findLevelPrice(18);

    all_mid_weight[0] = 0.1663687378;
    all_mid_weight[1] = 0.08830163023;
    all_mid_weight[2] = 0.0528819044;
    all_mid_weight[3] = 0.09197787682;
    all_mid_weight[4] = 0.04789476096;
    all_mid_weight[5] = 0.2934834057;
    all_mid_weight[6] = 0.0695165548;
    all_mid_weight[7] = 0.06419924478;
    all_mid_weight[8] = 0.0957358527;
    all_mid_weight[9] = 0.02135353965;
    all_mid_weight[10] = 0.007984198967;
    double sum = 0;
    for (size_t i = 0; i < all_mid_weight.size(); i++) {
        sum += all_mid_weight[i];
    }
    for (size_t i = 0; i < all_mid_weight.size(); i++) {
        all_mid_weight[i] = all_mid_weight[i] / sum;
    }
    double benchmark = 0;
    for (size_t i = 0; i < all_mid_weight.size(); i++) {
        benchmark += all_mid_price[i] * all_mid_weight[i];
    }
    return benchmark;
}
void PriceLadderStructure::getAllMidpirce() {
    mid_price_[INDEX_LEVEL_1] = getRawMidpirce();
    getMidPrice();
    mid_price_[INDEX_LEVEL_2] = findLevelPrice(2);
    mid_price_[INDEX_LEVEL_4] = findLevelPrice(4);
    mid_price_[INDEX_LEVEL_6] = findLevelPrice(6);
    mid_price_[INDEX_LEVEL_8] = findLevelPrice(8);
    std::vector<price> benchmark_vector{ 0 };
    for (auto price : mid_price_) {
        benchmark_vector.push_back(
            { .price = price, .weight = 1.0 / MID_PRICE_AMOUNT });
    }
    for (int i = 0; i < price_.size(); i++) {
        price_[i].price = mid_price_[i];
    }

    double benchmark = GetWeightPrice(benchmark_vector);
    if (last_weight_price_ == 0) {
        // setPriceWeight({ .one_thousand = 0.15,
        //                  .three_thousand = 0.125,
        //                  .ten_thousand = 0.125,
        //                  .thirty_thousand = 0.075,
        //                  .one_hundred__thousand = 0.125,
        //                  .level_1 = 0.075,
        //                  .level_2 = 0.075,
        //                  .level_4 = 0.1,
        //                  .level_6 = 0.075,
        //                  .level_8 = 0.075 });
        setPriceWeight({ .one_thousand = 0.2476156,
                         .three_thousand = 0.07011157,
                         .ten_thousand = 0.10111826,
                         .thirty_thousand = 0,
                         .one_hundred__thousand = 0.29177928,
                         .level_1 = 0,
                         .level_2 = 0,
                         .level_4 = 0.07568804,
                         .level_6 = 0.16291712,
                         .level_8 = 0.04770308 });
    }
    //double weight_price = GetWeightPrice(price_);
    double weight_price = getTestMidpirce();
    double similarity = 0;
    if (last_weight_price_ != 0) {
        similarity = std::abs(benchmark - last_weight_price_);
    } else {
        last_weight_price_ = weight_price;
    }
    updataWeight(benchmark, similarity);


    for (int i = 0; i < MID_PRICE_AMOUNT; i++) {
        mid_last_price_[i] = mid_price_[i];
    }
    QString str = QString::fromStdString(getTimestamp());
    average_difference_number++;
    average_difference_sum += similarity;
    // static constexpr int INDEX_RAW{ 0 }; //0
    // static constexpr int INDEX_ONE_THOUSAND{ 1 }; //1000
    //  static constexpr int INDEX_THREE_THOUSAND{ 5 };//3000
    // static constexpr int INDEX_TEN_THOUSAND{ 2 };//10000
    // static constexpr int INDEX_THIRTY_THOUSAND{ 3 };//30000
    // static constexpr int INDEX_ONE_HUNDRED_THOUSAND{ 4 };//100000
    //
    // static constexpr int INDEX_LEVEL_1{ 6 };
    // static constexpr int INDEX_LEVEL_3{ 7 };
    // static constexpr int INDEX_LEVEL_5{ 8 };
    // static constexpr int INDEX_LEVEL_8{ 9 };
    //Time,"$1,000 ","$3,000 ","$10,000 ","$30,000 ","$100,000 ",1-level,2-level,4-level,6-level,8-level,buy,sell,ask,best ask,bid,best bid,benchmark price(each weight 0.1),last prediction,difference,next prediction,average difference,"$1,000 weight","$3,000 weight","$10,000 weight","$30,000 weight","$100,000 weight",1-level weight,2-level weight,4-level weight,6-level weight,8-level weight
    std::string format_ladder = getAllFormatLadder(20);
    std::streambuf* coutBuf = std::cout.rdbuf();
    static std::ofstream of("out33.txt");
    std::streambuf* fileBuf = of.rdbuf();
    std::cout.rdbuf(fileBuf);
    //std::cout << "_WebSocket server: \r\n";

    mid_price_40_[40] = price_[INDEX_ONE_THOUSAND].price;
    mid_price_40_[41] = price_[INDEX_THREE_THOUSAND].price;
    mid_price_40_[42] = price_[INDEX_TEN_THOUSAND].price;
    mid_price_40_[43] = price_[INDEX_THIRTY_THOUSAND].price;
    mid_price_40_[44] = price_[INDEX_ONE_HUNDRED_THOUSAND].price;
    std::vector<price> benchmark_vector1{ 0 };
    for (auto price : mid_price_40_) {
        benchmark_vector1.push_back({ .price = price, .weight = 1.0 / 45 });
    }
    double f = 0.0;
    for (auto price : mid_price_40_) {
        f += price;
    }
    double benchmark1 = f / 45;

    std::cout << price_[INDEX_ONE_THOUSAND].price << ","
              << price_[INDEX_THREE_THOUSAND].price << ","
              << price_[INDEX_TEN_THOUSAND].price << ","
              << price_[INDEX_THIRTY_THOUSAND].price << ","
              << price_[INDEX_ONE_HUNDRED_THOUSAND].price << ","
              << format_ladder << "," << benchmark1 << "\r\n";
    of.flush();

    // qDebug() << str << "," << price_[INDEX_ONE_THOUSAND].price << ","
    //          << price_[INDEX_THREE_THOUSAND].price << ","
    //          << price_[INDEX_TEN_THOUSAND].price << ","
    //          << price_[INDEX_THIRTY_THOUSAND].price << ","
    //          << price_[INDEX_ONE_HUNDRED_THOUSAND].price << ","
    //          << price_[INDEX_LEVEL_1].price << ","
    //          << price_[INDEX_LEVEL_2].price << ","
    //          << price_[INDEX_LEVEL_4].price << ","
    //          << price_[INDEX_LEVEL_6].price << ","
    //          << price_[INDEX_LEVEL_8].price << ","
    //          << price_[INDEX_LEVEL_1].price
    //                 - getThresholdPrice(price_[INDEX_LEVEL_1].price)
    //          << ","
    //          << price_[INDEX_LEVEL_1].price
    //                 + getThresholdPrice(price_[INDEX_LEVEL_1].price)
    //          << "," << asks_map_.begin()->first << ","
    //          << asks_map_.begin()->second << "," << -(bids_map_.begin()->first)
    //          << "," << bids_map_.begin()->second << "," << benchmark << ","
    //          << last_weight_price_ << "," << similarity << "," << weight_price
    //          << ",AVER:" << average_difference_sum / average_difference_number
    //          << "," << price_[INDEX_ONE_THOUSAND].weight << ","
    //          << price_[INDEX_THREE_THOUSAND].weight << ","
    //          << price_[INDEX_TEN_THOUSAND].weight << ","
    //          << price_[INDEX_THIRTY_THOUSAND].weight << ","
    //          << price_[INDEX_ONE_HUNDRED_THOUSAND].weight << ","
    //          << price_[INDEX_LEVEL_1].weight << ","
    //          << price_[INDEX_LEVEL_2].weight << ","
    //          << price_[INDEX_LEVEL_4].weight << ","
    //          << price_[INDEX_LEVEL_6].weight << ","
    //          << price_[INDEX_LEVEL_8].weight << "," << format_ladder.c_str();

    qDebug() << average_difference_sum / average_difference_number;

    last_weight_price_ = weight_price;

    return;
}
void PriceLadderStructure::onUpdataDidsMap(double price, double qty,
                                           double SeverTimestamp,
                                           bool isUpdataBook) {
    if (qty == 0) {
        bids_map_.erase(-price);
    } else {
        bids_map_[-price] = qty;
    }
    if (isUpdataBook) {
        if (qty == 0
            && interesting_prices_.find(price) != interesting_prices_.end()) {
            interesting_prices_.erase(price);
            return;
        }
        return;
        getMidPrice();
        double ap = sellAcceptablePrice();  //Sell -----
        // /&& interesting_prices_.find(price) == interesting_prices_.end()
        if (RestApi.balance_ > MIN_BALANCE
            && interesting_prices_.find(price) == interesting_prices_.end()
            && qty > 0 && price >= ap) {
            double b = RestApi.balance_ - (MIN_BALANCE);

            real_qty_ = std::min({ b, AMOUNT, qty });

            if (real_qty_ == 0 || RestApi.balance_ <= MIN_BALANCE) {
                return;
            }


            // double ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            //                 std::chrono::system_clock::now().time_since_epoch())
            //                 .count();

            // double i = (ms - SeverTimestamp);
            // std::cout << "time : " << i << "\r\n";
            // RestApi.order_queue_.push(std::make_tuple(price, real_qty_, false));
            RestApi.SendOrder(price, real_qty_, false);


            addInterestingPrice(price, qty, real_qty_, false, ap,
                                RestApi.balance_);
        }
    }
}
double PriceLadderStructure::findMaxMidpirce(int level) {
    int i = 0;
    double ask_sum = 0;
    double bid_sum = 0;

    for (auto it = asks_map_.begin(); it != asks_map_.end(); it++) {
        if (i >= level) {
            break;
        }
        ask_sum += it->second;
        i++;
    }
    i = 0;
    for (auto it = bids_map_.begin(); it != asks_map_.end(); it++) {
        if (i >= level) {
            break;
        }
        bid_sum += it->second;
        i++;
    }
    double max = std::max(ask_sum, bid_sum);
    return max;
}
double PriceLadderStructure::findLevelPrice(int level) {
    int i = 0;
    double ask_sum = 0;
    double bid_sum = 0;
    ask_sum = std::next(asks_map_.begin(), level)->first;
    bid_sum = -std::next(bids_map_.begin(), level)->first;

    double arge = (ask_sum + bid_sum) / 2;
    return arge;
}

double PriceLadderStructure::findMinMidpirce(int level) {
    int i = 0;
    double ask_sum = 0;
    double bid_sum = 0;

    for (auto it = asks_map_.begin(); it != asks_map_.end(); it++) {
        if (i >= level) {
            break;
        }
        ask_sum += it->second;
        i++;
    }
    i = 0;
    for (auto it = bids_map_.begin(); it != asks_map_.end(); it++) {
        if (i >= level) {
            break;
        }
        bid_sum += it->second;
        i++;
    }
    double min = std::min(ask_sum, bid_sum);
    return min;
}


double PriceLadderStructure::getThresholdPrice(double mid_price) {
    double fee_rate = 0.01 / 100;
    double fee = fee_rate * mid_price;
    double profit_rate = 0.01 / 100;
    double profit = profit_rate * mid_price;
    return profit + fee;
}
double PriceLadderStructure::buyAcceptablePrice() {
    double mid = mid_price_[0];
    return mid - getThresholdPrice(mid);
}
double PriceLadderStructure::sellAcceptablePrice() {
    double mid = mid_price_[1];
    double a = mid + getThresholdPrice(mid);

    return a;
}

double PriceLadderStructure::getSpecifiedMidpirce(double goalSum) {
    int sum = 0;
    double ask_number = 0;
    double bid_number = 0;
    for (auto it = asks_map_.begin(); it != asks_map_.end() && sum < goalSum;
         it++) {
        sum += it->second;

        ask_number = it->first;
    }

    sum = 0;
    for (auto it = bids_map_.begin(); it != bids_map_.end() && sum < goalSum;
         it++) {
        sum += it->second;

        bid_number = -it->first;
    }
    double midpirce = (ask_number + bid_number) / 2;
    return midpirce;
}
double PriceLadderStructure::getRawMidpirce() {
    double asks_first_price = asks_map_.begin()->first;

    double bid_first_price = -bids_map_.begin()->first;
    double mid = (asks_first_price + bid_first_price) / 2;
    return mid;
}
double* PriceLadderStructure::getMidPrice() {
    double ask_price_one_thousand = -1;          //1000
    double ask_price_three_thousand = -1;        //3000
    double ask_price_ten_thousand = -1;          //10000
    double ask_price_thirty_thousand = -1;       //30000
    double ask_price_one_huander_thousand = -1;  //100000
    // double ask_price_three_huander_thousand = -1;  //300000


    // const double MID_PARAMETER{ 1000 };
    // const double MID_PARAMETER_THREE_THOUSAND{ 3000 };
    // const double MID_PARAMETER_TEN_THOUSAND{ 10000 };
    // const double MID_PARAMETER_THIRTY_THOUSAND{ 30000 };
    // const double MID_PARAMETER_ONE_HUNDRED_THOUSAND{ 100000 };

    double sum = 0;
    for (auto it = asks_map_.begin();
         it != asks_map_.end() && sum < MID_PARAMETER_THREE_HUNDRED_THOUSAND;
         it++) {
        sum += it->second;
        //1000
        if ((sum >= MID_PARAMETER && sum < MID_PARAMETER_THREE_THOUSAND
             && ask_price_one_thousand == -1)
            || (sum >= MID_PARAMETER && ask_price_one_thousand == -1)) {
            ask_price_one_thousand = it->first;
        }
        //3000
        if ((sum >= MID_PARAMETER_THREE_THOUSAND
             && sum < MID_PARAMETER_TEN_THOUSAND
             && ask_price_three_thousand == -1)
            || (sum >= MID_PARAMETER_THREE_THOUSAND
                && ask_price_three_thousand == -1)) {
            ask_price_three_thousand = it->first;
        }
        //10000
        if ((sum >= MID_PARAMETER_TEN_THOUSAND
             && sum < MID_PARAMETER_THIRTY_THOUSAND
             && ask_price_ten_thousand == -1)
            || (sum >= MID_PARAMETER_TEN_THOUSAND
                && ask_price_ten_thousand == -1)) {
            ask_price_ten_thousand = it->first;
        }
        //30000
        if ((sum >= MID_PARAMETER_THIRTY_THOUSAND
             && sum < MID_PARAMETER_ONE_HUNDRED_THOUSAND
             && ask_price_thirty_thousand == -1)
            || (sum >= MID_PARAMETER_THIRTY_THOUSAND
                && ask_price_thirty_thousand == -1)) {
            ask_price_thirty_thousand = it->first;
        }
        //100000
        if ((sum >= MID_PARAMETER_ONE_HUNDRED_THOUSAND)
            || std::next(it) == asks_map_.end()) {
            ask_price_one_huander_thousand = it->first;
            break;
        }
        // //300000
        // if (sum >= MID_PARAMETER_THREE_HUNDRED_THOUSAND
        //     || std::next(it) == asks_map_.end()) {
        //     ask_price_three_huander_thousand = it->first;
        // }
    }

    double bid_price_one_thousand = -1;          //1000
    double bid_price_three_thousand = -1;        //3000
    double bid_price_ten_thousand = -1;          //10000
    double bid_price_thirty_thousand = -1;       //30000
    double bid_price_one_huander_thousand = -1;  //100000
    //double bid_price_three_huander_thousand = -1;  //300000
    // const double MID_PARAMETER{ 1000 };
    // const double MID_PARAMETER_THREE_THOUSAND{ 3000 };
    // const double MID_PARAMETER_TEN_THOUSAND{ 10000 };
    // const double MID_PARAMETER_THIRTY_THOUSAND{ 30000 };
    // const double MID_PARAMETER_ONE_HUNDRED_THOUSAND{ 100000 };
    sum = 0;
    for (auto it = bids_map_.begin();
         it != bids_map_.end() && sum < MID_PARAMETER_THREE_HUNDRED_THOUSAND;
         it++) {
        sum += it->second;
        //1000
        if ((sum >= MID_PARAMETER && sum < MID_PARAMETER_TEN_THOUSAND
             && bid_price_one_thousand == -1)
            || (sum >= MID_PARAMETER && bid_price_one_thousand == -1)) {
            bid_price_one_thousand = -it->first;
        }
        //3000
        if ((sum >= MID_PARAMETER_THREE_THOUSAND
             && sum < MID_PARAMETER_TEN_THOUSAND
             && bid_price_three_thousand == -1)
            || (sum >= MID_PARAMETER_THREE_THOUSAND
                && bid_price_three_thousand == -1)) {
            bid_price_three_thousand = -it->first;
        }
        //10000
        if ((sum >= MID_PARAMETER_TEN_THOUSAND
             && sum < MID_PARAMETER_THIRTY_THOUSAND
             && bid_price_ten_thousand == -1)
            || (sum >= MID_PARAMETER_TEN_THOUSAND
                && bid_price_ten_thousand == -1)) {
            bid_price_ten_thousand = -it->first;
        }
        //30000
        if ((sum >= MID_PARAMETER_THIRTY_THOUSAND
             && sum < MID_PARAMETER_ONE_HUNDRED_THOUSAND
             && bid_price_thirty_thousand == -1)
            || (sum >= MID_PARAMETER_THIRTY_THOUSAND
                && bid_price_thirty_thousand == -1)) {
            bid_price_thirty_thousand = -it->first;
        }
        //100000
        if ((sum >= MID_PARAMETER_ONE_HUNDRED_THOUSAND
             || std::next(it) == bids_map_.end())) {
            bid_price_one_huander_thousand = -it->first;
            break;
        }
        // //300000
        // if (sum >= MID_PARAMETER_THREE_HUNDRED_THOUSAND
        //     || std::next(it) == bids_map_.end()) {
        //     bid_price_three_huander_thousand = -it->first;
        // }
    }
    mid_price_[INDEX_ONE_THOUSAND] =
        (ask_price_one_thousand + bid_price_one_thousand) / 2;  //1000
    mid_price_[INDEX_THREE_THOUSAND] =
        (ask_price_three_thousand + bid_price_three_thousand) / 2;  //3000
    mid_price_[INDEX_TEN_THOUSAND] =
        (ask_price_ten_thousand + bid_price_ten_thousand) / 2;  //10000
    mid_price_[INDEX_THIRTY_THOUSAND] =
        (ask_price_thirty_thousand + bid_price_thirty_thousand) / 2;  //30000
    mid_price_[INDEX_ONE_HUNDRED_THOUSAND] =
        (ask_price_one_huander_thousand + bid_price_one_huander_thousand)
        / 2;  //100000

    // mid_price_[5] =
    //     (ask_price_three_huander_thousand + bid_price_three_huander_thousand)
    //     / 2;  //300000
    // mid_price_[0] = (ask_price_one_thousand + bid_price_one_thousand) / 2;
    // mid_price_[1] = (ask_price_ten_thousand + bid_price_ten_thousand) / 2;
    // mid_price_[2] =
    //     (ask_price_one_huander_thousand + bid_price_one_huander_thousand) / 2;

    return mid_price_;
}