QT = core network sql

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    qaesencryption.cpp \
    crypto.cpp \
    main.cpp \
    operations.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    aesni/aesni-key-init.h \
    crypto.h \
    qaesencryption.h \
    aesni/aesni-key-exp.h \
    aesni/aesni-enc-ecb.h \
    aesni/aesni-enc-cbc.h \
    operations.h
