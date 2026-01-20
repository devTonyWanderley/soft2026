# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\com130126_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\com130126_autogen.dir\\ParseCache.txt"
  "com130126_autogen"
  )
endif()
