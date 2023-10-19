// MqttMessageNotifier
#include "mqtt_connection.hpp"
#include "mqtt_connection_settings.hpp"

// Qt
#include <QDebug>

namespace mmn {

MqttConnection::MqttConnection(QObject *parent)
    : QObject(parent)
{

}

MqttConnection::~MqttConnection()
{

}

void MqttConnection::connect(const MqttConnectionSettings &settings)
{
    qDebug() << "Received connection request.";

    qDebug() << settings.hostname.c_str();
    qDebug() << settings.username.c_str();
    qDebug() << settings.password.c_str();
    qDebug() << settings.clientId.c_str();
}

void MqttConnection::disconnect()
{

}

void MqttConnection::addSubscription()
{

}

void MqttConnection::removeSubscription()
{

}

} // namespace mmn
