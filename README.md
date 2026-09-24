# SUPER MARIO PRO2

## Contenidos

[1]Ejecución
[2]Controles
[3]Arquitectura del Juego
[4]Entities & Collectables
[5]HUD
[6]Enemigos Principales
[7]Wiggler
[8]Kamek
[9]Sistema de Respawn
[10]Fin de Nivel
[11]Modo 2 Jugadores
[12]Setas Aleatorias

---

[1] Ejecución

### Desde la raíz del proyecto:

$ make

### Ejecutable en bin/mario_pro2:

./bin/mario_pro2

---

[2]🎮 Controles

Jugador 1 (Mario):

[←] - Moverse a la izquierda

[→] - Moverse a la derecha

[↑] - Sprint

[↓] - Agacharse

[Space] - Saltar

[B] - Bailar

Jugador 2 (Luigi):

[A] - Moverse a la izquierda

[D] - Moverse a la derecha

[W] - Saltar

[S] - Agacharse

[V] - Sprint

[Q] - Bailar

Funciones Especiales:

[E] - Modo Debug (muestra los coliders)

[U] - Respawn inmediato

[P] - Pausar/Continuar (Cuidado! No hay alerta visual de que el juego esta en pausa).

[2] - Hacer aparecer y desaparecer a Luigi

---

[3] Arquitectura del Juego

Este proyecto sigue un diseño por componentes:

### BoxCollider: detección de colisiones rectangulares.

### Animator: gestiona sprites y animaciones frame‑by‑frame.

Cada entidad o colectable contiene estos componentes, lo que agiliza su creación y mantenimiento.

### ParticleSystemManager: Es un componente, que facilita el pintar efectos en la pantalla (particulas como las de respawn, eliminar enemigos, humo al saltar y correr...)

---

[4] Entities & Collectables

### Entities

Desde Goombas y Koopas hasta las setas.

### Collectables

Monedas tradicionales y el EndStar (estrella dorada estilo Galaxy).

---

[5] HUD

Con ayuda de la clase HUD.hh se muestra en pantalla:

1.Monedas recogidas
2.Tiempo en partida
3.Setas recogidas

[6] Enemigos Principales

---

### Wiggler

Inspirado en listas enlazadas: cada segmento del cuerpo es un nodo.

Al saltarle, se parte el segmento tocado, reapareciendo como un nuevo Wiggler.

Movimiento sinusoidal y animación de pisada gracias al Animator.

### Kamek

Vuela por la pantalla con multiples estados: Idle, Move, Summon

Idle-> Flota en el aire.
Move-> Escoje una posicion en la pantalla, y vuela hacia alli.
Summon-> Invoca enemigos.

---

[7] Sistema de Respawn

Banderas repartidas por el nivel: al tocarlas, se actualiza tu punto de respawn.

Si Mario muere, reaparecerá en la última bandera activada.

--- 

[8] Fin de Nivel

Un collectable especial: la End Star (inspirada en Super Mario Galaxy 2).

Al recogerla, se dispara una pequeña secuencia:

Y tus marcas se muestran en pantalla.

---

[9] Modo 2 Jugadores

Pulsa 2 para invocar o expulsar a Luigi, que aparece junto a Mario.

Si sale de la pantalla, desaparece automáticamente.

---

[10] Setas Aleatorias

Al golpear un bloque “?”, sale una seta al azar de varios tipos:

Normal, Roja, Verde… ¡y la Seta Gigante!
