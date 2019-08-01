TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

win32 {
    DEFINES += MSBUILD
}
## VS 2012 Compiler Flag additions. all pro files should be implemented these lines
win32:CONFIG(debug, debug|release|profile): QMAKE_CXXFLAGS += /GS /W3 /Gm /Od /sdl /WX- /Zc:forScope /RTC1 /Gd /Oy- /D "_DEBUG"
else:win32:CONFIG(release, debug|release|profile): QMAKE_CXXFLAGS += /GS /GL /analyze- /Gm- /W3 /Gy /Ox /D"NDEBUG" /sdl /Gd /Oy- /Oi
else:win32:CONFIG(profile, debug|release|profile): QMAKE_CXXFLAGS += /GS /GL /analyze- /Gm- /W3 /Gy /Ox /D"NDEBUG" /sdl /Gd /Oy- /Oi
win32:CONFIG(debug, debug|release|profile): QMAKE_LFLAGS +=
else:win32:CONFIG(release, debug|release|profile): QMAKE_LFLAGS += /SAFESEH /LTCG
else:win32:CONFIG(profile, debug|release|profile): QMAKE_LFLAGS += /SAFESEH /LTCG

INCLUDEPATH += \
    inc \
    src/1-Inheritance  \
    src/2-SmartPointer \
    src/3-Static \
    src/4-CPUArchitecture \
    src/5-Templates \
    src/6-Exceptions \
    src/7-CustomComparators \
    src/8-Cpp11Features

SOURCES += \
        main.cpp \
        src/2-SmartPointer/bonus/shared.cpp

HEADERS += \
    inc/no_destruct.h \
    inc/number.hpp \
    inc/playground_organizer.hpp \
    src/1-Inheritance/diamond_problem.hpp \
    src/1-Inheritance/friend_usage.hpp \
    src/1-Inheritance/inheritance_usage.hpp \
    src/1-Inheritance/virtual_destructor.hpp \
    src/2-SmartPointer/bonus/bonus_old_style_smart_pointer.hpp \
    src/2-SmartPointer/bonus/handle.h \
    src/2-SmartPointer/bonus/shared.h \
    src/2-SmartPointer/malloc_usage.hpp \
    src/2-SmartPointer/shared_pointer.hpp \
    src/2-SmartPointer/smart_pointer.hpp \
    src/2-SmartPointer/weak_pointer.hpp \
    src/3-Static/static_usage.hpp \
    src/4-CPUArchitecture/cpu_cache_misses.hpp \
    src/5-Templates/template_container.hpp \
    src/5-Templates/template_crtp_usage.hpp \
    src/5-Templates/template_decltype.hpp \
    src/5-Templates/template_inheritance_usage.hpp \
    src/5-Templates/template_mixin.hpp \
    src/5-Templates/template_policies.hpp \
    src/5-Templates/template_static_assert.hpp \
    src/5-Templates/template_staticpolymorphism.hpp \
    src/6-Exceptions/exception_usage.hpp \
    src/7-CustomComparators/custom_comparator.hpp \
    src/8-Cpp11Features/async_usage.hpp
