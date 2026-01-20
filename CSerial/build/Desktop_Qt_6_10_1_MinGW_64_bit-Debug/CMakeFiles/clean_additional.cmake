# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\CSerial_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CSerial_autogen.dir\\ParseCache.txt"
  "CSerial_autogen"
  )
endif()
