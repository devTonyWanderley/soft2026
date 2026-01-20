# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\revisao_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\revisao_autogen.dir\\ParseCache.txt"
  "revisao_autogen"
  )
endif()
