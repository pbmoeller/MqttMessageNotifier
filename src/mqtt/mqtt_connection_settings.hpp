#ifndef MMN_MQTT_MQTT_CONNECTION_SETTINGS_HPP
#define MMN_MQTT_MQTT_CONNECTION_SETTINGS_HPP

// STL
#include <string>
#include <vector>

namespace mmn {

enum SslSetting
{
    NoSsl       = 0,
    EasySsl     = 1,
    ExtendedSsl = 2,
};

struct Subscription
{
    std::string topic;
    bool notification;
};

struct MqttConnectionSettings
{
    int port{0};
    std::string hostname;
    std::string username;
    std::string password;
    std::string clientId;
    SslSetting sslSetting{NoSsl};
    std::vector<Subscription> subscriptions;
};

} // namespace mmn

#endif // MMN_MQTT_MQTT_CONNECTION_SETTINGS_HPP
