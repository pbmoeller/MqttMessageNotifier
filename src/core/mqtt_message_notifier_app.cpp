#include "mqtt_message_notifier_app.hpp"

namespace mmn {

MqttMessageNotifierApp::MqttMessageNotifierApp(QObject *parent)
    : QObject(parent)
    , m_mainWindow()
{

}

MqttMessageNotifierApp::~MqttMessageNotifierApp()
{

}

void MqttMessageNotifierApp::create()
{
    m_mainWindow.show();
}

} // namespace mmn
