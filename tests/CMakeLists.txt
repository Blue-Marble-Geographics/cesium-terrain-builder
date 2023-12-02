#
# See also https://google.github.io/googletest/quickstart-cmake.html
#

cmake_policy(SET CMP0135 NEW)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# The tools are not shared libraries
add_definitions(-DCPL_DISABLE_DLL)

set(TOOL_TARGETS commander ctb  ${CMAKE_THREAD_LIBS_INIT})

add_executable(
  "ctb-tests"
  "ctb-tests.cpp"
)
if (MSVC)
	target_compile_options (
		"ctb-tests"
		PRIVATE
		/wd4244 /wd4275 /wd4251 /wd4996 /wd4267
	)
endif(MSVC)

target_link_libraries(
  "ctb-tests"
  GTest::gtest_main
  ${TOOL_TARGETS}
)

include(GoogleTest)
gtest_discover_tests("ctb-tests")
