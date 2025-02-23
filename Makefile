BUILD_DIR=build/standard

build_milleniumdb_standard:
	cmake -B$(BUILD_DIR) && cmake --build $(BUILD_DIR)

build_milleniumdb_release:
	cmake -Bbuild/Release -DCMAKE_BUILD_TYPE=Release && cmake --build build/Release/
