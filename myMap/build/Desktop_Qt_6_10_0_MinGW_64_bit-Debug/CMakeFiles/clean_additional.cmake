# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Mymap_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Mymap_autogen.dir\\ParseCache.txt"
  "Mymap_autogen"
  )
endif()
