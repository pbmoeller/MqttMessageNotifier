#ifndef MMN_MQTT_MQTT_CONNECTION_SETTINGS_HPP
#define MMN_MQTT_MQTT_CONNECTION_SETTINGS_HPP

// STL
#include <string>

namespace mmn {

struct MqttConnectionSettings
{
    std::string hostname;
    int port;
    std::string username;
    std::string password;
    std::string clientId;
};

} // namespace mmn

#endif // MMN_MQTT_MQTT_CONNECTION_SETTINGS_HPP