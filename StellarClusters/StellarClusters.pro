TEMPLATE = app
CONFIG += console c++11 c++1z
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += g++

INCLUDEPATH += D:\MyC++\

CONFIG += debug

SOURCES += main.cpp \
    Catalog.cpp \
    CelestialBody.cpp \
    Processing.cpp \
    Program.cpp \
    neuralNetwork/Network.cpp \
    neuralNetwork/Akson.cpp \
    neuralNetwork/Neuron.cpp \
    neuralNetwork/Dendrit.cpp \
    Fits.cpp \
    GlobularCluster.cpp

HEADERS += \
    Catalog.h \
    CelestialBody.h \
    Improvements.h \
    Processing.h \
    Program.h \
    neuralNetwork/Network.h \
    neuralNetwork/Akson.h \
    neuralNetwork/Neuron.h \
    neuralNetwork/Dendrit.h \
    IN_conv_byVlad.h \
    IN_mnk_byVova.h \
    Fits.h \
    GlobularCluster.h
