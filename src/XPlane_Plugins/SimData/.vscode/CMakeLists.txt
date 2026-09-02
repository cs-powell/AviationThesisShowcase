cmake_minimum_required(VERSION 3.14)
project(S3Uploader CXX)

set(CMAKE_CXX_STANDARD 17)

# Required for AWS SDK package discovery
find_package(AWSSDK REQUIRED COMPONENTS s3)

add_executable(s3uploader
    SimData.cpp
)

# Link AWS libraries automatically
target_link_libraries(s3uploader
    ${AWSSDK_LINK_LIBRARIES}
)