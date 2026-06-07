import { useState, useEffect } from 'react'

export default function useWasm() {
    const [isLoaded, setIsLoaded] = useState(false)
    const [wasm, setWasm] = useState(null)
    const [generateOutfits, setGenerateOutfits] = useState(null)

    useEffect(() => {
        if (Module.calledRun) {
            const wrapped = Module.cwrap('generate_outfits', 'number', ['number', 'number', 'number', 'number', 'number'])
            setGenerateOutfits(() => wrapped)
            setWasm(Module)
            setIsLoaded(true)
        } else {
            Module['onRuntimeInitialized'] = () => {
                const wrapped = Module.cwrap('generate_outfits', 'number', ['number', 'number', 'number', 'number', 'number'])
                setGenerateOutfits(() => wrapped)
                setWasm(Module)
                setIsLoaded(true)
            }
        }
    }, [])

    return { generateOutfits, isLoaded }
}