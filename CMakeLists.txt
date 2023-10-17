cmake_minimum_required(VERSION 3.23)

project(MqttMessageNotifier
    VERSION 0.1.0
    LANGUAGES C CXX
)

# Set C/C++ Standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt specific variables
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Dependencies
find_package(Qt6
    REQUIRED
    COMPONENTS 
        Widgets
        Core
)

# Add the project sources
add_subdirectory(src)

