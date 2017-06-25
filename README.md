# Tarea 1 HPC

## Instrucciones para correr

Se puede correr la versión secuencial y paralela con un solo comando, el script `run.sh`. Este tiene el siguiente formato:  
```sh
chmod +x ./run.sh # No olvidemos hacer que sea executable
./run.sh <imagen input> <filtro> <repeticiones>
```

Los valores por defecto son

|variable|valor|
|:-:|:-:|
|imagen|big.png|
|filtro|blur_3.txt|
|repeticiones|3|

Las imagenes se ubican en `./test/img` y son 3: lena.png, big.png y really_big.png

Los filtros se ubican en `./test/kernel` y son 2: blur_3.txt y blur_6.txt

El funcionamiento de los filtros consiste en una primera línea que define el alto, una segunda que defina el ancho y luego la matriz a usar. Esto permite tener más filtros que solo blur, a costo de tener que poner los valores a mano en el .txt.

Si se quiere correr solo la versión paralela basta comentar la lína #37 de `run.sh`
