TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    CatalogGAIA2.cpp \
    Program.cpp \
    AlgMatrix.cpp \
    PictureObject.cpp \
    CatalogMyGC.cpp

HEADERS += \
    Improvements.h \
    Algorithms.h \
    MathMatrix.h \
    matrix.h \
    Astronomy/Сatalog.h \
    Astronomy/СelestialBody.h \
    Astronomy/Fits.h \
    CatalogGAIA2.h \
    Program.h \
    AlgMatrix.h \
    PictureObject.h \
    CatalogMyGC.h
