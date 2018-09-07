#!/bin/bash

emcc -O3 color.cpp -o color.js -s EXPORTED_FUNCTIONS="['_malloc', '_free']"
wasm2wat color.wasm -o wasm_color.wat
wasm2c color.wasm -o wasm_color.c

echo "
void WASM_RT_ADD_PREFIX(init_globals_)(void) {
  init_globals();
}
" >> wasm_color.c


