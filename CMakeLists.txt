cmake_minimum_required(VERSION 3.14)

include(FetchContent)

# ----------------------------- Eigen library ----------------------------------
FetchContent_Declare(
    Eigen
    URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    )
set(EIGEN_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(EIGEN_BUILD_PKGCONFIG OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(Eigen)

project(euler-equation-1d CXX)

add_library(cfd
    INTERFACE
    )
target_include_directories(cfd INTERFACE include/)
target_link_libraries(cfd
    INTERFACE
        Eigen3::Eigen
    )
target_compile_features(cfd INTERFACE cxx_std_17)
target_compile_options(cfd
    INTERFACE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W3 /utf-8>
        $<$<CXX_COMPILER_ID:Intel>:$<IF:$<PLATFORM_ID:Windows>,/W3,-w3>>
        $<$<CXX_COMPILER_ID:IntelLLVM>:$<IF:$<PLATFORM_ID:Windows>,/W3,-w3>>
    )

function(add_simulator name)
    add_executable(${name} simulators/${name}.cpp)
    target_link_libraries(${name} PRIVATE cfd)
    target_compile_definitions(${name}
        PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:_USE_MATH_DEFINES NOMINMAX>
        )
    target_include_directories(${name} PRIVATE simulators/)
endfunction()

add_simulator(simulator_lax_wendroff)
add_simulator(simulator_sw_1st_order)
add_simulator(simulator_sw_tvd_minmod)
add_simulator(simulator_sw_tvd_vanleer)
add_simulator(simulator_roe_1st_order)
add_simulator(simulator_roe_tvd_minmod)
add_simulator(simulator_roe_tvd_vanleer)
add_simulator(simulator_exact)