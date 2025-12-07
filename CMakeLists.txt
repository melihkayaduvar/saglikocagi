cmake_minimum_required(VERSION 3.16)

project(saglikocagi LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS LinguistTools Core Gui Sql Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS LinguistTools Core Gui Sql Widgets)

set(TS_FILES saglikocagi_tr_TR.ts)

# .ui dosyasını belirtin (Qt Designer ile oluşturulan dosya)
file(GLOB UI_FILES "ui/*.ui" "ui/veri-giris/*.ui" "ui/veri-liste/*.ui" "ui/veri-duzenleme/*.ui" "ui/veri-bilgi/*.ui")  # ui dosyalarını projenize dahil edin

file(GLOB SINIFLAR_SRC "ui/veri-bilgi/*.cpp" "ui/veri-bilgi/*.h" "ui/veri-duzenleme/*.cpp" "ui/veri-duzenleme/*.h" "ui/veri-liste/*.cpp" "ui/veri-liste/*.h" "ui/veri-giris/*.cpp" "ui/veri-giris/*.h" "ui/*.cpp" "ui/*.h" "Veri/Siniflar/*.cpp" "Veri/Siniflar/*.h" "Veri/Araclar/*.h" "Veri/*.cpp" "Veri/*.h")

add_executable(saglikocagi
  main.cpp
  ${TS_FILES}
  ${SINIFLAR_SRC}
  ${UI_FILES}
  kaynakdosyaları/resource.qrc
)
target_link_libraries(saglikocagi PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Sql
)

list(APPEND SOURCE_FILES kaynakdosyaları/resource.qrc)
if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

include(GNUInstallDirs)
install(TARGETS saglikocagi
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
