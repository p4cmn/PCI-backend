QT = core network

CONFIG += c++17 cmdline

LIBS += -lkernel32

SOURCES += \
        main.cpp \
        controller\pcicontroller.cpp \
        database\pcidatabase.cpp \
        model\pcidevice.cpp \
        service\pciservice.cpp \
        server\tcpserver.cpp

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
  PciDefinitions.h \
  controller\pcicontroller.h \
  database\pcidatabase.h \
  model\pcidevice.h \
  service\pciservice.h \
  server\tcpserver.h

RESOURCES += \
  resources.qrc
