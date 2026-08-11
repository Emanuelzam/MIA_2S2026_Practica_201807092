import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// configuracion basica de vite, el frontend corre en el puerto 3000
export default defineConfig({
  plugins: [react()],
  server: {
    port: 3000
  }
})