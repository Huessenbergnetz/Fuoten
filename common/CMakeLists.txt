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

set(commonqml_SRCS
    qml/common/models/PrivacyPolicyModel.qml
)

add_custom_target(commonqml
    SOURCES ${commonqml_SRCS}
)

install(DIRECTORY common DESTINATION ${CMAKE_INSTALL_DATADIR}/${FUOTEN_EXE_OUTPUT_NAME}/qml)
