project(Kapotah)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set (QT_USE_QTNETWORK TRUE)
set (QT_USE_QTXML TRUE)

set(Kapotah_SRCS
    ui/chatdialog.cpp
    outgoingtransfer.cpp
    incomingtransfer.cpp
    transfermanager.cpp
    outgoingtransferthread.cpp
    incomingtransferthread.cpp
    transferthread.cpp
    transfer.cpp
    ui/peerdialog.cpp
    peermanager.cpp
    peersmodel.cpp
    messagesenderthread.cpp
    messagedispatcher.cpp
    messagedispatcherserver.cpp
    messagedispatcherthread.cpp
    announcer.cpp
    peer.cpp
    xmlparser.cpp
    udpmanager.cpp
    tcpserver.cpp
    main.cpp)

SET(Kapotah_FORMS ui/peerdialog.ui ui/chatdialog.ui)

qt4_automoc(${Kapotah_SRCS})
QT4_WRAP_UI(Kapotah_FORMS_HEADERS ${Kapotah_FORMS})

add_executable(Kapotah ${Kapotah_SRCS} ${Kapotah_FORMS_HEADERS})
target_link_libraries(Kapotah ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY}
                                ${QT_QTXML_LIBRARY})
