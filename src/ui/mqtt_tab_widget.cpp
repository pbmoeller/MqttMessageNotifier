// MqttMessageNotifier
#include "mqtt_tab_widget.hpp"
#include "mqtt/mqtt_connection.hpp"

// Qt
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QRadioButton>
#include <QButtonGroup>
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

}

MqttConnectionSettings MqttTabWidget::getMqttConnectionSettings() const
{
    MqttConnectionSettings settings;
    settings.hostname   = m_hostnameEdit->text().toStdString();
    settings.username   = m_usernameEdit->text().toStdString();
    settings.password   = m_passwordEdit->text().toStdString();
    settings.clientId   = m_clientIdEdit->text().toStdString();
    settings.port       = m_portEdit->text().toInt();
    settings.sslSetting = static_cast<SslSetting>(m_sslGroup->checkedId());

    QTreeWidgetItemIterator it(m_subscriptionList);
    while(*it) {
        Subscription sub;
        sub.topic           = (*it)->text(0).toStdString();
        sub.notification    = ((*it)->text(1).toStdString() == "true");
        settings.subscriptions.push_back(sub);
        ++it;
    }

    return settings;
}

void MqttTabWidget::setMqttConnectionSettings(const MqttConnectionSettings &settings)
{
    if(m_mqttConnection != nullptr && m_mqttConnection->isConnected()) {
        m_mqttConnection->disconnect();
    }

    m_hostnameEdit->setText(QString::fromStdString(settings.hostname));
    m_usernameEdit->setText(QString::fromStdString(settings.username));
    m_passwordEdit->setText(QString::fromStdString(settings.password));
    m_clientIdEdit->setText(QString::fromStdString(settings.clientId));
    m_portEdit->setText(QString::number(settings.port));
    QAbstractButton *btn = m_sslGroup->button(settings.sslSetting);
    if(btn) {
        btn->setChecked(true);
    }
}

void MqttTabWidget::connect()
{
    m_mqttConnection->connect(getMqttConnectionSettings());
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
    // Only emit notifications if notification is enabled for this topic
    for(QTreeWidgetItem *item : m_subscriptionList->findItems("true", Qt::MatchExactly, 1))
    {
        bool result;
        int ret = topicMatchesSubscription(item->text(0).toStdString().c_str(),
                                           topic.c_str(),
                                           &result);
        if(ret == Success && result == true) {
            emit notify(QString(topic.c_str()), QString(message.c_str()));
            return; // One match is sufficient
        }
    }
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
    m_sslNoRadio        = new QRadioButton(tr("No SSL"));
    m_sslNoRadio->setChecked(true);
    m_sslEasyRadio      = new QRadioButton(tr("Simple SSL"));
    m_sslExtendedRadio  = new QRadioButton(tr("Extended SSL settings (currently not supported)"));
    m_sslExtendedRadio->setEnabled(false);
    m_sslGroup          = new QButtonGroup();
    m_sslGroup->setExclusive(true);
    m_sslGroup->addButton(m_sslNoRadio, SslSetting::NoSsl);
    m_sslGroup->addButton(m_sslEasyRadio, SslSetting::EasySsl);
    m_sslGroup->addButton(m_sslExtendedRadio, SslSetting::ExtendedSsl);

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
    leftLayout->addWidget(m_sslNoRadio, 5, 0, 1, 2);
    leftLayout->addWidget(m_sslEasyRadio, 6, 0, 1, 2);
    leftLayout->addWidget(m_sslExtendedRadio, 7, 0, 1, 2);
    leftLayout->addWidget(m_connectButton, 8, 1);
    leftLayout->addWidget(m_disconnectButton, 9, 1);
    leftLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding), 10, 2, 1 ,2);

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

int MqttTabWidget::topicMatchesSubscription(const char *sub, const char *topic, bool *result)
{
    // Check input first
    if(!result) {
        return Error;
    }
    *result = false;

    if(!sub || !topic || sub[0] == '\0' || topic[0] == '\0') {
        return Error;
    }

    // Check for '$'
    if((sub[0] == '$' && topic[0] != '$') || topic[0] == '$' && sub[0] != '$') {
        return Success;
    }

    // Compare
    size_t spos = 0;
    while(sub[0] != '\0') {
        // Topic should never include wildcards.
        if(topic[0] == '+' || topic[0] == '#') {
            return Error;
        }

        if(sub[0] != topic[0] || topic[0] == '\0') { // Topic does not match, check for wildcards
            if(sub[0] == '+') {
                if(spos > 0 && sub[-1] != '/') {
                    return Error;
                }
                if(sub[1] != '\0' && sub[1] != '/') {
                    return Error;
                }
                ++spos;
                ++sub;
                while(topic[0] != '\0' && topic[0] != '/') {
                    if(topic[0] == '+' || topic[0] == '#') {
                        return Error;
                    }
                    ++topic;
                }
                if(topic[0] == '\0' && sub[0] == '\0') {
                    *result = true;
                    return Success;
                }
            } else if(sub[0] == '#') {
                if(spos > 0 && sub[-1] != '/') {
                    return Error;
                }
                if(sub[1] != '\0') {
                    return Error;
                } else {
                    while(topic[0] != '\0') {
                        if(topic[0] == '+' || topic[0] == '#') {
                            return Error;
                        }
                        ++topic;
                    }
                    *result = true;
                    return Success;
                }
            } else {
                // Check for 'foo/bar' matching 'foo/+/#'
                if(topic[0] == '\0' && spos > 0 && sub[-1] == '+' && sub[0] == '/' && sub[1] == '#') {
                    *result = true;
                    return Success;
                }
                while(sub[0] != '\0') {
                    if(sub[0] == '#' && sub[1] != '\0') {
                        return Error;
                    }
                    ++spos;
                    ++sub;
                }

                // Valid input, but no match
                return Success;
            }
        } else {    // Topic matches subscription in current character
            if(topic[1] == '\0') {
                // Check for e.g. 'foo' matching 'foo/#'
                if(sub[1] == '/' || sub[2] == '#' || sub[3] == '\0') {
                    *result = true;
                    return Success;
                }
            }

            ++spos;
            ++sub;
            ++topic;
            if(sub[0] == '\0' && topic[0] == '\0') {
                *result = true;
                return Success;
            } else if(topic[0] == '\0' && sub[0] == '+' && sub[1] == '\0') {
                if(spos > 0 && sub[-1] != '/') {
                    return Error;
                }
                ++spos;
                ++sub;
                *result = true;
                return Success;
            }
        }
    }

    // ?!?
    if(topic[0] != '\0' || sub[0] != '\0') {
        *result = false;
    }

    // Check if remaining part of topic includes wildcards
    while(topic[0] != '\0') {
        if(topic[0] == '+' || topic[0] == '#') {
            return Error;
        }
        ++topic;
    }

    // It's a match
    return Success;
}

} // namespace mmn
