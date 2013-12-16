APP_NAME = VideoExamplePOD

CONFIG += qt warn_on cascades10

simulator {
 INCLUDEPATH += \
		${QNX_TARGET}/x86/usr/include/libxml2 \
		${QNX_TARGET}/x86/usr/include
LIBS+=-lpackedobjectsd -lxml2
}

device {
INCLUDEPATH += \
	${QNX_TARGET}/armle-v7/usr/include/libxml2 \
	${QNX_TARGET}/armle-v7/usr/include
	LIBS+=-lpackedobjectsd -lxml2
}

include(config.pri)
