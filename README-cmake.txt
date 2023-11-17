VS2022, x64

	"c:\Program Files\CMake\bin\cmake.exe" -S . -B build-vs2022-x64 --preset debug

	or

	"c:\Program Files\CMake\bin\cmake.exe" -S . -B build-vs2022-x64 \
		-DCMAKE_TOOLCHAIN_FILE=g:/opt/vcpkg/scripts/buildsystems/vcpkg.cmake
