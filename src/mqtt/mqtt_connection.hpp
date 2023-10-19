#ifndef MMN_MQTT_MQTT_CONNECTION_HPP
#define MMN_MQTT_MQTT_CONNECTION_HPP

// Qt
#include <QObject>

// STL
#include <memory>

namespace mqtt {
class async_client;
class message;
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
    void connect(const MqttConnectionSettings &settings);
    void disconnect();
    void addSubscription(const std::string &topic, int qos);
    void removeSubscription();

    void messageCallback(std::shared_ptr<const mqtt::message> message);

private:
    std::string m_hostname;

    std::unique_ptr<mqtt::async_client> m_mqttClient;
};

} // namespace mmn

#endif // MMN_MQTT_MQTT_CONNECTION_HPP
