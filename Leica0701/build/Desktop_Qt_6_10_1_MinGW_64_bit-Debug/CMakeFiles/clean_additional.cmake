# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Leica0701_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Leica0701_autogen.dir\\ParseCache.txt"
  "Leica0701_autogen"
  )
endif()
