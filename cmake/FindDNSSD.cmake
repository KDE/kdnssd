# - Try to find DNSSD
# Once done this will define
#
#  DNSSD_FOUND - system has DNSSD
#  DNSSD_INCLUDE_DIR - the DNSSD include directory
#  DNSSD_LIBRARIES - Link these to use dnssd
#  DNSSD_DEFINITIONS - Compiler switches required for using DNSSD
#
# need more test: look at into dnssd/configure.in.in

# Copyright (c) 2006, Laurent Montel, <montel@kde.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of the University nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

include(CMakePushCheckState)
include(CheckFunctionExists)

FIND_PATH(DNSSD_INCLUDE_DIR dns_sd.h
  /usr/include/avahi-compat-libdns_sd/
)

if (DNSSD_INCLUDE_DIR)
  if (APPLE)
    set(DNSSD_LIBRARIES "/usr/lib/libSystem.dylib")
  else (APPLE)
    FIND_LIBRARY(DNSSD_LIBRARIES NAMES dns_sd )
  endif (APPLE)

  cmake_push_check_state()
  set(CMAKE_REQUIRED_INCLUDES ${DNSSD_INCLUDE_DIR})
  set(CMAKE_REQUIRED_LIBRARIES ${DNSSD_LIBRARIES})
  CHECK_FUNCTION_EXISTS(DNSServiceRefDeallocate DNSSD_FUNCTION_FOUND)
  cmake_pop_check_state()

  if (DNSSD_INCLUDE_DIR AND DNSSD_LIBRARIES AND DNSSD_FUNCTION_FOUND)
     set(DNSSD_FOUND TRUE)
  endif (DNSSD_INCLUDE_DIR AND DNSSD_LIBRARIES AND DNSSD_FUNCTION_FOUND)
endif (DNSSD_INCLUDE_DIR)

if (DNSSD_FOUND)
  if (NOT DNSSD_FIND_QUIETLY)
    message(STATUS "Found DNSSD: ${DNSSD_LIBRARIES}")
  endif (NOT DNSSD_FIND_QUIETLY)
else (DNSSD_FOUND)
  if (DNSSD_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find DNSSD")
  endif (DNSSD_FIND_REQUIRED)
endif (DNSSD_FOUND)

MARK_AS_ADVANCED(DNSSD_INCLUDE_DIR DNSSD_LIBRARIES)
