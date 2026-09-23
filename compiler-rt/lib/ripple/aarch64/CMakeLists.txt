set(COMPILER_RT_DEFAULT_TARGET_TRIPLE
    "${COMPILER_RT_DEFAULT_TARGET_TRIPLE}"
    CACHE STRING "Target triple (e.g., aarch64-linux-gnu)")

# ------------------------------------------------------------------------------
# Determine target architecture.
# Extract the first token (arch) from the triple and normalize aliases,
# e.g. "arm64-apple-darwin" -> "aarch64".
# ------------------------------------------------------------------------------
set(_derived_arch "")
if(COMPILER_RT_DEFAULT_TARGET_TRIPLE)
  # Split triple by '-': "<arch>-<vendor>-<os>-<abi>"
  string(REPLACE "-" ";" _triple_parts "${COMPILER_RT_DEFAULT_TARGET_TRIPLE}")
  list(GET _triple_parts 0 _arch_raw)
  string(TOLOWER "${_arch_raw}" _arch)

  # Normalize common aliases to canonical names.
  if(_arch STREQUAL "arm64" OR _arch STREQUAL "armv8")
    set(_arch "aarch64")
  endif()

  set(_derived_arch "${_arch}")
endif()

# Check if the target is supported.
if(NOT _derived_arch)
  message(WARNING
    "Ripple aarch64 vector library disabled: no target triple provided. "
    "Pass -DCOMPILER_RT_DEFAULT_TARGET_TRIPLE=aarch64-linux-gnu to enable.")
  return()
elseif(NOT "${_derived_arch}" IN_LIST RIPPLE_SUPPORTED_ARCH)
  message(WARNING
    "Ripple aarch64 vector library disabled. "
    "Target ${_derived_arch} not supported.")
  return()
else()
  message(STATUS "Ripple vector library enabled for ${_derived_arch}")
endif()

set(RIPPLE_RT_SRC_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(RIPPLE_RT_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/header)
set(SME_BC_SRC ${RIPPLE_RT_SRC_DIR}/sme_veclib.c)
set(SME_HEADER ${RIPPLE_RT_HEADER_DIR}/ripple_sme_veclib.h)

if(NOT EXISTS ${SME_BC_SRC})
  message(WARNING "Cannot find source file ${SME_BC_SRC}, skipping Ripple aarch64 vector library.")
  return()
endif()

# ------------------------------------------------------------------------------
# 1. Determine build-tree locations.
# Query the compiler for its resource directory: the Ripple driver support in
# Clang.cpp looks for bitcode libraries under "<resource-dir>/lib/ripple" and
# clang's builtin header search always includes "<resource-dir>/include", so
# artifacts must land there (not compiler-rt's generic per-arch lib dir) for
# the just-built (uninstalled) Clang to find them, e.g. during tests.
# ------------------------------------------------------------------------------
execute_process(
  COMMAND ${CMAKE_C_COMPILER} -print-resource-dir
  OUTPUT_VARIABLE ACTUAL_RESOURCE_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(NOT ACTUAL_RESOURCE_DIR)
  set(ACTUAL_RESOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
endif()

set(BC_BUILD_DIR "${ACTUAL_RESOURCE_DIR}/lib/ripple")
set(BC_FILE "${BC_BUILD_DIR}/sme_veclib.bc")
set(HEADER_BUILD_DIR "${ACTUAL_RESOURCE_DIR}/include/ripple")
set(HEADER_FILE "${HEADER_BUILD_DIR}/ripple_sme_veclib.h")

# Compile bitcode directly into the build-tree resource directory.
add_custom_command(
  OUTPUT ${BC_FILE}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${BC_BUILD_DIR}
  COMMAND ${CMAKE_C_COMPILER}
          --target=aarch64-linux-gnu
          -march=armv9.2-a+sme
          -O2
          -msve-streaming-vector-bits=512
          -emit-llvm
          -c ${SME_BC_SRC}
          -o ${BC_FILE}
  DEPENDS ${SME_BC_SRC}
  COMMENT "Compiling sme_veclib.c to LLVM bitcode for Ripple SME"
  VERBATIM
)

# Copy the header directly into the build-tree resource directory so that
# `#include <ripple/ripple_sme_veclib.h>` resolves against the just-built,
# non-installed Clang.
add_custom_command(
  OUTPUT ${HEADER_FILE}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${HEADER_BUILD_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SME_HEADER} ${HEADER_FILE}
  DEPENDS ${SME_HEADER}
  COMMENT "Copying Ripple SME header to build-tree"
  VERBATIM
)


add_custom_target(RippleLibAArch64 ALL DEPENDS ${BC_FILE} ${HEADER_FILE})

# Wire build-tree artifacts into 'compiler-rt' so that `ninja compiler-rt`
# (or equivalent) actually builds this library.
if(TARGET compiler-rt)
  add_dependencies(compiler-rt RippleLibAArch64)
endif()

# ------------------------------------------------------------------------------
# 2. Installation rules.
# Handle empty COMPILER_RT_INSTALL_PATH to ensure destinations remain
# prefix-relative: "<prefix>/lib/ripple", mirroring the resource-dir layout
# used above so installed Ripple libraries are found the same way.
# ------------------------------------------------------------------------------
if(COMPILER_RT_INSTALL_PATH STREQUAL "")
  set(INSTALL_BC_DIR "lib/ripple")
else()
  set(INSTALL_BC_DIR "${COMPILER_RT_INSTALL_PATH}/lib/ripple")
endif()
set(INSTALL_INC_DIR "${COMPILER_RT_INSTALL_INCLUDE_DIR}/ripple")

install(FILES ${BC_FILE}
        DESTINATION ${INSTALL_BC_DIR}
        COMPONENT ripple
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

# Headers once (not per-arch); glob so future headers are picked up
# automatically, matching the Hexagon Ripple library's convention.
file(GLOB RIPPLE_RT_HEADERS ${RIPPLE_RT_HEADER_DIR}/*.h)
install(FILES ${RIPPLE_RT_HEADERS}
        DESTINATION ${INSTALL_INC_DIR}
        COMPONENT ripple
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

message(STATUS "Will emit ripple library for ${_derived_arch} to: ${BC_BUILD_DIR}")

# Wire into 'install-compiler-rt'
if(TARGET install-compiler-rt)
  add_custom_target(install-ripple-aarch64
                    DEPENDS RippleLibAArch64
                    COMMAND "${CMAKE_COMMAND}"
                            -DCMAKE_INSTALL_COMPONENT=ripple
                            -P "${CMAKE_BINARY_DIR}/cmake_install.cmake")
  add_dependencies(install-compiler-rt install-ripple-aarch64)
endif()