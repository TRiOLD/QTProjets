TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    Astronomy/Fits.h \
    Astronomy/Сatalog.h \
    Astronomy/СelestialBody.h \
    matrix.h \
    MathMatrix.h \
    AlgorithmsMain.h
