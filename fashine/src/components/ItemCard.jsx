import { useRef } from 'react'
import './ItemCard.css'

export default function ItemCard({ type, colour, onUpdate, onDelete }) {
    const colourInputRef = useRef(null)

    return (
        <div className="item-card">
            <button className="delete-btn" onClick={onDelete} aria-label="Delete item">×</button>

            <div
                className="item-swatch"
                style={{ backgroundColor: colour }}
                onClick={() => colourInputRef.current.click()}
            />
            <input
                ref={colourInputRef}
                type="color"
                value={colour}
                onChange={(e) => onUpdate({ colour: e.target.value })}
                className="hidden-colour-input"
            />

            <button
                className="item-type"
                onClick={() => onUpdate({ type: type === 'TOP' ? 'BOTTOM' : 'TOP' })}
            >
                {type.toLowerCase()}
            </button>
        </div>
    )
}