#ifndef MMN_UI_MAINWINDOW_HPP
#define MMN_UI_MAINWINDOW_HPP

// Qt
#include <QMainWindow>

namespace mmn {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
};

} // namespace mmn

#endif // MMN_UI_MAINWINDOW_HPP
