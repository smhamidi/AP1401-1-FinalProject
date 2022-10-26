#include "network.h"


BroadCast::BroadCast(QThread* parent) : QThread{parent},
    ip{extern_config.broadcast_ip}, port{extern_config.broadcast_port}, board{extern_wm.board}, blue{extern_wm.blue}, red{extern_wm.red}
{
    udp = new QUdpSocket{};
}

BroadCast::~BroadCast()
{
    delete udp;
}

void BroadCast::run()
{
    QJsonObject main_json;
    main_json.insert("game_state", QJsonValue::fromVariant(state_to_text(extern_gamestate)));

    main_json.insert("blue", QJsonValue::fromVariant(extern_config.blueteam_name));
    main_json.insert("red", QJsonValue::fromVariant(extern_config.redteam_name));

    // QJsonArray blue_tail;
    // blue_tail.push_back(blue.tail.x());
    // blue_tail.push_back(blue.tail.y());
    // main_json.insert("1_tail", blue_tail);
    // QJsonArray red_tail;
    // red_tail.push_back(red.tail.x());
    // red_tail.push_back(red.tail.y());
    // main_json.insert("2_tail", red_tail);

    main_json.insert("blue_score", QJsonValue::fromVariant(blue.score));
    main_json.insert("red_score", QJsonValue::fromVariant(red.score));

    QJsonArray blue_heads;
    auto tmp = blue.head;
    while(!tmp.empty())
    {
        QJsonArray head;
        head.push_back(tmp.top().x());
        head.push_back(tmp.top().y());
        blue_heads.push_back(head);
        tmp.pop();
    }
    main_json.insert("blue_heads", blue_heads);

    QJsonArray red_heads;
    tmp = red.head;
    while(!tmp.empty())
    {
        QJsonArray head;
        head.push_back(tmp.top().x());
        head.push_back(tmp.top().y());
        red_heads.push_back(head);
        tmp.pop();
    }
    main_json.insert("red_heads", red_heads);

    QJsonDocument doc(main_json);
    QByteArray dgram = doc.toJson(QJsonDocument::Compact);
    
    // std::cout << dgram.toStdString() << std::endl;
    auto ret = udp->writeDatagram(dgram.data(), dgram.size(), QHostAddress{ip}, 1234);
    // qDebug() << ret << dgram.size();
}