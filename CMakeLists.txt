cmake_minimum_required(VERSION 2.8.12)

project(KDNSSD)

find_package(ECM 0.0.12 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_SOURCE_DIR}/cmake/modules) #TODO: remove cmake/modules when extra-cmake-modules is done

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings)
include(KDECMakeSettings)

include(ECMCreateQmFromPoFiles)

set(KF5_VERSION "4.98.0")

set(REQUIRED_QT_VERSION "5.2.0")
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Network)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

remove_definitions(-DQT_NO_CAST_FROM_BYTEARRAY)

include(FeatureSummary)
include(GenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMPackageConfigHelpers)

ecm_setup_version(${KF5_VERSION} VARIABLE_PREFIX KDNSSD
         SOVERSION 5               VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kdnssd_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5DNSSDConfigVersion.cmake"
                        SOVERSION 5)

configure_file(config-kdnssd.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kdnssd.h )

remove_definitions(-DQT_NO_CAST_TO_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)

find_package(Avahi)
set_package_properties(Avahi PROPERTIES DESCRIPTION "Facilities for service discovery on a local network (DNSSD)"
                       URL "http://avahi.org"
                       TYPE OPTIONAL
                       PURPOSE "Either Avahi or DNSSD is required for KDE applications to make use of multicast DNS/DNS-SD service discovery"
                      )

if(NOT AVAHI_FOUND)
    find_package(DNSSD)
    set_package_properties(DNSSD PROPERTIES DESCRIPTION "Facilities for service discovery on a local network"
                          URL "http://avahi.org"
                          TYPE OPTIONAL
                          PURPOSE "Either Avahi or DNSSD is required for KDE applications to make use of multicast DNS/DNS-SD service discovery"
                          )
endif(
                        )

if(AVAHI_FOUND OR DNSSD_FOUND)
    find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED DBus)
endif()

file(GLOB PO_FILES "po/*.po")
if (PO_FILES)
  ecm_create_qm_from_po_files(PO_FILES ${PO_FILES} CATALOG_NAME kdnssd5_qt)
endif()

add_subdirectory(src)

set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5DNSSD")
ecm_configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/KF5DNSSDConfig.cmake.in"
                              "${CMAKE_CURRENT_BINARY_DIR}/KF5DNSSDConfig.cmake"
                              INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
                              )

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/KF5DNSSDConfig.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/KF5DNSSDConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel )

install(EXPORT KF5DNSSDTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE KF5DNSSDTargets.cmake NAMESPACE KF5:: COMPONENT Devel)

if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
endif()

