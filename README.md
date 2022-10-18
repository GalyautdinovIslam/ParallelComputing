# ParallelComputing

***

Репозиторий для домашних работ по Параллельному Программированию

### Используются:
- **С++ 14**
- **MinGW**
- **CLion**
- **Windows 10**
- **OpenMP**
- **MPI**

### Запуск OpenMP
- Добавить в **CMakeLists.txt**:
```
set(OPENMP_COMPILE_FLAG "-fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPENMP_COMPILE_FLAG}")
```

- Для каждого **cpp**-файла с методом `main` добавить в **CMakeLists.txt**:
```add_executable(_NAME _FILE)```

- **Build** и **Run**

### Запуск MPI
- Добавить в **CMakeLists.txt**:
```
find_package(MPI REQUIRED)
```

- Для каждого **cpp**-файла с методом `main` добавить в **CMakeLists.txt**:
```
add_executable(_NAME _FILE)
target_link_libraries(_NAME PUBLIC MPI::MPI_CXX)
```

- **Build**
- Запуск через **git bash terminal** (здесь:
  - **_N** — количество процессов;
  - **_PATH** — путь до скомпилированного **exe**-файла):

```
mpiexec -n _N _PATH
```
