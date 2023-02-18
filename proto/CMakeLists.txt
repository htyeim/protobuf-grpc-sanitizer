
# to find *.bp.h files
# include_directories(${CMAKE_CURRENT_BINARY_DIR})
# message("CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})

set(PROTO_FILES
    myproto/address.proto 
    myproto/addressbook.proto
)
add_library(${MY_PROTO} ${PROTO_FILES})
target_link_libraries(${MY_PROTO}
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)

target_include_directories(${MY_PROTO} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate(TARGET ${MY_PROTO} LANGUAGE cpp)

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET myproto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")


protobuf_generate_python(PY_SOURCES ${PROTO_FILES})
set(MY_PROTO_PY myprotopy)
add_custom_target(${MY_PROTO_PY} ALL 
                  DEPENDS ${PY_SOURCES})
