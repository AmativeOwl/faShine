import { useState } from 'react'
import useWasm from '../hooks/useWasm'
import ItemCard from './ItemCard'
import Matches from './Matches'

export default function Closet() {
    const [fashionItems, setFashionItems] = useState([])
    const [fashionType, setFashionType] = useState('TOP')
    const [occasion, setOccasion] = useState('CASUAL')
    const [colour, setColour] = useState('#ffffff')
    const [adjective, setAdjective] = useState('BOLD')
    const [queryOccasion, setQueryOccasion] = useState('CASUAL')

    const { generateOutfits, isLoaded } = useWasm()

    function hexToHSL(hex) {
        let r = parseInt(hex.slice(1, 3), 16) / 255
        let g = parseInt(hex.slice(3, 5), 16) / 255
        let b = parseInt(hex.slice(5, 7), 16) / 255
        const max = Math.max(r, g, b), min = Math.min(r, g, b)
        let h, s, l = (max + min) / 2

        if (max == min) h = s = 0 
        else {
            const dist = max - min

            s = (l > 0.5) ? (dist / (2 - max - min)) : (dist / (max + min))

            switch (max) {
                case r: 
                    h = ((g - b) / dist + (g < b ? 6 : 0)) / 6
                    break
                case g: 
                    h = ((b - r) / dist + 2) / 6 
                    break
                case b: 
                    h = ((r - g) / dist + 4) / 6
                    break
            }
        }

        return { hue: Math.round(h * 360), saturation: s, lightness: l }
    }

    function addItem() {
        const hsl = hexToHSL(colour)
        setItems([...items, { type, occasion, colour, hsl }])
    }

    function allocateCloset(items) {
        const ITEM_SIZE = 24
        const ptr = Module._malloc(items.length * ITEM_SIZE)
        items.forEach((item, i) => {
            const offset = ptr + (i * ITEM_SIZE)
            const occasionMap = { CASUAL: 0, SMART_CASUAL: 1, CORPORATE: 2, FORMAL: 3, ATHLETIC: 4, EVENING: 5 }
            Module.HEAP32[offset >> 2] = i
            Module.HEAP32[(offset + 4) >> 2] = item.type === 'TOP' ? 0 : 1
            Module.HEAP32[(offset + 8) >> 2] = item.hsl.hue
            Module.HEAPF32[(offset + 12) >> 2] = item.hsl.saturation
            Module.HEAPF32[(offset + 16) >> 2] = item.hsl.lightness
            Module.HEAP32[(offset + 20) >> 2] = occasionMap[item.occasion] ?? 0
        })
        return ptr
    }

    return (
        <div>
            <h2>my closet</h2>

            {/* form to add a new item */}
            <label>Type</label>
            <select value={type} onChange={(e) => setType(e.target.value)}>
                <option value="TOP">Top</option>
                <option value="BOTTOM">Bottom</option>
            </select>

            <label>Occasion</label>
            <select value={occasion} onChange={(e) => setOccasion(e.target.value)}>
                <option value="CASUAL">Casual</option>
                <option value="SMART_CASUAL">Smart Casual</option>
                <option value="CORPORATE">Corporate</option>
                <option value="FORMAL">Formal</option>
                <option value="ATHLETIC">Athletic</option>
                <option value="EVENING">Evening</option>
            </select>

            <label>Colour</label>
            <input type="color" value={colour} onChange={(e) => setColour(e.target.value)} />

            <button onClick={addItem}>Add Item</button>

            {/* display one ItemCard per item in the closet */}
            <div>
                {items.map((item, index) => (
                    <ItemCard key={index} type={item.type} occasion={item.occasion} colour={item.colour} />
                ))}
            </div>

            <h2>generate outfit</h2>

            {/* query dropdowns */}
            <label>Occasion</label>
            <select value={queryOccasion} onChange={(e) => setQueryOccasion(e.target.value)}>
                <option value="CASUAL">Casual</option>
                <option value="SMART_CASUAL">Smart Casual</option>
                <option value="CORPORATE">Corporate</option>
                <option value="FORMAL">Formal</option>
                <option value="ATHLETIC">Athletic</option>
                <option value="EVENING">Evening</option>
            </select>

            <label>Vibe</label>
            <select value={adjective} onChange={(e) => setAdjective(e.target.value)}>
                <option value="BOLD">Bold</option>
                <option value="SUBTLE">Subtle</option>
                <option value="TONAL">Tonal</option>
                <option value="ECLECTIC">Eclectic</option>
                <option value="CLASSIC">Classic</option>
            </select>

            {/* pass everything to Matches */}
            <Matches
                items={items}
                allocateCloset={allocateCloset}
                generateOutfits={generateOutfits}
                isLoaded={isLoaded}
                n={3}
                adjective={adjective}
                occasion={queryOccasion}
            />
        </div>
    )
}