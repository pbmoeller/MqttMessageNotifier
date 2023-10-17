// MqttMessageNotifier
#include "mainwindow.hpp"

// Qt
#include <QAction>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QVBoxLayout>

namespace mmn {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createTrayIcon();

    createContent();

    m_trayIcon->show();
    resize(400, 300);
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
    m_minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(m_minimizeAction, &QAction::triggered, this, &QWidget::hide);

    m_maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(m_maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    m_restoreAction = new QAction(tr("&Restore"), this);
    connect(m_restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    m_quitAction = new QAction(tr("&Quit"), this);
    connect(m_quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::createTrayIcon()
{
    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(m_minimizeAction);
    m_trayIconMenu->addAction(m_maximizeAction);
    m_trayIconMenu->addAction(m_restoreAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->setIcon(QIcon(":/32/MqttMessageNotifier32"));
}

void MainWindow::createContent()
{
    //QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(new QWidget);
    //setLayout(mainLayout);
}

void MainWindow::showMessage()
{

}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{

}

} // namespace mmn
