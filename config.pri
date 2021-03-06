# Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR =  $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/main.cpp) \
                 $$quote($$BASEDIR/src/video_responder.cpp) \
                 $$quote($$BASEDIR/src/video_searcher.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/applicationui.hpp) \
                 $$quote($$BASEDIR/src/packedobjects/canon.h) \
                 $$quote($$BASEDIR/src/packedobjects/config.h) \
                 $$quote($$BASEDIR/src/packedobjects/decode.h) \
                 $$quote($$BASEDIR/src/packedobjects/encode.h) \
                 $$quote($$BASEDIR/src/packedobjects/expand.h) \
                 $$quote($$BASEDIR/src/packedobjects/ier.h) \
                 $$quote($$BASEDIR/src/packedobjects/packedobjects.h) \
                 $$quote($$BASEDIR/src/packedobjects/packedobjects_decode.h) \
                 $$quote($$BASEDIR/src/packedobjects/packedobjects_encode.h) \
                 $$quote($$BASEDIR/src/packedobjects/packedobjects_init.h) \
                 $$quote($$BASEDIR/src/packedobjects/schema.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/broker.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/config.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/global.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/md5_hash.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/message.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/packedobjectsd.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/request.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/response.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/xmlutils.h) \
                 $$quote($$BASEDIR/src/video_responder.h) \
                 $$quote($$BASEDIR/src/video_searcher.h)
    }

    CONFIG(release, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/main.cpp) \
                 $$quote($$BASEDIR/src/video_responder.cpp) \
                 $$quote($$BASEDIR/src/video_searcher.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/applicationui.hpp) \
                 $$quote($$BASEDIR/src/packedobjects/canon.h) \
                 $$quote($$BASEDIR/src/packedobjects/config.h) \
                 $$quote($$BASEDIR/src/packedobjects/decode.h) \
                 $$quote($$BASEDIR/src/packedobjects/encode.h) \
                 $$quote($$BASEDIR/src/packedobjects/expand.h) \
                 $$quote($$BASEDIR/src/packedobjects/ier.h) \
                 $$quote($$BASEDIR/src/packedobjects/packedobjects.h) \
                 $$quote($$BASEDIR/src/packedobjects/packedobjects_decode.h) \
                 $$quote($$BASEDIR/src/packedobjects/packedobjects_encode.h) \
                 $$quote($$BASEDIR/src/packedobjects/packedobjects_init.h) \
                 $$quote($$BASEDIR/src/packedobjects/schema.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/broker.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/config.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/global.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/md5_hash.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/message.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/packedobjectsd.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/request.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/response.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/xmlutils.h) \
                 $$quote($$BASEDIR/src/video_responder.h) \
                 $$quote($$BASEDIR/src/video_searcher.h)
    }
}

simulator {
    CONFIG(debug, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/main.cpp) \
                 $$quote($$BASEDIR/src/video_responder.cpp) \
                 $$quote($$BASEDIR/src/video_searcher.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/applicationui.hpp) \
                 $$quote($$BASEDIR/src/packedobjects/canon.h) \
                 $$quote($$BASEDIR/src/packedobjects/config.h) \
                 $$quote($$BASEDIR/src/packedobjects/decode.h) \
                 $$quote($$BASEDIR/src/packedobjects/encode.h) \
                 $$quote($$BASEDIR/src/packedobjects/expand.h) \
                 $$quote($$BASEDIR/src/packedobjects/ier.h) \
                 $$quote($$BASEDIR/src/packedobjects/packedobjects.h) \
                 $$quote($$BASEDIR/src/packedobjects/packedobjects_decode.h) \
                 $$quote($$BASEDIR/src/packedobjects/packedobjects_encode.h) \
                 $$quote($$BASEDIR/src/packedobjects/packedobjects_init.h) \
                 $$quote($$BASEDIR/src/packedobjects/schema.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/broker.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/config.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/global.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/md5_hash.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/message.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/packedobjectsd.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/request.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/response.h) \
                 $$quote($$BASEDIR/src/packedobjectsd/xmlutils.h) \
                 $$quote($$BASEDIR/src/video_responder.h) \
                 $$quote($$BASEDIR/src/video_searcher.h)
    }
}

INCLUDEPATH +=  $$quote($$BASEDIR/src/packedobjects) \
         $$quote($$BASEDIR/src/packedobjectsd) \
         $$quote($$BASEDIR/src)

CONFIG += precompile_header

PRECOMPILED_HEADER =  $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES +=  $$quote($$BASEDIR/../assets/images/*.c) \
             $$quote($$BASEDIR/../assets/images/*.c++) \
             $$quote($$BASEDIR/../assets/images/*.cc) \
             $$quote($$BASEDIR/../assets/images/*.cpp) \
             $$quote($$BASEDIR/../assets/images/*.cxx) \
             $$quote($$BASEDIR/../src/*.c) \
             $$quote($$BASEDIR/../src/*.c++) \
             $$quote($$BASEDIR/../src/*.cc) \
             $$quote($$BASEDIR/../src/*.cpp) \
             $$quote($$BASEDIR/../src/*.cxx) \
             $$quote($$BASEDIR/../assets/*.qml) \
             $$quote($$BASEDIR/../assets/*.js) \
             $$quote($$BASEDIR/../assets/*.qs)

    HEADERS +=  $$quote($$BASEDIR/../assets/images/*.h) \
             $$quote($$BASEDIR/../assets/images/*.h++) \
             $$quote($$BASEDIR/../assets/images/*.hh) \
             $$quote($$BASEDIR/../assets/images/*.hpp) \
             $$quote($$BASEDIR/../assets/images/*.hxx) \
             $$quote($$BASEDIR/../src/*.h) \
             $$quote($$BASEDIR/../src/*.h++) \
             $$quote($$BASEDIR/../src/*.hh) \
             $$quote($$BASEDIR/../src/*.hpp) \
             $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS =  $$quote($${TARGET}.ts)
