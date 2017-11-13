# Simulador de voo

Esse projeto é um simulador de voo construído com OpenGL 2.1 e FreeGlut para a disciplina de INF 390 - Computação Gráfica da Universidade Federal de Viçosa (UFV)


### Para carregar arquivos OBJ

 ```cpp
Model model;
OBJ obj("<nome_do_arquivo>");
obj.load(model);
```

### Tech

Utilizamos o MSYS2 MinGW 64bits com o CMake e o Ninja para compilar.

* [MSYS2]
* [CMake]
* [Ninja]

### Installation (Windows)
* Instale o MSYS2
* No MYSYS2 MinGW 64-Bit, atualize os pacotes e instale o CMake, o Ninja e o gcc/g++

    ```sh
    pacman -Syu
    pacman -Su
    pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-gcc
    ```

### Compilation
```sh
cd <project_directory>
mkdir build
cd build
cmake .. -G Ninja
ninja
```

### Exec

```sh
cd <project_directory>
./script.sh
```

[MSYS2]: <http://www.msys2.org>
[Ninja]: <https://ninja-build.org>
[CMake]: <https://cmake.org>
