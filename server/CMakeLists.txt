﻿include_directories(../3rdpart)
file(GLOB jsoncpp_src_list ../3rdpart/jsoncpp/*.cpp ../3rdpart/jsoncpp/*.h )
add_library(jsoncpp STATIC ${jsoncpp_src_list})


if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(MediaServer_src_list ./WebApi.cpp ./WebHook.cpp main.cpp)
else()
	file(GLOB MediaServer_src_list ./*.cpp ./*.h)
endif()

message(STATUS ${MediaServer_src_list})

add_executable(MediaServer ${MediaServer_src_list})

if(WIN32)
	set_target_properties(MediaServer PROPERTIES COMPILE_FLAGS  ${VS_FALGS} )
endif()

target_link_libraries(MediaServer jsoncpp ${LINK_LIB_LIST})


