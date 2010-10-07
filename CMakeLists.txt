project(Kapotah)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(Kapotah_SRCS
    messagesenderthread.cpp
    messagedispatcher.cpp
    messagedispatcherserver.cpp
    messagedispatcherthread.cpp
    announcer.cpp
    peer.cpp
    peermanager.cpp
    xmlparser.cpp
    broadcastmanager.cpp
    tcpserver.cpp
    main.cpp)

qt4_automoc(${Kapotah_SRCS})
add_executable(Kapotah ${Kapotah_SRCS})
target_link_libraries(Kapotah ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY}
                                ${QT_QTXML_LIBRARY})
