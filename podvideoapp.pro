APP_NAME = podvideoapp

CONFIG += qt warn_on cascades10

simulator {
 	LIBS+=-L${PWD}library-dependencies/x86 -lpackedobjectsd -lxml2 -lzmq -lpackedobjects -lbbplatform
}

device {
	LIBS+=-L${PWD}library-dependencies/armle-v7 -lpackedobjectsd -lxml2 -lzmq -lpackedobjects -lbbplatform
}

include(config.pri)
