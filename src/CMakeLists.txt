ecm_create_qm_loader(kdnssd_QM_LOADER kdnssd5_qt)

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
        avahi_listener.cpp
    )
    qt5_add_dbus_interface (kdnssd_LIB_SRCS org.freedesktop.Avahi.DomainBrowser.xml avahi_domainbrowser_interface )
    qt5_add_dbus_interface (kdnssd_LIB_SRCS org.freedesktop.Avahi.ServiceBrowser.xml avahi_servicebrowser_interface )
    qt5_add_dbus_interface (kdnssd_LIB_SRCS org.freedesktop.Avahi.ServiceTypeBrowser.xml avahi_servicetypebrowser_interface )
elseif (DNSSD_FOUND)
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
ecm_generate_export_header(KF5DNSSD
    EXPORT_FILE_NAME ${KDNSSD_BINARY_DIR}/dnssd/kdnssd_export.h
    BASE_NAME KDNSSD
    # GROUP_BASE_NAME KF <- enable once all of KF modules use ecm_generate_export_header
    VERSION ${KF5_VERSION}
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS 4.0
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

# Apps must include <DNSSD/File> or <dnssd/file.h>
target_include_directories(KF5DNSSD INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KDNSSD>")

target_link_libraries(KF5DNSSD PUBLIC Qt5::Network)

if (AVAHI_FOUND OR DNSSD_FOUND)
    target_link_libraries(KF5DNSSD PRIVATE Qt5::DBus)
endif ()

if (DNSSD_FOUND)
  target_link_libraries(KF5DNSSD PRIVATE ${DNSSD_LIBRARIES})
endif ()

set(KDNSSD_BUILD_INCLUDE_DIRS ${KDNSSD_BINARY_DIR})
target_include_directories(KF5DNSSD PUBLIC "$<BUILD_INTERFACE:${KDNSSD_BUILD_INCLUDE_DIRS}>")

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

install(FILES ${KDNSSD_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDNSSD/DNSSD COMPONENT Devel)

install(TARGETS KF5DNSSD EXPORT KF5DNSSDTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############
install(FILES
  ${KDNSSD_BINARY_DIR}/dnssd/kdnssd_export.h
  ${kdnssd_HEADERS}
  DESTINATION  ${KDE_INSTALL_INCLUDEDIR_KF5}/KDNSSD/dnssd COMPONENT Devel
)

if(BUILD_QCH)
    ecm_add_qch(
        KF5DNSSD_QCH
        NAME KDNSSD
        BASE_NAME KF5DNSSD
        VERSION ${KF5_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${kdnssd_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            Qt5Network_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
            ${KDNSSD_BUILD_INCLUDE_DIRS}
        BLANK_MACROS
            KDNSSD_EXPORT
            KDNSSD_DEPRECATED
            KDNSSD_DEPRECATED_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KDNSSD LIB_NAME KF5DNSSD DEPS "network" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KDNSSD)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
