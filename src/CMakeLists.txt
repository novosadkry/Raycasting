file(GLOB_RECURSE SOURCE
    *.h
    *.hpp
    *.cpp
)

target_include_directories(
    ${TARGET}
    PRIVATE
    "."
)

target_sources(
    ${TARGET}
    PRIVATE
    ${SOURCE}
)