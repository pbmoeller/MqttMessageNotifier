// MqttMessageNotifier
#include "mqtt_connection.hpp"
#include "mqtt_connection_settings.hpp"

// Paho MQTT
#include <mqtt/async_client.h>
#include <mqtt/properties.h>

// Qt
#include <QDebug>

namespace mmn {

MqttConnection::MqttConnection(QObject *parent)
    : QObject(parent)
{

}

MqttConnection::~MqttConnection()
{
    m_mqttClient->disconnect();
}

std::string MqttConnection::getServerUri() const
{
    return m_serverUri;
}

void MqttConnection::connect(const MqttConnectionSettings &settings)
{
    qDebug() << "Received connection request.";

    qDebug() << settings.hostname.c_str();
    qDebug() << settings.username.c_str();
    qDebug() << settings.password.c_str();
    qDebug() << settings.clientId.c_str();

    m_serverUri = settings.hostname + ":" + std::to_string(settings.port);
    m_mqttClient = std::make_unique<mqtt::async_client>(m_serverUri, settings.clientId);

    mqtt::connect_options opts = mqtt::connect_options_builder()
        .user_name(settings.username)
        .password(settings.password)
        .clean_session(true)
        .finalize();

    m_mqttClient->set_message_callback(std::bind(&MqttConnection::messageCallback,
                                                 this,
                                                 std::placeholders::_1));
    m_mqttClient->set_connected_handler(std::bind(&MqttConnection::connectedCallback,
                                                  this,
                                                  std::placeholders::_1));
    m_mqttClient->set_connection_lost_handler(std::bind(&MqttConnection::connectionLostCallback,
                                                  this,
                                                  std::placeholders::_1));
    m_mqttClient->set_disconnected_handler(std::bind(&MqttConnection::disconnectedCallback,
                                                     this,
                                                     std::placeholders::_1,
                                                     std::placeholders::_2));

    m_mqttClient->connect(opts);
}

void MqttConnection::disconnect()
{
    m_mqttClient->disconnect();
    emit connectionStatusChanged(false);
}

void MqttConnection::addSubscription(const std::string &topic, int qos)
{
    m_mqttClient->subscribe(topic, qos);
}

void MqttConnection::removeSubscription(const std::string &topic)
{
    m_mqttClient->unsubscribe(topic);
}

void MqttConnection::connectedCallback(const std::string& cause)
{
    emit connectionStatusChanged(true);
}

void MqttConnection::connectionLostCallback(const std::string& cause)
{
    emit connectionStatusChanged(false);
}

void MqttConnection::disconnectedCallback(const mqtt::properties&, mqtt::ReasonCode)
{
    emit connectionStatusChanged(false);
}

void MqttConnection::messageCallback(std::shared_ptr<const mqtt::message> message)
{
    qDebug() << message->get_payload().c_str();
}

} // namespace mmn
