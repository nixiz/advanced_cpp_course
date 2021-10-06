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
        src/1-Inheritance/diamond_problem.cpp \
        src/1-Inheritance/friend_usage.cpp \
        src/1-Inheritance/inheritance_usage.cpp \
        src/1-Inheritance/virtual_destructor.cpp \
        src/2-SmartPointer/bonus/shared.cpp \
        src/2-SmartPointer/bonus/bonus_old_style_smart_pointer.cpp \
        src/2-SmartPointer/malloc_usage.cpp \
        src/2-SmartPointer/shared_pointer.cpp \
        src/2-SmartPointer/smart_pointer.cpp \
        src/2-SmartPointer/weak_pointer.cpp \
        src/3-Static/static_usage.cpp \
        src/4-CPUArchitecture/cpu_cache_misses.cpp \
        src/4-CPUArchitecture/branch_prediction.cpp \
        src/4-CPUArchitecture/false_sharing.cpp \
        src/4-CPUArchitecture/packed_data_access.cpp \
        src/5-Templates/template_container.cpp \
        src/5-Templates/template_crtp_usage.cpp \
        src/5-Templates/template_decltype.cpp \
        src/5-Templates/template_inheritance_usage.cpp \
        src/5-Templates/template_mixin.cpp \
        src/5-Templates/template_policies.cpp \
        src/5-Templates/template_static_assert.cpp \
        src/5-Templates/template_staticpolymorphism.cpp \
        src/5-Templates/template_traits.cpp \
        src/6-Exceptions/exception_usage.cpp \
        src/7-CustomComparators/custom_comparator.cpp \
        src/8-Cpp11Features/async_usage.cpp
        

HEADERS += \
    inc/no_destruct.h \
    inc/number.hpp \
    inc/benchmarker.hpp \
    inc/advanced_cpp_topics.h \
    inc/playground_organizer.hpp \
    src/2-SmartPointer/bonus/handle.h \
    src/2-SmartPointer/bonus/shared.h