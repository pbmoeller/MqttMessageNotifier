#ifndef MMN_UI_MQTT_TAB_WIDGET_HPP
#define MMN_UI_MQTT_TAB_WIDGET_HPP

// Qt
#include <QWidget>

// Qt Forward Declarations
QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QTreeWidget;
class QCheckBox;
class QLabel;
class QGridLayout;
class QRadioButton;
class QButtonGroup;
QT_END_NAMESPACE

namespace mmn {

class MqttConnection;

class MqttTabWidget : public QWidget
{
    Q_OBJECT

    enum TopicMatch
    {
        Success = 0,
        Error   = 1,
    };

public:
    explicit MqttTabWidget(QWidget *parent = nullptr);
    virtual ~MqttTabWidget();

public slots:
    void connect();
    void disconnect();
    void subscribe();
    void unsubscribe();
    void onMqttConnectionStatusChange(bool isConnected);
    void onMessage(const std::string &topic, const std::string &message);

signals:
    void connectionChanged(const QString &newName);
    void notify(const QString &topic, const QString &message);

private:
    void createContent();
    int topicMatchesSubscription(const char *sub, const char *topic, bool *result);

private:
    MqttConnection *m_mqttConnection;

    // Ui Left
    QPushButton     *m_connectButton;
    QPushButton     *m_disconnectButton;
    QLabel          *m_hostnameLabel;
    QLabel          *m_portLabel;
    QLabel          *m_usernameLabel;
    QLabel          *m_passwordLabel;
    QLabel          *m_clientIdLabel;
    QLineEdit       *m_hostnameEdit;
    QLineEdit       *m_portEdit;
    QLineEdit       *m_usernameEdit;
    QLineEdit       *m_passwordEdit;
    QLineEdit       *m_clientIdEdit;
    QRadioButton    *m_sslNoRadio;
    QRadioButton    *m_sslEasyRadio;
    QRadioButton    *m_sslExtendedRadio;
    QButtonGroup    *m_sslGroup;

    // Ui Right
    QGridLayout     *m_rightLayout;
    QLabel          *m_subscriptionLabel;
    QLineEdit       *m_subscriptionEdit;
    QLabel          *m_notificationLabel;
    QCheckBox       *m_notifyCheck;
    QPushButton     *m_subscribeButton;
    QTreeWidget     *m_subscriptionList;
};

} // namespace mmn

#endif // MMN_UI_MQTT_TAB_WIDGET_HPP
