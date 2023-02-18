
# for each "src/app_x.cpp", generate target "app_x"
file(GLOB_RECURSE all_app_srcs CONFIGURE_DEPENDS app_*.cpp)
foreach(v ${all_app_srcs})
    string(REGEX MATCH "src/.*" relative_path ${v})
    # message(${relative_path})
    string(REGEX REPLACE "src/" "" target_name ${relative_path})
    string(REGEX REPLACE ".cpp" "" target_name ${target_name})

    add_executable(${target_name} ${v})
    TARGET_LINK_LIBRARIES(${target_name} gRPC::gRPC ${MY_PROTO})
endforeach()
