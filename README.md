# Busca Minas

## Objetivo
Busca Minas es un juego de estrategia donde el jugador debe evitar las minas ocultas en un tablero mientras intenta despejar todas las casillas seguras. El juego combina elementos de azar y lógica, permitiendo al jugador apostar y ganar basándose en su habilidad para navegar el campo minado.

## Características del Juego

- Tablero de 5x5 casillas
- Número variable de minas (1-24)
- Sistema de apuestas con multiplicadores dinámicos
- Puntuación basada en casillas despejadas
- Registro de puntuaciones altas

## Cómo Jugar

1. Inicia el juego ejecutando el programa.
2. Ingresa la cantidad de dinero que deseas apostar.
3. Elige la cantidad de minas que quieres en el tablero (1-24).
4. El juego mostrará un tablero vacío.
5. En cada turno:
   - Ingresa una coordenada X (1-5) y Y (A-E) para despejar una casilla.
   - Si la casilla está libre, ganarás puntos y el multiplicador aumentará.
   - Si encuentras una mina, pierdes la partida y tu apuesta.
6. El juego termina cuando:
   - Encuentras una mina (pierdes).
   - Despejas todas las casillas seguras (ganas).

## Puntuación y Ganancias

- Cada casilla despejada con éxito suma 100 puntos.
- El multiplicador de ganancias aumenta con cada casilla despejada.
- Tus ganancias se calculan multiplicando tu apuesta inicial por el multiplicador actual.

## Registro de Puntuaciones

Al finalizar el juego, podrás guardar tu puntuación. El sistema mostrará las 5 mejores puntuaciones registradas.

## Aspectos Técnicos

- Programado en C
- Utiliza matrices dinámicas para representar el tablero
- Implementa manejo de archivos para guardar y mostrar puntuaciones
- Incluye gestión de memoria para prevenir fugas

## Compilación y Ejecución

Para compilar el juego, asegúrate de tener un compilador de C instalado y ejecuta:

```
gcc -o busca_minas busca_minas.c
```

Para ejecutar el juego:

```
./busca_minas
```

¡Disfruta jugando a Busca Minas y buena suerte con tus apuestas!
