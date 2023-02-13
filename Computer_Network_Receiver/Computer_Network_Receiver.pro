TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c

HEADERS += \
    Receiver_Application_Layer.h \
    Receiver_DataLink_Layer.h \
    Receiver_Network_Layer.h \
    Receiver_Transport_Layer.h \
    Tools.h
