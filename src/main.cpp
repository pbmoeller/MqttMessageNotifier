// MqttMessageNotifier
#include "core/mqtt_message_notifier_app.hpp"

// Qt
#include <QApplication>
#include <QWidget>

// STL
#include <iostream>

int main(int argc, char **argv)
{
    std::cout << "Hello MqttMessageNotifier" << std::endl;

    QCoreApplication::setApplicationName("MqttMessageNotifier");

    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    // Create application window and start
    mmn::MqttMessageNotifierApp window;
    window.create();
    return app.exec();
}