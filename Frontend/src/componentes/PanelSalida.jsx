//Le da el color a cada linea segun lo que contenga
function claseLinea(linea) {
  const texto = linea.trim()
  if (texto.startsWith('#')) {
    return 'texto-comentario'
  }
  if (texto.startsWith('[OK]')) {
    return 'texto-ok'
  }
  if (texto.startsWith('[ERROR')) {
    return 'texto-error'
  }
  if (texto.startsWith('>')) {
    return 'texto-prompt'
  }
  if (linea.startsWith('$')) {
    return 'texto-comando'
  }
  return 'texto-normal'
}

//panel de la parte de abajo, dividido en salida y registro de errores
function PanelSalida({ salida, errores }) {
  return (
    <div className="panel-salida">
      <div className="subpanel">
        <div className="panel-titulo">Salida</div>
        <div className="consola">
          {salida.length === 0 ? (
            <span className="placeholder">-- sin salida todavia --</span>
          ) : (
            salida.map((linea, i) => (
              <div key={i} className={`linea ${claseLinea(linea)}`}>
                {linea}
              </div>
            ))
          )}
        </div>
      </div>

      <div className="subpanel">
        <div className="panel-titulo">Registro de errores</div>
        <div className="consola consola-error">
          {errores.length === 0 ? (
            <span className="placeholder">-- sin errores --</span>
          ) : (
            errores.map((linea, i) => (
              <div key={i} className="linea texto-error">
                {linea}
              </div>
            ))
          )}
        </div>
      </div>
    </div>
  )
}

export default PanelSalida