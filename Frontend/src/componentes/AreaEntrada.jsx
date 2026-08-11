//textarea donde se escriben los comandos
//con Enter se ejecuta Shift+Enter se hace un salto de linea
function AreaEntrada({ comando, setComando, onEjecutar }) {
  function manejarTecla(evento) {
    if (evento.key === 'Enter' && !evento.shiftKey) {
      evento.preventDefault()
      onEjecutar()
    }
  }

  return (
    <div className="area-entrada">
      <div className="etiqueta">Entrada de comandos</div>
      <textarea
        value={comando}
        onChange={(evento) => setComando(evento.target.value)}
        onKeyDown={manejarTecla}
        placeholder={[
          'Escribe un comando, por ejemplo:',
          '  mkdisk -size=5 -unit=M -path=/tmp/Disco1.mia',
          '  fdisk -size=2 -path=/tmp/Disco1.mia -name=Part1',
          '  mount -path=/tmp/Disco1.mia -name=Part1'
        ].join('\n')}
        spellCheck={false}
      />
    </div>
  )
}

export default AreaEntrada