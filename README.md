# Tarea 2 HPC

## Instrucciones para correr

Se puede correr la versión secuencial y paralela con un solo comando, el script `run.sh`  
También se puede correr una versión en particular  
El `<problema>` por defecto (si no se incluye) es `./test/t1.txt`
```bash
chmod +x ./run.sh # No olvidemos hacer que sea executable
./run.sh <problema.txt> # Corre paralelo y secuencial
./run.sh -s <problema.txt> # Corre secuencial
./run.sh -m <problema.txt> # Corre paralelo
```

Importante notar que si se quiere correr fuera de tripio modificar la línea 24 para que no ocupe el `hostfile`
```bash
is_local_nico() { [[ $(pwd) == *"Nico"* ]] || [[ $(pwd) == *'Parallels'* ]]; }
# Reemplazar Nico por algun string solo presente en el pwd local
```

Los problemas están ubicados en `./test`  
Estos fueron echos a mano o generados con el script `generate.py`  
Si se quiere crear un nuevo problema basta con hacer
```bash
python3 generate.py <nombre> <tamaño>
# Archivo se guardará como ./test/<nombre>.txt
```
