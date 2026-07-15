fashion: fashion.c test.c
	gcc fashion.c test.c -o fashion_test

wasm: fashion.c
	emcc fashion.c -o fashion.js \
		-s EXPORTED_FUNCTIONS='["_generate_outfits","_malloc","_free"]' \
		-s EXPORTED_RUNTIME_METHODS='["ccall","cwrap","HEAP32","HEAPF32","HEAPF64","HEAPU32"]'