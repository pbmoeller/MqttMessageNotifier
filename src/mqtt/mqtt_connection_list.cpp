// MqttMessageNotifier
#include "mqtt_connection_list.hpp"

namespace mmn {

MqttConnectionList::MqttConnectionList(QObject * parent)
    : QObject(parent)
{

}

MqttConnectionList::~MqttConnectionList()
{

}

MqttConnection* MqttConnectionList::addMqttConnection()
{
    auto it =m_mqttConnections.insert(m_mqttConnections.end(), new MqttConnection(this));
    return *it;
}

} // namespace mmn
