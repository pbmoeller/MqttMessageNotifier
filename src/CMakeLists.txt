add_executable(MqttMessageNotifierApp
    "main.cpp"
)

set_target_properties(MqttMessageNotifierApp
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    OUTPUT_NAME
        "MqttMessageNotifier"
)
