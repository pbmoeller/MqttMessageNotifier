#ifndef MMN_UI_MQTT_TAB_WIDGET_HPP
#define MMN_UI_MQTT_TAB_WIDGET_HPP

// Qt
#include <QWidget>

// Qt Forward Declarations
QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QListWidget;
QT_END_NAMESPACE

namespace mmn {

class MqttConnection;

class MqttTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MqttTabWidget(QWidget *parent = nullptr);
    virtual ~MqttTabWidget();

public slots:
    void connect();
    void disconnect();
    void subscribe();
    void unsubscribe();

private:
    void createContent();

private:
    MqttConnection *m_mqttConnection;

    // Ui Left
    QPushButton *m_connectButton;
    QPushButton *m_disconnectButton;
    QLineEdit   *m_hostnameEdit;
    QLineEdit   *m_portEdit;
    QLineEdit   *m_usernameEdit;
    QLineEdit   *m_passwordEdit;
    QLineEdit   *m_clientIdEdit;

    // Ui Right
    QPushButton *m_subscribeButton;
    QLineEdit   *m_subscriptionEdit;
    QListWidget *m_subscriptionList;
};

} // namespace mmn

#endif // MMN_UI_MQTT_TAB_WIDGET_HPP
