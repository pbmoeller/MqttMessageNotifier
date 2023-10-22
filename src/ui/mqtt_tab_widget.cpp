// MqttMessageNotifier
#include "mqtt_tab_widget.hpp"
#include "mqtt/mqtt_connection.hpp"
#include "mqtt/mqtt_connection_settings.hpp"

// Qt
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>

namespace mmn {

MqttTabWidget::MqttTabWidget(QWidget *parent)
    : QWidget(parent)
{
    m_mqttConnection = new MqttConnection(this);
    QObject::connect(m_mqttConnection, &MqttConnection::connectionStatusChanged,
                     this, &MqttTabWidget::onMqttConnectionStatusChange);
    QObject::connect(m_mqttConnection, &MqttConnection::messageArrived,
                     this, &MqttTabWidget::onMessage);

    createContent();
    onMqttConnectionStatusChange(false);
}

MqttTabWidget::~MqttTabWidget()
{
    disconnect();
}

void MqttTabWidget::connect()
{
    MqttConnectionSettings settings;
    settings.hostname = m_hostnameEdit->text().toStdString();
    settings.username = m_usernameEdit->text().toStdString();
    settings.password = m_passwordEdit->text().toStdString();
    settings.clientId = m_clientIdEdit->text().toStdString();
    settings.port     = m_portEdit->text().toInt();

    m_mqttConnection->connect(settings);
}

void MqttTabWidget::disconnect()
{
    m_mqttConnection->disconnect();
}

void MqttTabWidget::subscribe()
{
    QString sub = m_subscriptionEdit->text();
    QString notify = m_notifyCheck->isChecked() ? "true" : "false";
    m_mqttConnection->addSubscription(sub.toStdString(), 1);
    m_subscriptionList->addTopLevelItem(new QTreeWidgetItem({sub, notify}));
}

void MqttTabWidget::unsubscribe()
{

}

void MqttTabWidget::onMqttConnectionStatusChange(bool isConnected)
{
    // Left Layout
    m_hostnameLabel->setDisabled(isConnected);
    m_portLabel->setDisabled(isConnected);
    m_usernameLabel->setDisabled(isConnected);
    m_passwordLabel->setDisabled(isConnected);
    m_clientIdLabel->setDisabled(isConnected);
    m_hostnameEdit->setDisabled(isConnected);
    m_portEdit->setDisabled(isConnected);
    m_usernameEdit->setDisabled(isConnected);
    m_passwordEdit->setDisabled(isConnected);
    m_clientIdEdit->setDisabled(isConnected);
    m_connectButton->setDisabled(isConnected);
    m_connectButton->setDisabled(isConnected);
    m_disconnectButton->setEnabled(isConnected);

    // Right Layout
    m_subscriptionLabel->setEnabled(isConnected);
    m_subscriptionEdit->setEnabled(isConnected);
    m_notificationLabel->setEnabled(isConnected);
    m_notifyCheck->setEnabled(isConnected);
    m_subscribeButton->setEnabled(isConnected);
    m_subscriptionList->setEnabled(isConnected);

    // Notify listener with new broker name
    emit connectionChanged(QString(m_mqttConnection->getServerUri().c_str()));
}

void MqttTabWidget::onMessage(const std::string &topic, const std::string &message)
{
    emit notify(QString(topic.c_str()), QString(message.c_str()));
}

void MqttTabWidget::createContent()
{
    // Left Layout
    m_hostnameLabel     = new QLabel(tr("Hostname:"));
    m_portLabel         = new QLabel(tr("Port:"));
    m_usernameLabel     = new QLabel(tr("Username:"));
    m_passwordLabel     = new QLabel(tr("Password:"));
    m_clientIdLabel     = new QLabel(tr("ClientId:"));
    m_hostnameEdit      = new QLineEdit();
    m_portEdit          = new QLineEdit();
    m_usernameEdit      = new QLineEdit();
    m_passwordEdit      = new QLineEdit();
    m_clientIdEdit      = new QLineEdit();
    m_connectButton     = new QPushButton(tr("Connect"));
    QObject::connect(m_connectButton, &QPushButton::clicked, this, &MqttTabWidget::connect);
    m_disconnectButton  = new QPushButton(tr("Disconnect"));
    QObject::connect(m_disconnectButton, &QPushButton::clicked, this, &MqttTabWidget::disconnect);

    QGridLayout *leftLayout = new QGridLayout;
    leftLayout->addWidget(m_hostnameLabel, 0, 0);
    leftLayout->addWidget(m_hostnameEdit, 0, 1);
    leftLayout->addWidget(m_portLabel, 1, 0);
    leftLayout->addWidget(m_portEdit, 1, 1);
    leftLayout->addWidget(m_usernameLabel, 2, 0);
    leftLayout->addWidget(m_usernameEdit, 2, 1);
    leftLayout->addWidget(m_passwordLabel, 3, 0);
    leftLayout->addWidget(m_passwordEdit, 3, 1);
    leftLayout->addWidget(m_clientIdLabel, 4, 0);
    leftLayout->addWidget(m_clientIdEdit, 4, 1);
    leftLayout->addWidget(m_connectButton, 5, 1);
    leftLayout->addWidget(m_disconnectButton, 6, 1);
    leftLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding), 7, 2, 1 ,2);

    // Right Layout
    m_subscriptionEdit  = new QLineEdit;
    m_subscriptionLabel = new QLabel(tr("Subscription:"));
    m_subscribeButton   = new QPushButton(tr("Subscribe"));
    QObject::connect(m_subscribeButton, &QPushButton::clicked, this, &MqttTabWidget::subscribe);
    m_notificationLabel = new QLabel(tr("Notification:"));
    m_notifyCheck       = new QCheckBox;
    m_subscriptionList  = new QTreeWidget;
    m_subscriptionList->setColumnCount(2);
    m_subscriptionList->setHeaderLabels({"Topic", "Notification"});

    m_rightLayout = new QGridLayout;
    m_rightLayout->addWidget(m_subscriptionLabel, 0, 0);
    m_rightLayout->addWidget(m_subscriptionEdit, 0, 1);
    m_rightLayout->addWidget(m_notificationLabel, 1, 0);
    m_rightLayout->addWidget(m_notifyCheck, 1, 1);
    m_rightLayout->addWidget(m_subscribeButton, 2, 1);
    m_rightLayout->addWidget(m_subscriptionList, 3, 0, 1, 2);

    // Layout
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addLayout(leftLayout);
    hLayout->addLayout(m_rightLayout);
    setLayout(hLayout);
}

} // namespace mmn
