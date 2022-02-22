include(HBNSfosGenerateIcons)

hbn_sfos_add_appicon(appIcon harbour-fuoten.svg)

add_custom_target(fuotenAppIcon ALL
                  DEPENDS ${appIcon}
                  COMMENT "Generating application icon"
                  SOURCES harbour-fuoten.svg)

set(fuotenIcons
    src/feed.svg
)

hbn_sfos_add_icon(themeIcons ${fuotenIcons} SIZES s m l)

add_custom_target(fuotenThemeIcons ALL
                  DEPENDS ${themeIcons}
                  COMMENT "Generating theme icons"
                  SOURCES ${fuotenIcons})
