# SE301-GalaxEirb
 
## Plateforme PI-4

| Implementation | FPS | Watt | 
|---------------:|-----|:-----|
| naive          | FPS | Watt | 
| neon           | FPS | Watt | 
| openmp         | FPS | Watt | 
| neon+openmp    | FPS | Watt | 
|---------------:|-----|:-----|

## INTEL Xeon


## APPLE ARM-M1 Pro

The execution plateform is an APPLE MacBook Pro 2021 platform. It is composed of:

- an Apple M1 Pro CPU that is composed of 8 processing efficient cores and 2 energy efficient ones,
- 16 GB of LPDDR5 memory,
- A 16 core integrated GPU from APPLE (for metal).

It runs macOS 13.0 (22A380) and the compiler toolchain used is llvm/15.0.3. The compilation flags applied to generate the binary executable are (see CMakeList.txt) :

```
-mcpu=native -mtune=native -std=c++17 -Ofast -ffast-math -fopenmp
```

The performance level measured using the **--view null** at galaxeirb level are provided bellow whereas power metrics were measured using the **Mx Power Gadget** tool.

| Implementation | FPS | Watt | 
|---------------:|----:|-----:|
| naive          | 4   |  3 W | 
| neon           | 55  |  4 W | 
| openmp         | 25  | 20 W | 
| neon+openmp    | 308 | 16 W | 
| metal          | 334 |  9 W | 

Note that in OpenMP experimentations, only 8 of the 10 physical cores execute computations (it seems that energy efficient ones are not activated automatically by OpenMP API).


## APPLE Macbook Pro (2019)


## Dell Computer (Thomas)


## PC de l'école


## NVIDIA Jetson Tegra 1


## NVIDIA Jetson Orin
