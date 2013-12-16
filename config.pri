# Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR =  $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/main.cpp) \
                 $$quote($$BASEDIR/src/video_responder.cpp) \
                 $$quote($$BASEDIR/src/video_searcher.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/applicationui.hpp) \
                 $$quote($$BASEDIR/src/video_responder.h) \
                 $$quote($$BASEDIR/src/video_searcher.h)
    }

    CONFIG(release, debug|release) {
        INCLUDEPATH +=  $$quote("${QNX_TARGET}/${CPUVARDIR}/usr/include")

        DEPENDPATH +=  $$quote("${QNX_TARGET}/${CPUVARDIR}/usr/include")

        SOURCES +=  $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/main.cpp) \
                 $$quote($$BASEDIR/src/video_responder.cpp) \
                 $$quote($$BASEDIR/src/video_searcher.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/applicationui.hpp) \
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
                 $$quote($$BASEDIR/src/video_responder.h) \
                 $$quote($$BASEDIR/src/video_searcher.h)
    }
}

INCLUDEPATH +=  $$quote($$BASEDIR/src)

CONFIG += precompile_header

PRECOMPILED_HEADER =  $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES +=  $$quote($$BASEDIR/../src/*.c) \
             $$quote($$BASEDIR/../src/*.c++) \
             $$quote($$BASEDIR/../src/*.cc) \
             $$quote($$BASEDIR/../src/*.cpp) \
             $$quote($$BASEDIR/../src/*.cxx) \
             $$quote($$BASEDIR/../assets/*.qml) \
             $$quote($$BASEDIR/../assets/*.js) \
             $$quote($$BASEDIR/../assets/*.qs)

    HEADERS +=  $$quote($$BASEDIR/../src/*.h) \
             $$quote($$BASEDIR/../src/*.h++) \
             $$quote($$BASEDIR/../src/*.hh) \
             $$quote($$BASEDIR/../src/*.hpp) \
             $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS =  $$quote($${TARGET}.ts)
