import useWasm from './hooks/useWasm'

function App() {
    const { isLoaded, generateOutfits } = useWasm()
    console.log('isLoaded:', isLoaded)
    console.log('generateOutfits:', generateOutfits)
    return <div>faShine</div>
}

export default App