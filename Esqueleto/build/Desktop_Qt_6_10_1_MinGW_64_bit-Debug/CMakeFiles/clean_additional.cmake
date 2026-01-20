# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Esqueleto_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Esqueleto_autogen.dir\\ParseCache.txt"
  "Esqueleto_autogen"
  )
endif()
