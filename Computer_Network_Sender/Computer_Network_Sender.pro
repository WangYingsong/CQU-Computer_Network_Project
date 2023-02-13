TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c

HEADERS += \
    Sender_Application_Layer.h \
    Sender_DataLink_Layer.h \
    Sender_Network_Layer.h \
    Sender_Transport_Layer.h \
    Tools.h
