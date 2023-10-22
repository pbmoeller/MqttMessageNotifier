#ifndef MMN_MQTT_MQTT_CONNECTION_HPP
#define MMN_MQTT_MQTT_CONNECTION_HPP

// Paho Mqtt
#include <mqtt/types.h>

// Qt
#include <QObject>

// STL
#include <memory>

namespace mqtt {
class async_client;
class message;
class properties;
} // namespace mqtt

namespace mmn {

struct MqttConnectionSettings;

class MqttConnection : public QObject
{
    Q_OBJECT

public:
    MqttConnection() = delete;
    explicit MqttConnection(QObject *parent = nullptr);
    virtual ~MqttConnection();

public:
    std::string getServerUri() const;

    void connect(const MqttConnectionSettings &settings);
    void disconnect();
    void addSubscription(const std::string &topic, int qos);
    void removeSubscription(const std::string &topic);

    // Callbacks for asny_client
    void connectedCallback(const std::string& cause);
    void connectionLostCallback(const std::string& cause);
    void disconnectedCallback(const mqtt::properties&, mqtt::ReasonCode);
    void messageCallback(std::shared_ptr<const mqtt::message> message);

signals:
    void connectionStatusChanged(bool isConnected);

private:
    std::string m_serverUri;

    std::unique_ptr<mqtt::async_client> m_mqttClient;
};

} // namespace mmn

#endif // MMN_MQTT_MQTT_CONNECTION_HPP
