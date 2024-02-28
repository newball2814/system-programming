## Demo Windows

https://github.com/newball2814/system-programming/assets/35733326/01178065-5b1c-4e4d-a7ae-5918247be60f

## Prerequisites
Run C2 server
```shell
ncat.exe -lvnp 13337
```

### Build Instruction
```shell
g++ -c -o main.o main.cpp
g++ -c -o bacc.o bacc.cpp
g++ -lws2_32 -o bacc.exe main.o bacc.o
```

## Demo Linux

https://github.com/newball2814/system-programming/assets/35733326/a1c46ca2-6d37-48f2-a5ed-bd26715f0b37

## Prerequisites
Run C2 server
```shell
nc -lvnp 13337
```

### Build Instruction
```shell
g++ -o bacc baccdoor_linux.cpp
```
