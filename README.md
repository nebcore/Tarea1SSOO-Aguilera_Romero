# Simulador de Carrera de Autos Tarea1SSOO-Aguilera_Romero 

## Instrucciones de Compilación

Para compilar el programa, asegúrate de tener GNU g++ instalado en tu sistema. A continuación, sigue estos pasos:

1.- Navega hasta el directorio del proyecto donde se encuentra el archivo ```Makefile``` y el archivo ```carrera.cpp```.

2.- Ejecuta el siguiente comando en la terminal para compilar el programa:

```sh
make
```
Esto generará un archivo ejecutable llamado ```carrera```.

## Instrucciones de Ejecución
Una vez compilado el programa, puedes ejecutarlo con el siguiente comando:
```sh
./carrera <distancia_total> <numero_autos>
```
- ```distancia_total```: La distancia total de la carrera en metros
- ```numero_autos```: El número de autos participantes

### Ejemplos de ejecución:
```sh
./carrera 100 3
```
El programa mostrá el progreso de cada automovil en tiempo real, finalizando con el orden de llegada.
```sh
$ ./carrera 100 3
------------------------
Auto2 avanza 6 metros (total: 6 metros)
Auto0 avanza 5 metros (total: 5 metros)
Auto2 avanza 4 metros (total: 10 metros)
Auto1 avanza 9 metros (total: 9 metros)
Auto1 avanza 9 metros (total: 9 metros)
Auto2 avanza 8 metros (total: 18 metros)
Auto2 avanza 5 metros (total: 23 metros)
Auto1 avanza 4 metros (total: 13 metros)
...
Auto1 ha terminado la carrera en el lugar 1!
Auto2 avanza 7 metros (total: 100 metros)
Auto2 ha terminado la carrera en el lugar 2!
Auto0 avanza 1 metros (total: 55 metros)
Auto0 avanza 4 metros (total: 59 metros)
Auto0 avanza 1 metros (total: 60 metros)
...
Auto0 avanza 4 metros (total: 99 metros)
Auto0 avanza 1 metros (total: 100 metros)
Auto0 ha terminado la carrera en el lugar 3!

Lugar   Auto
-----------------
1       Auto1
2       Auto2
3       Auto0
```
## Consideraciones
- Durante la ejecución, puedes utilizar herramientas como ```htop``` para visualizar el proceso y las hebras activas.
- Si deseas limpiar los archivos generados durante la compilación, puedes usar el comando ```make clean```. Eliminará los archivos y ejecutables generados durante la compilación.
