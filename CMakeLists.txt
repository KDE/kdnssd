cmake_minimum_required(VERSION 3.5)

set(KF_VERSION "5.82.0") # handled by release scripts
project(KDNSSD VERSION ${KF_VERSION})

include(FeatureSummary)
find_package(ECM 5.81.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules) #TODO: remove cmake/modules when extra-cmake-modules is done

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(KDEGitCommitHooks)

include(ECMPoQmTools)
include(ECMAddQch)

set(EXCLUDE_DEPRECATED_BEFORE_AND_AT 0 CACHE STRING "Control the range of deprecated API excluded from the build [default=0].")

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

set(REQUIRED_QT_VERSION 5.15.0)
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Network)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


include(ECMGenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(CMakePackageConfigHelpers)

ecm_setup_version(PROJECT VARIABLE_PREFIX KDNSSD
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kdnssd_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5DNSSDConfigVersion.cmake"
    SOVERSION 5)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kdnssd_version.h DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel)

configure_file(config-kdnssd.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kdnssd.h )


find_package(Avahi)
set_package_properties(Avahi PROPERTIES DESCRIPTION "Facilities for service discovery on a local network (DNSSD)"
                       URL "https://avahi.org"
                       TYPE OPTIONAL
                       PURPOSE "Either Avahi or DNSSD is required for KDE applications to make use of multicast DNS/DNS-SD service discovery"
                      )

if(NOT AVAHI_FOUND)
    find_package(DNSSD)
    set_package_properties(DNSSD PROPERTIES DESCRIPTION "Facilities for service discovery on a local network"
                          URL "https://avahi.org"
                          TYPE OPTIONAL
                          PURPOSE "Either Avahi or DNSSD is required for KDE applications to make use of multicast DNS/DNS-SD service discovery"
                          )
endif( )

if(AVAHI_FOUND OR DNSSD_FOUND)
    find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED DBus)
endif()

ecm_install_po_files_as_qm(po)

remove_definitions(-DQT_NO_CAST_FROM_ASCII)
add_definitions(-DQT_NO_FOREACH)
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f00)
add_subdirectory(src)

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5DNSSD")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5DNSSD_QCH
        FILE KF5DNSSDQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5DNSSDQchTargets.cmake\")")
endif()

configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/KF5DNSSDConfig.cmake.in"
                              "${CMAKE_CURRENT_BINARY_DIR}/KF5DNSSDConfig.cmake"
                              INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
                              )

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/KF5DNSSDConfig.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/KF5DNSSDConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel )

install(EXPORT KF5DNSSDTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE KF5DNSSDTargets.cmake NAMESPACE KF5:: COMPONENT Devel)

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)


kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
# Remove these 3 lines when the ECM version is bumped to 5.80
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
