project(Kapotah)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

set (QT_USE_QTNETWORK TRUE)
set (QT_USE_QTXML TRUE)

set (kapotah_RCCS systray.qrc)
qt4_add_resources(kapotah_RCC_SRCS ${kapotah_RCCS})

set(Kapotah_SRCS
    ui/transferdialog.ui
    filesearcherthread.cpp
    filesearcher.cpp
    xml/searchxmlparser.cpp
    ui/multicastdialog.cpp
    ui/searchdialog.cpp
    xml/transferstatusxmlparser.cpp
    xml/peerstatusxmlparser.cpp
    peerenvironment.cpp
    fileserversingleton.cpp
    xml/transferxmlparser.cpp
    xml/announcexmlparser.cpp
    xml/messagexmlparser.cpp
    ui/peerdialog.cpp
    ui/chatdialog.cpp
    ui/transferdialog.cpp
    ui/transferwidget.cpp
    fileserverthread.cpp
    outgoingtransfer.cpp
    incomingtransfer.cpp
    transfermanager.cpp
    outgoingtransferthread.cpp
    incomingtransferthread.cpp
    transferthread.cpp
    transfer.cpp
    peermanager.cpp
    peersmodel.cpp
    messagesenderthread.cpp
    messagedispatcher.cpp
    messagedispatcherserver.cpp
    messagedispatcherthread.cpp
    announcer.cpp
    peer.cpp
    udpmanager.cpp
    fileserver.cpp
    main.cpp
    ${kapotah_RCC_SRCS}
)

SET(Kapotah_FORMS
    ui/peerdialog.ui
    ui/chatdialog.ui
    ui/multicastdialog.ui
    ui/searchdialog.ui
    ui/transferdialog.ui
)

qt4_automoc(${Kapotah_SRCS})
QT4_WRAP_UI(Kapotah_FORMS_HEADERS ${Kapotah_FORMS})

add_executable(Kapotah WIN32 ${Kapotah_SRCS} ${Kapotah_FORMS_HEADERS})
target_link_libraries(Kapotah ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY}
                                ${QT_QTXML_LIBRARY} ${QT_QT_LIBRARY})

