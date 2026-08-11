import React from 'react'
import ReactDOM from 'react-dom/client'

import App from './App.jsx'
import './styles.css'

//Punto de entrada de la aplicacion dibuja el componente principal
ReactDOM.createRoot(document.getElementById('root')).render(
  <React.StrictMode>
    <App />
  </React.StrictMode>
)