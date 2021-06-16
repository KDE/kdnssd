add_library(KF5DNSSD SHARED)

ecm_create_qm_loader(KF5DNSSD kdnssd5_qt)

target_sources(KF5DNSSD PRIVATE
    servicebase.cpp
    servicemodel.cpp
    domainmodel.cpp
)

if (AVAHI_FOUND)
    include_directories( ${AVAHI_INCLUDE_DIR} )
    target_sources(KF5DNSSD PRIVATE
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
    set(kdnssd_dbus_LIB_SRCS)
    qt_add_dbus_interface(kdnssd_dbus_LIB_SRCS org.freedesktop.Avahi.DomainBrowser.xml avahi_domainbrowser_interface)
    qt_add_dbus_interface(kdnssd_dbus_LIB_SRCS org.freedesktop.Avahi.ServiceBrowser.xml avahi_servicebrowser_interface)
    qt_add_dbus_interface(kdnssd_dbus_LIB_SRCS org.freedesktop.Avahi.ServiceTypeBrowser.xml avahi_servicetypebrowser_interface)
    target_sources(KF5DNSSD PRIVATE ${kdnssd_dbus_LIB_SRCS})
elseif (DNSSD_FOUND)
    include_directories( ${DNSSD_INCLUDE_DIR} )
    target_sources(KF5DNSSD PRIVATE
        mdnsd-domainbrowser.cpp
        mdnsd-remoteservice.cpp
        mdnsd-publicservice.cpp
        mdnsd-responder.cpp
        mdnsd-servicebrowser.cpp
        mdnsd-servicetypebrowser.cpp
    )
else ()
    target_sources(KF5DNSSD PRIVATE
        dummy-domainbrowser.cpp
        dummy-remoteservice.cpp
        dummy-publicservice.cpp
        dummy-servicebrowser.cpp
        dummy-servicetypebrowser.cpp
    )

endif ()

ecm_generate_export_header(KF5DNSSD
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/kdnssd_export.h
    BASE_NAME KDNSSD
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS 4.0
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

# Apps must include <KDNSSD/File> or <kdnssd/file.h>
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
   VERSION "${KDNSSD_VERSION}"
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

  PREFIX KDNSSD
  REQUIRED_HEADERS kdnssd_HEADERS
)

install(FILES ${KDNSSD_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDNSSD/KDNSSD COMPONENT Devel)

install(TARGETS KF5DNSSD EXPORT KF5DNSSDTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/kdnssd_export.h
  ${kdnssd_HEADERS}
  DESTINATION  ${KDE_INSTALL_INCLUDEDIR_KF5}/KDNSSD/kdnssd COMPONENT Devel
)

# provide compat headers for old broken include path prefix dnssd/ & DNSSD/
if(NOT EXCLUDE_DEPRECATED_BEFORE_AND_AT STREQUAL "CURRENT" AND
   EXCLUDE_DEPRECATED_BEFORE_AND_AT VERSION_LESS 5.84.0)
    function(generate_compat_headers)
        foreach(classname ${ARGV})
            string(TOLOWER ${classname} classname_lc)
            set(HEADER_NAME "${classname_lc}.h")
            # normal header
            set(compat_header "${CMAKE_CURRENT_BINARY_DIR}/compat/${HEADER_NAME}")
            set(NEW_INCLUDE "kdnssd/${HEADER_NAME}")
            configure_file(compatheader.h.in ${compat_header} @ONLY)
            install(FILES ${compat_header}
                DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDNSSD/dnssd
                COMPONENT Devel
            )
            # CamelCase header
            set(compat_header "${CMAKE_CURRENT_BINARY_DIR}/compat/${classname}")
            set(NEW_INCLUDE "KDNSSD/${classname}")
            configure_file(compatheader.h.in ${compat_header} @ONLY)
            install(FILES ${compat_header}
                DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDNSSD/DNSSD
                COMPONENT Devel
            )
        endforeach()
    endfunction()

    generate_compat_headers(
        DomainBrowser
        RemoteService
        ServiceTypeBrowser
        PublicService
        ServiceBase
        ServiceBrowser
        ServiceModel
        DomainModel
    )
endif()

if(BUILD_QCH)
    ecm_add_qch(
        KF5DNSSD_QCH
        NAME KDNSSD
        BASE_NAME KF5DNSSD
        VERSION ${KF_VERSION}
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
            "KDNSSD_DEPRECATED_VERSION(x, y, t)"
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KDNSSD LIB_NAME KF5DNSSD DEPS "network" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KDNSSD)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
