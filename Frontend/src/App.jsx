import { useState } from 'react'

import BarraSuperior from './componentes/BarraSuperior'
import AreaEntrada from './componentes/AreaEntrada'
import PanelSalida from './componentes/PanelSalida'

//Direccion donde corre el backend el servidor de la api
const API = 'http://localhost:8080'

//Manda un solo comando al backend y devuelve la respuesta
async function enviarComando(comando) {
  const respuesta = await fetch(`${API}/comando`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ comando: comando })
  })
  return await respuesta.json()
}

//limpia el estado del backend para empezar desde cero
async function reiniciarEstado() {
  await fetch(`${API}/reset`, { method: 'POST' })
}

function App() {
  const [comando, setComando] = useState('')
  const [salida, setSalida] = useState([])
  const [errores, setErrores] = useState([])
  const [ejecutando, setEjecutando] = useState(false)

  //separa cada linea y la manda al backend una por una
  //las lineas con [ERROR] van al registro de errores
  async function ejecutar() {
    if (ejecutando) {
      return
    }
    const lineas = comando
      .split('\n')
      .map((l) => l.trim())
      .filter((l) => l.length > 0)

    if (lineas.length === 0) {
      return
    }

    setEjecutando(true)

    const nuevasSalida = []
    const nuevosErrores = []

    for (const linea of lineas) {
      // los comentarios solo se muestran en la salida, no se mandan
      // al backend porque no son comandos
      if (linea.startsWith('#')) {
        nuevasSalida.push(linea)
        continue
      }

      //comando como promt de linux
      nuevasSalida.push(`$ ${linea}`)

      try {
        const datos = await enviarComando(linea)
        const textoSalida = datos.salida || '--- sin respuesta ---'

        //guarda la salida, los errores tambien se muestran
        //en la salida para que el usuario los vea
        for (const l of textoSalida.split('\n')) {
          if (l.trim().length === 0) {
            continue
          }
          nuevasSalida.push(l)
          if (l.includes('[ERROR]')) {
            nuevosErrores.push(l)
          }
        }
      } catch (err) {
        // si el backend no esta corriendo el fetch falla
        const aviso = '[ERROR] no se pudo conectar con el backend (revisa que ./servidor este arriba)'
        nuevasSalida.push(aviso)
        nuevosErrores.push(aviso)
      }
    }

    setSalida((prev) => [...prev, ...nuevasSalida])
    setErrores((prev) => [...prev, ...nuevosErrores])
    setEjecutando(false)
  }

  function limpiar() {
    setComando('')
    setSalida([])
    setErrores([])
  }

  //reinicia el backend y limpia los paneles
  function resetear() {
    reiniciarEstado()
    setSalida([])
    setErrores([])
  }

  //carga un archivo de comandos en el textarea
  function subirArchivo(evento) {
    const archivo = evento.target.files[0]
    if (!archivo) {
      return
    }
    const lector = new FileReader()
    lector.onload = () => {
      setComando(String(lector.result || ''))
      evento.target.value = ''
    }
    lector.readAsText(archivo)
  }

  return (
    <div className="app">
      <BarraSuperior
        onEjecutar={ejecutar}
        onLimpiar={limpiar}
        onResetear={resetear}
        onSubirArchivo={subirArchivo}
        ejecutando={ejecutando}
      />
      <AreaEntrada comando={comando} setComando={setComando} onEjecutar={ejecutar} />
      <PanelSalida salida={salida} errores={errores} />
    </div>
  )
}

export default App