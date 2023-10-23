// MqttMessageNotifier
#include "mainwindow.hpp"
#include "mqtt_tab_style.hpp"
#include "mqtt_tab_widget.hpp"

// Qt
#include <QAction>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QTabBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QStandardPaths>
#include <QSettings>

namespace mmn {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    appSavePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    createActions();
    createMenuBar();
    createTrayIcon();

    createContent();

    m_trayIcon->show();
    resize(640, 480);

    setWindowTitle("MqttMessageNotifier");
    restoreSettings();
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::setVisible(bool visible)
{
    m_minimizeAction->setEnabled(visible);
    m_maximizeAction->setEnabled(!isMaximized());
    QMainWindow::setVisible(visible);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(m_trayIcon->isVisible()) {
        QMessageBox::information(this,
                                 tr("MqttMessageNotifier"),
                                 tr("The application will keep running in the background. "
                                    "To terminate the application choose <b>Quit</b> in"
                                    "the context menu of the system tray entry."));
        hide();
        event->ignore();
    }
}

void MainWindow::createActions()
{
    // General Actions
    m_minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(m_minimizeAction, &QAction::triggered, this, &QWidget::hide);

    m_maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(m_maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    m_restoreAction = new QAction(tr("&Restore"), this);
    connect(m_restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    m_quitAction = new QAction(tr("&Quit"), this);
    connect(m_quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    m_saveAction = new QAction(tr("&Save"), this);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveSettings);

    // Mqtt Actions
    m_addMqttConnection = new QAction(tr("Add Connection"));
    connect(m_addMqttConnection, &QAction::triggered, this, &MainWindow::addMqttConnection);
}

void MainWindow::createMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_quitAction);

    QMenu *mqttMenu = menuBar()->addMenu(tr("&Mqtt"));
    mqttMenu->addAction(m_addMqttConnection);
}

void MainWindow::createTrayIcon()
{
    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(m_minimizeAction);
    // m_trayIconMenu->addAction(m_maximizeAction);
    m_trayIconMenu->addAction(m_restoreAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->setIcon(QIcon(":/32/MqttMessageNotifier32"));
}

void MainWindow::createContent()
{
    m_tabWidget = new QTabWidget;
    m_tabWidget->setTabPosition(QTabWidget::TabPosition::West);
    m_tabWidget->tabBar()->setStyle(new MqttTabStyle);

    setCentralWidget(m_tabWidget);
}

void MainWindow::showMessage(const QString &title, const QString &body)
{
    m_trayIcon->showMessage(title, body, QIcon(":/64/MqttMessageNotifier64"));
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{

}

MqttTabWidget* MainWindow::addMqttConnection()
{
    int idx = m_tabWidget->addTab(new MqttTabWidget, "New Connection");
    MqttTabWidget* widget = static_cast<MqttTabWidget*>(m_tabWidget->widget(idx));
    connect(widget, &MqttTabWidget::connectionChanged, this, &MainWindow::connectionChanged);
    connect(widget, &MqttTabWidget::notify, this, &MainWindow::showMessage);

    return widget;
}

void MainWindow::connectionChanged(const QString &newName)
{
    MqttTabWidget* tabWidget = qobject_cast<MqttTabWidget*>(sender());
    if(tabWidget != nullptr)
    {
        int idx = m_tabWidget->indexOf(tabWidget);
        if(idx >= 0) {
            m_tabWidget->tabBar()->setTabText(idx, newName);
        }
    }
}

void MainWindow::saveSettings()
{
    QSettings appSettings(QString("%1/%2.ini").arg(appSavePath, QCoreApplication::applicationName()),
                          QSettings::IniFormat);

    appSettings.beginGroup("mqtt");
    appSettings.beginWriteArray("connections");
    for(int idx = 0; idx < m_tabWidget->count(); ++idx) {
        MqttTabWidget* widget = static_cast<MqttTabWidget*>(m_tabWidget->widget(idx));
        MqttConnectionSettings settings = widget->getMqttConnectionSettings();

        appSettings.setArrayIndex(idx);
        appSettings.setValue("hostname",    settings.hostname.c_str());
        appSettings.setValue("port",        settings.port);
        appSettings.setValue("username",    settings.username.c_str());
        appSettings.setValue("password",    settings.password.c_str());
        appSettings.setValue("clientId",    settings.clientId.c_str());
        appSettings.setValue("ssl",         static_cast<int>(settings.sslSetting));
        appSettings.beginWriteArray("subscriptions");
        for(int subIdx = 0; subIdx < settings.subscriptions.size(); ++subIdx) {
            appSettings.setArrayIndex(subIdx);
            appSettings.setValue("topic",           settings.subscriptions[subIdx].topic.c_str());
            appSettings.setValue("notification",    settings.subscriptions[subIdx].notification);
        }
        appSettings.endArray(); // subscriptions
    }
    appSettings.endArray(); // connections
    appSettings.endGroup(); // mqtt
}

void MainWindow::restoreSettings()
{
    QSettings appSettings(QString("%1/%2.ini").arg(appSavePath, QCoreApplication::applicationName()),
                          QSettings::IniFormat);

    appSettings.beginGroup("mqtt");
    int size = appSettings.beginReadArray("connections");
    for(int idx = 0; idx < size; ++idx) {
        appSettings.setArrayIndex(idx);
        MqttConnectionSettings settings;
        settings.hostname   = appSettings.value("hostname", QString()).toString().toStdString();
        settings.username   = appSettings.value("username", QString()).toString().toStdString();
        settings.password   = appSettings.value("password", QString()).toString().toStdString();
        settings.clientId   = appSettings.value("clientId", QString()).toString().toStdString();
        settings.port       = appSettings.value("port",     1883).toInt();
        settings.sslSetting = static_cast<SslSetting>(appSettings.value("ssl", NoSsl).toInt());
        int subSize = appSettings.beginReadArray("subscriptions");
        for(int subIdx = 0; subIdx < size; ++subIdx) {
            Subscription sub;
            sub.topic           = appSettings.value("topic", QString()).toString().toStdString();
            sub.notification    = appSettings.value("notification", false).toBool();
            settings.subscriptions.push_back(sub);
        }
        appSettings.endArray(); // subscriptions

        MqttTabWidget *widget = addMqttConnection();
        widget->setMqttConnectionSettings(settings);
    }

    appSettings.endArray(); // connections
    appSettings.endGroup(); // mqtt
}

} // namespace mmn
