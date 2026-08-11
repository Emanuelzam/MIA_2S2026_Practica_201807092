//barra de la parte superior con las acciones principales
function BarraSuperior({ onEjecutar, onLimpiar, onResetear, onSubirArchivo, ejecutando }) {
  return (
    <div className="barra-superior">
      <span className="titulo">Analizador EXT2</span>

      <div className="acciones">
        <button className="btn btn-ejecutar" onClick={onEjecutar} disabled={ejecutando}>
          {ejecutando ? 'Ejecutando...' : '\u25B6 Ejecutar'}
        </button>
        <button className="btn" onClick={onLimpiar}>
          Limpiar
        </button>
        <button className="btn" onClick={onResetear} disabled={ejecutando}>
          Reiniciar estado
        </button>

        {/* es un label porque el input file va oculto */}
        <label className="btn">
          Subir archivo
          <input type="file" accept=".mia,.txt" onChange={onSubirArchivo} hidden />
        </label>
      </div>
    </div>
  )
}

export default BarraSuperior