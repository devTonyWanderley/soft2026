# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\WSerial_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\WSerial_autogen.dir\\ParseCache.txt"
  "WSerial_autogen"
  )
endif()
