# SPDX-FileCopyrightText: 2026 Volker Krause <vkrause@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

if(TARGET Qt6::Widgets)
    add_executable(zeroconf-browser)
    target_sources(zeroconf-browser PRIVATE browser.cpp)
    target_link_libraries(zeroconf-browser PRIVATE KF6DNSSD Qt6::Widgets)
endif()
