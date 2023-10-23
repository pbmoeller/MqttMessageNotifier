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
    if(m_mqttClient != nullptr) {
        m_mqttClient->disconnect();
    }
}

std::string MqttConnection::getServerUri() const
{
    return m_serverUri;
}

bool MqttConnection::isConnected() const
{
    if(m_mqttClient == nullptr) {
        return false;
    }

    return m_mqttClient->is_connected();
}

void MqttConnection::connect(const MqttConnectionSettings &settings)
{
    // Create URI
    m_serverUri = settings.hostname + ":" + std::to_string(settings.port);

    // Set basic connection options
    mqtt::connect_options opts = mqtt::connect_options_builder()
        .user_name(settings.username)
        .password(settings.password)
        .clean_session(true)
        .finalize();

    // Set SSL options
    switch(settings.sslSetting)
    {
        case SslSetting::ExtendedSsl:
        {
            qDebug() << "Extended SSL is not supported at the moment.";
            [[fallthrough]];
        }
        case SslSetting::EasySsl:
        {
            m_serverUri = std::string("ssl://").append(m_serverUri);
            mqtt::ssl_options sslOptions = mqtt::ssl_options_builder()
                .verify(false)
                .enable_server_cert_auth(false)
                .error_handler([](const std::string &msg) {
                                    qDebug() << "MQTT SSL Error " << msg.c_str();
                               })
                .finalize();
            opts.set_ssl(std::move(sslOptions));
            break;
        }
        case SslSetting::NoSsl:
        default:
            break;
    }

    // Create client object
    m_mqttClient = std::make_unique<mqtt::async_client>(m_serverUri, settings.clientId);

    // Set all callbacks
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

    // Connect to broker
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
    emit messageArrived(message->get_topic(), message->get_payload());
}

} // namespace mmn
