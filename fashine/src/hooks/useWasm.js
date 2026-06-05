import { useState, useEffect } from 'react'

export default function useWasm() {
    const [isLoaded, setIsLoaded] = useState(false)
    const [wasm, setWasm] = useState(null)
    const [generateOutfits, setGenerateOutfits] = useState(null)

    useEffect(() => {
        Module['onRuntimeInitialized'] = () => {
            setWasm(Module)
            setIsLoaded(true)
            setGenerateOutfits(Module['_generate_outfits'])
        }
    }, [])

    return { generateOutfits, isLoaded }
}