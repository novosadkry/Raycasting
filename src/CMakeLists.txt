file(GLOB_RECURSE SOURCE
    *.h
    *.hpp
    *.cpp
)

target_sources(
    ${TARGET}
    PRIVATE
    ${SOURCE}
)