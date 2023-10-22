set(APP_SOURCES
    "core/mqtt_message_notifier_app.cpp"

    "mqtt/mqtt_connection.cpp"

    "ui/mainwindow.cpp"
    "ui/mqtt_tab_style.cpp"
    "ui/mqtt_tab_widget.cpp"
)

set(APP_HEADER
    "core/mqtt_message_notifier_app.hpp"
    
    "mqtt/mqtt_connection.hpp"
    "mqtt/mqtt_connection_settings.hpp"

    "ui/mainwindow.hpp"
    "ui/mqtt_tab_style.hpp"
    "ui/mqtt_tab_widget.hpp"
)

set(APP_RESOURCES
    "${MqttMessageNotifier_SOURCE_DIR}/resources/resources.qrc"
)

add_executable(MqttMessageNotifierApp
    "main.cpp"
    ${APP_HEADER}
    ${APP_SOURCES}
    ${APP_RESOURCES}
)

target_include_directories(MqttMessageNotifierApp
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

target_link_libraries(MqttMessageNotifierApp
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        PahoMqttCpp::paho-mqttpp3
        eclipse-paho-mqtt-c::paho-mqtt3as
)

set_target_properties(MqttMessageNotifierApp
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    OUTPUT_NAME
        "MqttMessageNotifier"
)
