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

namespace mmn {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createMenuBar();
    createTrayIcon();

    createContent();

    m_trayIcon->show();
    resize(640, 480);
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

    // Mqtt Actions
    m_addMqttConnection = new QAction(tr("Add Connection"));
    connect(m_addMqttConnection, &QAction::triggered, this, &MainWindow::addMqttConnection);
}

void MainWindow::createMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
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

void MainWindow::showMessage()
{

}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{

}

void MainWindow::addMqttConnection()
{
    int idx = m_tabWidget->addTab(new MqttTabWidget, "New Connection");
    MqttTabWidget* widget = static_cast<MqttTabWidget*>(m_tabWidget->widget(idx));
    connect(widget, &MqttTabWidget::connectionChanged, this, &MainWindow::connectionChanged);
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

} // namespace mmn
