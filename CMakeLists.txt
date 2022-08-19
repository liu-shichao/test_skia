cmake_minimum_required(VERSION 3.0.0)
project(demo VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

add_executable(demo
    main.cpp
)
set(THIRD_PARTY_DIR "/home/liushichao/Exercise/test_skia/thirdparty")
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    "${THIRD_PARTY_DIR}/include/src/shaders"
    "thirdparty/include/src/core"
    # "/home/liushichao/Exercise/test_skia/thirdparty/include/include/core"
    "thirdparty/include/include/core"
    "thirdparty/include"
    "thirdparty"
)

target_link_libraries(
    ${PROJECT_NAME}
    "${THIRD_PARTY_DIR}/lib/libsvg.a"
    "${THIRD_PARTY_DIR}/lib/libskia.a"
    # "${THIRD_PARTY_DIR}/lib/libsktext.a"
    "${THIRD_PARTY_DIR}/lib/libskshaper.a"
    # "${THIRD_PARTY_DIR}/lib/libskottie.a"
    "${THIRD_PARTY_DIR}/lib/libskunicode.a"
    # "${THIRD_PARTY_DIR}/lib/libsksg.a"
    # "${THIRD_PARTY_DIR}/lib/libskresources.a"
    # "${THIRD_PARTY_DIR}/lib/libskparagraph.a"
    # "${THIRD_PARTY_DIR}/lib/libskcms.a"
    # "${THIRD_PARTY_DIR}/lib/libpiex.a"
    # "${THIRD_PARTY_DIR}/lib/libpathkit.a"
    # "${THIRD_PARTY_DIR}/lib/libparticles.a"
    # "${THIRD_PARTY_DIR}/lib/libharfbuzz.a"
    # "${THIRD_PARTY_DIR}/lib/libdng_sdk.a"
    # "${THIRD_PARTY_DIR}/lib/libcompression_utils_portable.a"
    pthread
    png
    z
    expat
    fontconfig
    freetype
)