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
}