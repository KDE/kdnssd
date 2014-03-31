set(kdnssd_LIB_SRCS
    servicebase.cpp
    servicemodel.cpp
    domainmodel.cpp
    ${kdnssd_QM_LOADER}
)

if (AVAHI_FOUND)
    include_directories( ${AVAHI_INCLUDE_DIR} )
    set(kdnssd_LIB_SRCS ${kdnssd_LIB_SRCS}
        avahi-domainbrowser.cpp
        avahi-servicebrowser.cpp
        avahi-remoteservice.cpp
        avahi-publicservice.cpp
        avahi-servicetypebrowser.cpp
        avahi_server_interface.cpp
        avahi_serviceresolver_interface.cpp
        avahi_entrygroup_interface.cpp
    )
    qt5_add_dbus_interface (kdnssd_LIB_SRCS org.freedesktop.Avahi.DomainBrowser.xml avahi_domainbrowser_interface )
    qt5_add_dbus_interface (kdnssd_LIB_SRCS org.freedesktop.Avahi.ServiceBrowser.xml avahi_servicebrowser_interface )
    qt5_add_dbus_interface (kdnssd_LIB_SRCS org.freedesktop.Avahi.ServiceTypeBrowser.xml avahi_servicetypebrowser_interface )
elseif (DNSSD_FOUND)
    kconfig_add_kcfg_files(kdnssd_LIB_SRCS settings.kcfgc)
    include_directories( ${DNSSD_INCLUDE_DIR} )
    set(kdnssd_LIB_SRCS ${kdnssd_LIB_SRCS}
        mdnsd-domainbrowser.cpp
        mdnsd-remoteservice.cpp
        mdnsd-publicservice.cpp
        mdnsd-responder.cpp
        mdnsd-servicebrowser.cpp
        mdnsd-servicetypebrowser.cpp
    )
else ()
    set(kdnssd_LIB_SRCS ${kdnssd_LIB_SRCS}
        dummy-domainbrowser.cpp
        dummy-remoteservice.cpp
        dummy-publicservice.cpp
        dummy-servicebrowser.cpp
        dummy-servicetypebrowser.cpp
    )

endif ()

add_library(KF5DNSSD SHARED ${kdnssd_LIB_SRCS})
generate_export_header(KF5DNSSD BASE_NAME KDNSSD
  EXPORT_FILE_NAME ${KDNSSD_BINARY_DIR}/dnssd/kdnssd_export.h
)

# Apps must include <DNSSD/File> or <dnssd/file.h>
target_include_directories(KF5DNSSD INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/KDNSSD>")

target_link_libraries(KF5DNSSD PUBLIC Qt5::Network)

if (AVAHI_FOUND OR DNSSD_FOUND)
    target_link_libraries(KF5DNSSD PRIVATE Qt5::DBus)
endif ()

if (DNSSD_FOUND)
  target_link_libraries(KF5DNSSD PRIVATE ${DNSSD_LIBRARIES}
                                            KF5::ConfigCore # settings.kcfgc
                       )
endif ()

target_include_directories(KF5DNSSD PUBLIC "$<BUILD_INTERFACE:${KDNSSD_BINARY_DIR}>")

set_target_properties(KF5DNSSD PROPERTIES
   VERSION "${KDNSSD_VERSION_STRING}"
   SOVERSION "${KDNSSD_SOVERSION}"
   EXPORT_NAME DNSSD
)

ecm_generate_headers(KDNSSD_CamelCase_HEADERS
  HEADER_NAMES
  DomainBrowser
  RemoteService
  ServiceTypeBrowser
  PublicService
  ServiceBase
  ServiceBrowser
  ServiceModel
  DomainModel

  PREFIX DNSSD
  REQUIRED_HEADERS kdnssd_HEADERS
)

install(FILES ${KDNSSD_CamelCase_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/KDNSSD/DNSSD COMPONENT Devel)

install(TARGETS KF5DNSSD EXPORT KF5DNSSDTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############
install(FILES
  ${KDNSSD_BINARY_DIR}/dnssd/kdnssd_export.h
  ${kdnssd_HEADERS}
  DESTINATION  ${INCLUDE_INSTALL_DIR}/KDNSSD/dnssd COMPONENT Devel
)

if (DNSSD_FOUND)
    install( FILES ${CMAKE_CURRENT_BINARY_DIR}/settings.h
             DESTINATION  ${INCLUDE_INSTALL_DIR}/KDNSSD/dnssd COMPONENT Devel)
endif ()

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KDNSSD LIB_NAME KF5DNSSD DEPS "network" FILENAME_VAR PRI_FILENAME)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
