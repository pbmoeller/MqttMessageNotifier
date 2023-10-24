#ifndef MMN_UI_MAINWINDOW_HPP
#define MMN_UI_MAINWINDOW_HPP

// Qt
#include <QMainWindow>
#include <QSystemTrayIcon>

// Qt Forward Declarations
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTabWidget;
QT_END_NAMESPACE

namespace mmn {

class MqttTabWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setVisible(bool visible) override;

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void createActions();
    void createMenuBar();
    void createTrayIcon();
    void createContent();

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    MqttTabWidget* addMqttConnection();

public slots:
    void connectionChanged(const QString &newName);
    void showMessage(const QString &title, const QString &body);
    void saveSettings();
    void restoreSettings();
    void clearAll();
    void removeConnection();

private:
    // Settings
    QString appSavePath;

    // Widgets
    QTabWidget          *m_tabWidget;

    // Actions
    QAction             *m_quitAction;
    QAction             *m_minimizeAction;
    QAction             *m_maximizeAction;
    QAction             *m_restoreAction;
    QAction             *m_saveAction;

    // Mqtt Actions
    QAction             *m_removeConnectionAction;
    QAction             *m_clearAction;
    QAction             *m_addMqttConnection;

    // TrayIcon
    QSystemTrayIcon     *m_trayIcon;
    QMenu               *m_trayIconMenu;
};

} // namespace mmn

#endif // MMN_UI_MAINWINDOW_HPP
