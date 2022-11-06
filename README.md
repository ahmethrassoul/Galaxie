# SE301-GalaxEirb
 
 Le projet GalaxEirb est né il y a quelques années sous l'impulsion de [J. CRENNE](https://www.linkedin.com/in/jeremiecrenne) en 2012. Ce projet dont le nom **GalaxEirb** est la contraction de **GALAXy simulator** and **ensEIRB** avait pour vocation à servir de support à l'enseignement de la programmation parallèle en dernière année du département électronique en option système embarqué.

L'objectif du projet est d'implenter un simulteur of celestial objects qui est un probleme de type n-body problem. L'espect mathématique de la chose est dérit dans la présentation réalisée par J. Crenne qui est disponible ici:

REF J. Crenne

## Project objectives

L'objectif de ce projet est pour vous :

- D'implenter une version fonctionnelle du simulateur de galaxies dans les codes source mis à votre disposition.

- D'accélérer le processus de simulation à l'aide des compétences que vous avec acquise lors des séances précédentes (cf. HTTP).

Pour accélérer le processus de simulation vous avez le droit d'utiliser toutes les optimisations que vous connaissez et de les associer. Vous pouvez même mettre en oeuvre des approches non étudiés dans le module ex. OpenCL ou METAL (pour ceux qui ont des machines compatibles).

## Provided resources

Afin de vous simplifier la vie, votre enseignant a développé un framework permettant de charger en mémoire les particules présentes dans le fichier (dubinski.tab) et de les afficher à l'écran dans une GUI SDL2/OpenGL. Les codes sources actuels sont pour une partie d'entre eux des versions amendées de ceux développés par J. Crenne à l'origine.

Le repertoire contenant les codes sources est décomposé en 5 parties:

1 - **Backend** : Ce sont les fichiers nécessaires à la création des interfaces graphiques de visualisation (GUI). *Normalement vous n'avez pas besoin de les regarder ni de les modifier*.

2 - **datatype** : Ce sont les fichiers qui décrivent le processus de chargement des données en mémoire. Ces fonctions sont utilisée afin de créer un élément de type **Galaxy** que vous manipulerez. *Normalement vous n'avez pas besoin de les regarder ni de les modifier*.

3 - **Galaxy** : La classe **Galaxy** décrite dans les fichiers sources présent dans ce repertoire fournit décrit la structure de mémorisation des particules. C'est sur cette structuration des données en mémoire que vous devrez travailler.

4 - **kernel** : Ce repertoire contient un ensemble de sous repertoires qui vous permettront de décrire et d'implanter différentes optimisation sans trop vous fatiguer... L'activation et la descativation des fichiers est autimatique à la compilation en fonction du système.

5 - **main.cpp** : C'est le programme principal qui permet de mettre en musique l'ensemble des classes développées.


## Compiling the cloned version on ENSEIRB computers

This step should be quite simple. Like in previous stages (XXX), a CMakeLists file is provided. So you should simply write in a terminal the following commands:

```
cd SE301-GalaxEirb-Student
mkdir build
cd build
cmake ..
make
```

Normallly, if everything goes right the compilation process succeds. Then, you can execute the following command to launch the program in its original state:

```
./galax_eirb --impl null --view null
```

In the terminal, a set of lines such as these ones should appear:

```
(II) GalaxEirb student project (ARM NEON version)
(II) + developped by Bertrand LE GAL in 2022 ... 20xx
(II) +        and by Jeremie CRENNE  in 2012 ... 2021
(II) Binary generated : Nov  6 2022 - 17:05:51
(II) + Clang/LLVM version 15.0.3
(II) + Trace date and time : Sun Nov  6 17:44:59 2022 CET
(II)
(II) The puncturing is set to (8)
(II) The file was loaded (../galaxies/dubinski_colored.tab)
... ... ... ... ...
... ... ... ... ...
... ... ... ... ...
```

The reported values gives the frames per second (fps) performance metric. These values are high because no computation are executed (it is your job !).


## Observing the particule simulation

In the previous section use used the **Null** render to get the FPS performance level of the processing kernel implementation. It is usefull to measure performance because to OS / GUI hazard impacts on performance measurement. However, it is not really friendly for visuallization and debugging purpose. Consequently, we can change the vizualization render:

```
./galax_eirb --impl null --view ColorPixel
```

It should display a window like this bellow. Note that the particules do not move because the computation kernel is not implemented.


Using your mouse you can rotate and zoom to observe the simulation. If you remove the **view** flags from the command line you can get the list of the available renders:

```
./galax_eirb --impl null
```

You can add new renders or modify the existing ones, however, do forget the project objectives.



## Playing with the command line interface (CLI)

The easiest way to play with the galaxeib simulator is to take advantage of the execution flags supported by the program. You can add your own flags but these ones are directly supported: 

Required execution flags:

- **impl**       - used to select the optimized kernel version that should be executed,
- **view**       - used to specify the GUI that will show particule simulation,

Optionnal execution flags:

- **file**       - used to select a text file containing particules,
- **step**       - used to set a down sampling factor during the file loading (by default, its value is **16**). To load the overall set of particules from a file you shoud set its value to **1**,

- **width**      - used to set the width of the GUI window in pixels,
- **height**     - used to set the height of the GUI window in pixels,
- **colors**     - used to change the coloring methods for sprite based GUI vizualization,

- **delta-time** - used to fix the simulation time step.


## Installation on other computers

The initial configuration of the project should work on all ENSEIRB computers (Linux stations). However, it should also run on your laptops. However, intallation of additionnal libraries and tools seems necessary. In the following subsections, informations are given for common laptop configurations.

### Computers with Linux (INTEL - CPU)

On Linux machine with an Ubuntu or an ARCH Linux distribution, things should be seamless even on Raspberry-Pi 4 devices. GalaxEirb project relies on SDL2 and OpenGL libraries for particuple visualization purposes. You should thus install the library development files.

This can be dones as follow for Ubuntu users:

```
sudo apt update
sudo apt install freeglut3-dev libglew-dev libsdl2-dev
```

whereas ARCH Linux users should execute the following command:

```
sudo pamac upgrade
sudo pamac install freeglut3-dev libglew-dev libsdl2-dev
```

Installing these libraries should allow you to take advantage of the major features and optimization approaches. For the rest, e.g. CUDA parallelization, you still have the school computers.


### Computers with Linux (INTEL CPU + NVIDIA GPU)

If you are lucky enough to have an NVIDIA GPU you need to complete the previous procedure. In addition, you will have to install the NVIDIA CUDA framework (compiler for the GPU cores and the associated Linux libraries). As you will see, it is not really more complicated!

First of all you have to install the graphics libraries (SDL and OpenGL) as previously mentioned. For that the following commands should do the job.

```
sudo apt update
sudo apt install freeglut3-dev libglew-dev libsdl2-dev
```

The you should install the NVIDIA CUDA framework. Using for instance the officiel Ubuntu package, that is not the newest one, it should work like a charm.

```
sudo apt install nvidia-cuda-toolkit
```

These installed package should allow you to take advantage of the overall features and parallelization strategies.


https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html

### Computers with Linux (INTEL CPU + AMD RADEON GPU)

```
TODO
```

### Computers with MacOS (INTEL CPU)

```
TODO
```

### Computers with MacOS (INTEL CPU + AMD RADEON)

```
TODO
```

### Computers with MacOS (ARM CPU)

```
TODO
```

### Computers with Windows XY (any CPU)

I don't have a Windows computer so i am unable to help you. If a friendly student write a tutorial, i would add it here !