set(APP_SOURCES
    "core/mqtt_message_notifier_app.cpp"

    "ui/mainwindow.cpp"
)

set(APP_HEADER
    "core/mqtt_message_notifier_app.hpp"

    "ui/mainwindow.hpp"
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
