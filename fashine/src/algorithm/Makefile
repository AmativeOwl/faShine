fashion: fashion.c test.c
	gcc fashion.c test.c -o fashion_test

wasm: fashion.c
	emcc fashion.c -o fashion.js \
		-s EXPORTED_FUNCTIONS='["_generate_outfits"]' \
		-s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]'