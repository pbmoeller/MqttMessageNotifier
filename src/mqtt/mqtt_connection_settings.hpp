#ifndef MMN_MQTT_MQTT_CONNECTION_SETTINGS_HPP
#define MMN_MQTT_MQTT_CONNECTION_SETTINGS_HPP

// STL
#include <string>

namespace mmn {

enum SslSetting
{
    NoSsl       = 0,
    EasySsl     = 1,
    ExtendedSsl = 2,
};

struct MqttConnectionSettings
{
    int port{0};
    std::string hostname;
    std::string username;
    std::string password;
    std::string clientId;
    SslSetting sslSetting;
};

} // namespace mmn

#endif // MMN_MQTT_MQTT_CONNECTION_SETTINGS_HPP
