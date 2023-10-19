#ifndef MMN_MQTT_MQTT_CONNECTION_HPP
#define MMN_MQTT_MQTT_CONNECTION_HPP

// Qt
#include <QObject>

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
    void addSubscription();
    void removeSubscription();

private:
    std::string m_hostname;
};

} // namespace mmn

#endif // MMN_MQTT_MQTT_CONNECTION_HPP
