.PHONY: setup tests compile assets shaders textures clean

setup: assets
	@meson setup build --reconfigure

tests:
	@meson test -C build

compile:
	@meson compile -C build

assets: shaders textures

shaders:
	@bash scripts/setup.sh assets shaders

textures:
	@bash scripts/setup.sh assets textures

clean:
	@rm -rf build/*

	@rm -rf assets/shaders/build/*
	@rm -rf assets/textures/build/*

	@rm -rf scripts/logs/*
