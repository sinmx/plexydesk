set(icon_dpi "hdpi")

file(GLOB action_icons actions/*.png)
file(GLOB toolbar_icons toolbar/*.png)
file(GLOB navigation_icons navigation/*.png)
file(GLOB panel_icons panel/*.png)
file(GLOB desktop_icons desktop/*.png)

install(FILES ${action_icons} DESTINATION ${RESOURCES_DEST_DIR}/default/${icon_dpi}/actions/)
install(FILES ${toolbar_icons} DESTINATION ${RESOURCES_DEST_DIR}/default/${icon_dpi}/toolbar/)
install(FILES ${navigation_icons} DESTINATION ${RESOURCES_DEST_DIR}/default/${icon_dpi}/navigation/)
install(FILES ${panel_icons} DESTINATION ${RESOURCES_DEST_DIR}/default/${icon_dpi}/panel/)
install(FILES ${desktop_icons} DESTINATION ${RESOURCES_DEST_DIR}/default/${icon_dpi}/desktop/)
