#ifndef MMN_MQTT_MQTT_CONNECTION_LIST_HPP
#define MMN_MQTT_MQTT_CONNECTION_LIST_HPP

// MqttMessageNotifier
#include "mqtt_connection.hpp"

// Qt
#include <QObject>

// STL
#include <vector>

namespace mmn {

class MqttConnectionList : public QObject
{
    Q_OBJECT

public:
    MqttConnectionList() = delete;
    explicit MqttConnectionList(QObject *parent = nullptr);
    virtual ~MqttConnectionList();

public slots:
    MqttConnection* addMqttConnection();

private:
    std::vector<MqttConnection*> m_mqttConnections;
};

} // namespace mmn

#endif // MMN_MQTT_MQTT_CONNECTION_LIST_HPP
