# SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

include(HBNSfosGenerateTranslations)

set(TS_FILES
    fuoten.ts
    fuoten_da.ts
    fuoten_de.ts
    fuoten_el.ts
    fuoten_en_GB.ts
    fuoten_en_US.ts
    fuoten_fr.ts
    fuoten_it.ts
    fuoten_nl.ts
    fuoten_sv.ts
    fuoten_zh_CN.ts
)

hbn_sfos_add_translation(QM_FILES ${TS_FILES} OPTIONS -idbased)

add_custom_target(fuotentrans ALL
                  DEPENDS ${QM_FILES}
                  COMMENT "Compiles Fuoten translation messages"
                  SOURCES ${TS_FILES}
)

install(FILES ${QM_FILES} DESTINATION ${FUOTEN_TRANSDIR})
