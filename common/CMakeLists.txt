target_sources(fuoten_exe
    PRIVATE
        configuration.h
        configuration.cpp
        contextconfig.h
        contextconfig.cpp
        enums.h
        globals.h
        updateintervalmodel.h
        updateintervalmodel.cpp
        useravatar.h
        useravatar.cpp
)

target_include_directories(fuoten_exe BEFORE
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(commonqml_SRCS
    qml/common/models/PrivacyPolicyModel.qml
)

add_custom_target(commonqml
    SOURCES ${commonqml_SRCS}
)

install(DIRECTORY qml/common DESTINATION ${CMAKE_INSTALL_DATADIR}/${FUOTEN_EXE_OUTPUT_NAME}/qml)
