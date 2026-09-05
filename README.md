# Control HM-10

Panel de control web para vehículos Bluetooth con módulo HM-10, vía Web Bluetooth.

## Uso

1. Abre este proyecto en **Chrome/Edge/Opera para Android** (Web Bluetooth no funciona en Safari/Firefox).
2. Debe servirse por **HTTPS** — funciona directo desde GitHub Pages, no abriendo el archivo local (`file://`).
3. Toca **"Buscar HM-10"** (o **"Modo compatibilidad"** si tu módulo no aparece) para conectar.
4. Controla el vehículo con los botones, comandos de voz ("coche 1/2/0") o comandos manuales.

## Firmware

El sketch de Arduino UNO correspondiente (`control-hm10.ino`) recibe los comandos `F` / `B` / `S`
por el HM-10 vía `SoftwareSerial` y mueve dos motores reductores a través de un driver L298N.

## Estructura

- `index.html` — panel de control (todo en un solo archivo, sin dependencias de build).
- `control-hm10.ino` — firmware de Arduino.
