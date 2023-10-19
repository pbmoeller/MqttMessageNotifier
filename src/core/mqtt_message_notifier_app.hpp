#ifndef MMN_CORE_MQTT_MESSAGE_NOTIFIER_APP_HPP
#define MMN_CORE_MQTT_MESSAGE_NOTIFIER_APP_HPP

// MqttMessageNotifier
#include "ui/mainwindow.hpp"

// Qt
#include <QObject>

namespace mmn {

class MqttMessageNotifierApp : public QObject
{
    Q_OBJECT

public:
    explicit MqttMessageNotifierApp(QObject *parent = nullptr);
    ~MqttMessageNotifierApp();

    void create();

private:
    MainWindow m_mainWindow;
};

} // namespace mmn

#endif // MMN_CORE_MQTT_MESSAGE_NOTIFIER_APP_HPP
