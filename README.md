# Simulador de voo

Esse projeto é um simulador de voo construído com OpenGL 2.1 e FreeGlut para a disciplina de INF 390 - Computação Gráfica da Universidade Federal de Viçosa (UFV)


### Load OBJ files

 ```cpp
Model model;
OBJ obj("<nome_do_arquivo>");
obj.load(model);
```

### Tech
* [gcc]
* [MSYS2]
* [CMake]
* [Ninja]

### Installation (Windows)
* Install MSYS2
* No shell do MYSYS2 MinGW 64-Bit, digite:

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
[gcc]: <https://gcc.gnu.org>
