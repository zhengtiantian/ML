#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>
#include <iostream>
#include "kraken_websocket.h"

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription("Kraken example");
    parser.addHelpOption();
    QCommandLineOption dbgOption(
        QStringList() << "d"
                      << "debug",
        QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    parser.process(a);
    bool debug = true;
    //api.futures.kraken.com/ws/v1
    //wss://futures.kraken.com/ws/v1
    //https://demo-futures.kraken.com
    KrakenWebsocket client(
        QUrl(QStringLiteral("wss://futures.kraken.com/ws/v1")), debug);
    QObject::connect(&client, &KrakenWebsocket::closed, &a,
                     &QCoreApplication::quit);

    try {
        return a.exec();
    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }
}