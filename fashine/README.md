# faShine

A fashion outfit generator powered by a custom scoring algorithm written in C, compiled to WebAssembly via Emscripten, and created as a React hook.

## What it does

Given a closet of clothing items, faShine scores all possible top and bottom combinations using colour theory and occasion compatibility, returning the top matches ranked by a weighted score.

The user queries the algorithm with an occasion (e.g. casual, formal) and a colour adjective (e.g. bold, subtle), and the algorithm returns the best outfit combinations for that context.

## The algorithm

Scoring is pairwise, where every top is scored against every bottom, and the results are sorted to return the top n matches.

Each pair is scored across two axes:

**Colour score** is based on HSL colour theory. The adjective determines which theory applies:
- Bold = complementary (hues ~180° apart)
- Subtle = analogous (hues ~30° apart)
- Eclectic = triadic (hues ~120° apart)
- Tonal = monochromatic (same hue, varying lightness)
- Classic = neutral (low average saturation)

**Occasion score** is based on compatibility between item occasions. Same occasion scores 1.0, compatible pairs (e.g. casual + athletic, formal + evening) score 0.5, incompatible scores 0.0. Compatible pairs are stored in a chained hashtable keyed by a composite of the two occasion enum values.

**Final score** is weighted average of colour and occasion scores. Equal weighting (0.5/0.5) for casual queries, heavier occasion weighting (0.7/0.3) for formal contexts.

## Tech stack
Algorithm:  C
Build:      gcc (local testing) + Emscripten (WASM)
Frontend:   React + Vite
Hook:       useWasm.js — loads the WASM module and exposes generate_outfits

## Build

### Local test
```bash
cd src/algorithm
mingw32-make
./fashion_test
```

### Compile to WebAssembly
```bash
cd src/algorithm
mingw32-make wasm
```

Outputs `fashion.js` and `fashion.wasm` to `public/`.