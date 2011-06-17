# Project Needs a name ofcourse
project(plasma-qmpdplasmoid)
 
# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
# We add our source code here
set(qmpdplasmoid_SRCS wheelbutton.cpp qmpdplasmoid.cpp networkaccess.cpp commondebug.cpp mpdalbum.cpp mpdartist.cpp mpdfileentry.cpp mpdtrack.cpp wlifile.cpp wlitrack.cpp ui_contextview.cpp ui_songinfo.cpp QsKineticScroller.cpp ui_contextview.ui ui_songinfo.ui)
kde4_add_ui_files(qmpdplasmoid_SRCS ui_contextview.ui ui_songinfo.ui qmpdplasmoidconfig.ui qmpdplasmoidconfigview.ui)
#kde4_add_kcfg_files(qmpdplasmoid_SRCS qmpdplasmoidsettings.kcfgc)

# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_qmpdplasmoid ${qmpdplasmoid_SRCS})
#install(FILES qmpdplasmoid.kcfg DESTINATION ${KCFG_INSTALL_DIR})

target_link_libraries(plasma_applet_qmpdplasmoid 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS} ${QT_QTNETWORK_LIBRARY} )
 
install(TARGETS plasma_applet_qmpdplasmoid
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-qmpdplasmoid.desktop 
        DESTINATION ${SERVICES_INSTALL_DIR})
