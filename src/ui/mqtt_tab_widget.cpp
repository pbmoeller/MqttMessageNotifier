// MqttMessageNotifier
#include "mqtt_tab_widget.hpp"
#include "mqtt/mqtt_connection.hpp"
#include "mqtt/mqtt_connection_settings.hpp"

// Qt
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>

namespace mmn {

MqttTabWidget::MqttTabWidget(MqttConnection *mqttConnection, QWidget *parent)
    : QWidget(parent)
    , m_mqttConnection(mqttConnection)
{
    createContent();
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
    m_mqttConnection->addSubscription(m_subscriptionEdit->text().toStdString(), 1);
}

void MqttTabWidget::unsubscribe()
{

}

void MqttTabWidget::createContent()
{
    // Left Layout
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
    leftLayout->addWidget(new QLabel(tr("Hostname:")), 0, 0);
    leftLayout->addWidget(m_hostnameEdit, 0, 1);
    leftLayout->addWidget(new QLabel(tr("Port:")), 1, 0);
    leftLayout->addWidget(m_portEdit, 1, 1);
    leftLayout->addWidget(new QLabel(tr("Username:")), 2, 0);
    leftLayout->addWidget(m_usernameEdit, 2, 1);
    leftLayout->addWidget(new QLabel(tr("Password:")), 3, 0);
    leftLayout->addWidget(m_passwordEdit, 3, 1);
    leftLayout->addWidget(new QLabel(tr("ClientId:")), 4, 0);
    leftLayout->addWidget(m_clientIdEdit, 4, 1);
    leftLayout->addWidget(m_connectButton, 5, 1);
    leftLayout->addWidget(m_disconnectButton, 6, 1);
    leftLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding), 7, 2, 1 ,2);

    // Right Layout
    m_subscriptionEdit      = new QLineEdit;
    m_subscribeButton       = new QPushButton(tr("Subscribe"));
    QObject::connect(m_subscribeButton, &QPushButton::clicked, this, &MqttTabWidget::subscribe);
    m_subscriptionList      = new QListWidget;

    QGridLayout *rightLayout = new QGridLayout;
    rightLayout->addWidget(new QLabel(tr("Subscription:")), 0, 0);
    rightLayout->addWidget(m_subscriptionEdit, 0, 1);
    rightLayout->addWidget(m_subscribeButton, 1, 1);
    rightLayout->addWidget(m_subscriptionList, 2, 0, 1, 2);

    // Layout
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addLayout(leftLayout);
    hLayout->addLayout(rightLayout);
    setLayout(hLayout);
}

} // namespace mmn
