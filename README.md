[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/1kKW-HvG)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-718a45dd9cf7e7f842a935f5ebbe5719a5e09af4491e668f4dbf3b35d5cca122.svg)](https://classroom.github.com/online_ide?assignment_repo_id=12660440&assignment_repo_type=AssignmentRepo)
# PathFinding

## Integrantes
> Lucas Carranza Bueno
> Adrian Céspedes Zevallos
> David Herencia Galván

## Enunciado
Implementar **eficientemente** los siguientes algoritmos de búsqueda en grafos y aplicarlo al problema de PathFinding de manera visual.
- Dijkstra
- A*
- Greedy BSF (Opcional, +0.5 examen final)

## Instalación y ejecución
Clonar el repositorio:
```bash
git clone https://github.com/utec-cs-aed-2023-2/pathfinding-milordturing.git
cd pathfinding-milordturing
```

Ejecutar
```bash
# Linux
./linux_run.sh

# Windows
.\windows_run.bat
```

## Guía de uso

- Presionar SPACE para iniciar la simulación.
- Presionar H para ayuda.
- Click izquierdo para colocar paredes, Click derecho para eliminar paredes.
- Doble Click izquierdo para colocar el inicio.
- Doble Click derecho para colocar el fin.

- Presionar R para reiniciar la cuadrícula (elimina todas las paredes).
- Presionar C para limpiar la traza del algoritmo.

- Presionar los números 1-2-3 para seleccionar el algoritmo.
- Presionar las teclas E y M para cambiar la heurística.

> **Nota:**
> - El código debe ser original, si se evidencia copia completa, copia parcial o copia estructural, la nota del ejercicio será de 0 y se penaliza con -10 puntos en evaluación contínua.
> - Deben usar sus propios contenedores. 


**Representación del Grafo**:
- Cada celda representa un vertice del grafo
- Los vertices adyacentes son los vecinos en vertical (2), horizontal (2) y diagonal (4). Como máximo 8 vertices adyacentes.
- Considerar como costo de movimiento:
  * Movimiento en horizontal y vertical: 10
  * Movimiento en diagonal: 14
- Implementar dos heurísticas: distancia manhattan y distancia euclidiana

![image](https://github.com/utec-cs-aed/PathFinding/assets/48141762/48c513ba-4a34-4f42-874e-a32a1e50cc72)


**Visualización**:
- Visualizar la ejecución del algoritmo de manera interactiva en consola. 
- Pueden usar interfaz gráfica de usuario (GUI) con cualquier libreria gráfica (Opcional, +1 examen final):
    * [SFML](https://www.sfml-dev.org/)
    * [QT Open Source](https://www.qt.io/download-open-source#source)
    * [WxWidgets](https://www.wxwidgets.org/)
    * [Visual Studio C++](https://visualstudio.microsoft.com/es/vs/features/cplusplus/)
    * [C++ Builder](https://www.embarcadero.com/es/products/cbuilder)

![image](https://github.com/utec-cs-aed/PathFinding/assets/48141762/0836d9ad-ca21-4382-b515-9feb4e467023)



