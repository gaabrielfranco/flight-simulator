# OBJ Loader

Veja <Model/Model.h> ...

Para carregar arquivos OBJ:

```cpp
OBJ obj("<nome_do_arquivo");
obj.load(model);
```

Instalar msys2 windows 64 bits

no msys2:
    pacman -Syu
    pacman -Su
    pacman -S

abrir msys mingw64:
    pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
    pacman -S mingw-w64-x86_64-gcc
    cd /c/Users/(diretório OBJ)
    mkdir build
    cd build
    cmake .. -G Ninja
    ninja

para compilar/executar:
    no diretório OBJ:
        ./script.sh

