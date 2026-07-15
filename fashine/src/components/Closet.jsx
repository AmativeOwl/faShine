import { useState, useEffect } from 'react'
import useWasm from '../hooks/useWasm'
import ItemCard from './ItemCard'
import Matches from './Matches'
import './Matches.css'
import './Closet.css'

export default function Closet() {
    const [items, setItems] = useState([])
    const [type, setType] = useState('TOP')
    const [colour, setColour] = useState('#ffffff')
    const [queryOccasion, setQueryOccasion] = useState('CASUAL')
    const [adjective, setAdjective] = useState('BOLD')
    const [n, setN] = useState(0)
    const [results, setResults] = useState([])
    const [addWarning, setAddWarning] = useState('')
    const { generateOutfits, isLoaded } = useWasm()

    const tops = items.filter(item => item.type === 'TOP').length
    const bottoms = items.filter(item => item.type === 'BOTTOM').length
    const maxOutfits = tops * bottoms
    const canGenerate = tops > 0 && bottoms > 0
    const missingType = tops === 0 ? 'top' : bottoms === 0 ? 'bottom' : null

    useEffect(() => {
        setN(maxOutfits)
    }, [maxOutfits])

    function hexToHSL(hex) {
        let r = parseInt(hex.slice(1, 3), 16) / 255
        let g = parseInt(hex.slice(3, 5), 16) / 255
        let b = parseInt(hex.slice(5, 7), 16) / 255
        const max = Math.max(r, g, b), min = Math.min(r, g, b)
        let h, s, l = (max + min) / 2
        if (max === min) {
            h = s = 0
        } else {
            const d = max - min
            s = l > 0.5 ? d / (2 - max - min) : d / (max + min)
            switch (max) {
                case r: h = ((g - b) / d + (g < b ? 6 : 0)) / 6; break
                case g: h = ((b - r) / d + 2) / 6; break
                case b: h = ((r - g) / d + 4) / 6; break
            }
        }
        return { hue: Math.round(h * 360), saturation: s, lightness: l }
    }

    function addItem() {
        const isDuplicate = items.some(
            item => item.type === type && item.colour.toLowerCase() === colour.toLowerCase()
        )
        if (isDuplicate) {
            setAddWarning(`You already have a ${type.toLowerCase()} in that colour.`)
            return
        }
        setAddWarning('')
        const hsl = hexToHSL(colour)
        setItems([...items, { type, colour, hsl }])
    }

    function updateItem(index, changes) {
        setItems(prev => prev.map((item, i) => {
            if (i !== index) return item
            const updated = { ...item, ...changes }
            if (changes.colour) updated.hsl = hexToHSL(changes.colour)
            return updated
        }))
    }

    function deleteItem(index) {
        setItems(prev => prev.filter((_, i) => i !== index))
    }

    function handleNChange(e) {
        const val = parseInt(e.target.value)
        if (isNaN(val) || val < 1) setN(1)
        else if (val > maxOutfits && maxOutfits > 0) setN(maxOutfits)
        else setN(val)
    }

    function allocateCloset(items) {
        const ITEM_SIZE = 24
        const ptr = Module._malloc(items.length * ITEM_SIZE)
        items.forEach((item, i) => {
            const offset = ptr + i * ITEM_SIZE
            Module.HEAP32[offset >> 2] = i
            Module.HEAP32[(offset + 4) >> 2] = item.type === 'TOP' ? 0 : 1
            Module.HEAP32[(offset + 8) >> 2] = item.hsl.hue
            Module.HEAPF32[(offset + 12) >> 2] = item.hsl.saturation
            Module.HEAPF32[(offset + 16) >> 2] = item.hsl.lightness
            Module.HEAP32[(offset + 20) >> 2] = 0
        })
        return ptr
    }

    return (
        <div className="closet">
            <div className="closet-main">
                <section className="closet-section">
                    <h2>My Closet</h2>
                    <div className="section-body">
                        <div className="closet-form">
                            <label>
                                type
                                <select value={type} onChange={(e) => setType(e.target.value)}>
                                    <option value="TOP">Top</option>
                                    <option value="BOTTOM">Bottom</option>
                                </select>
                            </label>

                            <label>
                                colour
                                <input type="color" value={colour} onChange={(e) => setColour(e.target.value)} />
                            </label>

                            <button className="add-btn" onClick={addItem}>add item</button>
                        </div>

                        {addWarning && <p className="warning-text">{addWarning}</p>}

                        <div className="items-grid">
                            {items.map((item, index) => (
                                <ItemCard
                                    key={index}
                                    type={item.type}
                                    colour={item.colour}
                                    onUpdate={(changes) => updateItem(index, changes)}
                                    onDelete={() => deleteItem(index)}
                                />
                            ))}
                        </div>
                    </div>
                </section>

                <section className="closet-section">
                    <h2>Generate Outfit</h2>
                    <div className="section-body">
                        <div className="query-section">
                            <label>
                                occasion
                                <select value={queryOccasion} onChange={(e) => setQueryOccasion(e.target.value)}>
                                    <option value="CASUAL">Casual</option>
                                    <option value="SMART_CASUAL">Smart Casual</option>
                                    <option value="CORPORATE">Corporate</option>
                                    <option value="FORMAL">Formal</option>
                                    <option value="ATHLETIC">Athletic</option>
                                    <option value="EVENING">Evening</option>
                                </select>
                            </label>

                            <label>
                                vibe
                                <select value={adjective} onChange={(e) => setAdjective(e.target.value)}>
                                    <option value="BOLD">Bold</option>
                                    <option value="SUBTLE">Subtle</option>
                                    <option value="TONAL">Tonal</option>
                                    <option value="ECLECTIC">Eclectic</option>
                                    <option value="CLASSIC">Classic</option>
                                </select>
                            </label>

                            <label>
                                <span className="label-row">
                                    results
                                    {maxOutfits > 0 && <span className="max-label">(max {maxOutfits})</span>}
                                </span>
                                <input
                                    type="number"
                                    min="1"
                                    max={maxOutfits || 1}
                                    value={n}
                                    onChange={handleNChange}
                                />
                            </label>
                        </div>

                        <Matches
                            items={items}
                            allocateCloset={allocateCloset}
                            generateOutfits={generateOutfits}
                            isLoaded={isLoaded}
                            n={n}
                            adjective={adjective}
                            occasion={queryOccasion}
                            results={results}
                            setResults={setResults}
                            canGenerate={canGenerate}
                        />

                        {missingType && (
                            <p className="warning-text">You need to add a {missingType}!</p>
                        )}
                    </div>
                </section>
            </div>

            <div className="closet-sidebar">
                <h2>Results</h2>
                <div className="outfit-results">
                    {results.map((outfit, index) => (
                        <div key={index} className="outfit-card">
                            <span className="outfit-rank">{String(index + 1).padStart(2, '0')}</span>
                            <div className="outfit-score">
                                score <span>{outfit.score.toFixed(2)}</span>
                            </div>
                            <div className="outfit-items">
                                {outfit.itemIds.map((id, j) => {
                                    const item = items[id]
                                    return item ? (
                                        <div
                                            key={j}
                                            className="outfit-swatch"
                                            style={{ backgroundColor: item.colour }}
                                            title={item.type}
                                        />
                                    ) : null
                                })}
                            </div>
                        </div>
                    ))}
                </div>
            </div>
        </div>
    )
}