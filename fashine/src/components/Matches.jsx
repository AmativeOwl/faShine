export default function Matches({ items, allocateCloset, generateOutfits, isLoaded, n, adjective, occasion, results, setResults, canGenerate }) {
    const adjMap = { BOLD: 0, SUBTLE: 1, TONAL: 2, ECLECTIC: 3, CLASSIC: 4 }
    const occasionMap = { CASUAL: 0, SMART_CASUAL: 1, CORPORATE: 2, FORMAL: 3, ATHLETIC: 4, EVENING: 5 }

    function handleGenerate() {
        if (!isLoaded || items.length === 0 || !canGenerate) return

        const ptr = allocateCloset(items)
        const resultPtr = generateOutfits(ptr, items.length, n, adjMap[adjective], occasionMap[occasion])

        const OUTFIT_SIZE = 24
        const outfits = []

        for (let i = 0; i < n; i++) {
            const offset = resultPtr + i * OUTFIT_SIZE
            const score = Module.HEAPF64[offset >> 3]
            const itemIdsPtr = Module.HEAPU32[(offset + 8) >> 2]
            const itemIdsSize = Module.HEAP32[(offset + 12) >> 2]

            const itemIds = []
            for (let j = 0; j < itemIdsSize; j++) {
                itemIds.push(Module.HEAP32[(itemIdsPtr + j * 4) >> 2])
            }

            outfits.push({ score, itemIds })
        }

        Module._free(ptr)
        const validOutfits = outfits.filter(o => o.score > 0 && isFinite(o.score))
        setResults(validOutfits)
    }

    return (
        <button
            className="generate-btn"
            onClick={handleGenerate}
            disabled={!isLoaded || items.length === 0 || !canGenerate}
        >
            {isLoaded ? 'generate outfits' : 'loading...'}
        </button>
    )
}