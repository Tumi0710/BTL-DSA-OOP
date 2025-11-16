cmake_minimum_required(VERSION 3.16)

project(myMap LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tìm các module Qt6 cần thiết
find_package(Qt6 COMPONENTS Core Gui Widgets Sql REQUIRED)

# Bật tự động xử lý moc, uic, rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Thêm executable và các file
add_executable(myMap
    main.cpp
    mainwindow.cpp

    mainwindow.ui
    mainwindow.h   # Khai báo file UI
)

# Liên kết các thư viện Qt6
target_link_libraries(myMap
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
)
