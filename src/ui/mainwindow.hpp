#ifndef MMN_UI_MAINWINDOW_HPP
#define MMN_UI_MAINWINDOW_HPP

// Qt
#include <QMainWindow>
#include <QSystemTrayIcon>

// Qt Forward Declarations
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

namespace mmn {

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
    void createTrayIcon();
    void createContent();

private slots:
    void showMessage();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:

    // Actions
    QAction         *m_quitAction;
    QAction         *m_minimizeAction;
    QAction         *m_maximizeAction;
    QAction         *m_restoreAction;

    // TrayIcon
    QSystemTrayIcon *m_trayIcon;
    QMenu           *m_trayIconMenu;
};

} // namespace mmn

#endif // MMN_UI_MAINWINDOW_HPP
