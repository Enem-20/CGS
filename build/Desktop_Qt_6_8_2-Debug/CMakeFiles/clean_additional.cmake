# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CGS_autogen"
  "CMakeFiles/CGS_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/CGS_autogen.dir/ParseCache.txt"
  )
endif()
