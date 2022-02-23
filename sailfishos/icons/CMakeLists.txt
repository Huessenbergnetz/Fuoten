include(HBNSfosGenerateIcons)

hbn_sfos_add_appicon(appIconPngs harbour-fuoten.svg)

add_custom_target(appIcon ALL
                  DEPENDS ${appIconPngs}
                  COMMENT "Generating application icon"
                  SOURCES harbour-fuoten.svg)

set(iconSvgs
    src/feed.svg
)

hbn_sfos_add_icon(iconPngs ${iconSvgs} SIZES s m l)

add_custom_target(icons ALL
                  DEPENDS ${iconPngs}
                  COMMENT "Generating theme icons"
                  SOURCES ${iconSvgs})

set(lockIconSvgs
    src/fuoten-error.svg
    src/fuoten-info.svg
)

hbn_sfos_add_icon(lockIconPngs ${lockIconSvgs} SIZES lock)

add_custom_target(lockIcons ALL
                  DEPENDS ${lockIconPngs}
                  COMMENT "Generating lock icons"
                  SOURCS ${lockIconSvgs})
