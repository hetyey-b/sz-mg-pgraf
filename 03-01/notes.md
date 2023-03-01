# 03-01
* Vad Viktor - vva@inf.elte.hu

* https://docs.google.com/document/d/18xALaVLc-t3NP8mwV3iGMm6EXsuRHc6hYI4RdLXzvBA/edit
* Grafikai programozás alapjai
* Ablak és eseménykezelés
* OpenGL API
    * Grafikus pipeline
    * Bufferek, textúrák
    * Vertex & fragment shaderek

* Pontgyűjtés
    * 13 gyakorlat (órai munka)
    * 2 kisbeadandó -> Fontos!
    * Házi feladatok (opcionális, de pontot ér)
    * ZH (4 órás) VAGY nagybeadandó (félév közepe táján lesz kiírva)

--------------------------------------

* Simple DirectMedia Layer 2.0
    * Ablakozó keretrendszer
    * Elfedi és megkönnyíti az operációs rendszerhez tartozó hívásokat
    * Illeszkedik az OpenGLhez
* GL Extension Wrapper (GLEW)
    * hivatalos OpenGL inicializáció több száz soros
    * GLEW ezt helyettesíti egy sorral
* OpenGL Mathematics (GLM)
    * hivatalos OpenGL nem tartalmaz matematikai könyvtárat
    * Header-only, kvázi szabványos megoldás erre


* http://cg.elte.hu/~bsc_cg/gyak/linux_readme.txt
Általános követelmények:
- g++ cmake libsdl2-dev libsdl2-image-dev csomagok telepítése
- OGLPACK_PATH környezeti változó beállítása ami az OGLPack mappára mutat (ls $OGLPACK_PATH -> "bin  include  lib")
  - ezt az `export OGLPACK_PATH=/path/to/OGLPack` paranccsal tudjuk megtenni a terminálban, vagy permanenssé tehetjük, ha egy login szkriptbe írjuk be (pl bash esetén ~/.bashrc végére)

Fordítás:
- A projekt nevével megegyező mappába másoljuk be a CMakeLists.txt-t az sln fájl mellé
- Például ha a solution mappájának neve 01_OGLBase, akkor a mappaszerkezet a következő:
  - 01_OGLBase *
    - 01_OGLBase *
      - main.cpp
      - MyApp.cpp
      - ...
    - 01_OGLBase.sln
    - build
    - CMakeLists.txt
- Fontos, hogy a *-gal jelölt mappák nevei megegyezzenek!
- Fordítsuk le a projektet:
  - mkdir build && cd build
  - cmake ..
  - make
- Futtatás a build mappából a CG_PROJ nevű állomány indításával: 
  - ./CG_PROJ


  * Házi feladat: Ugyanaz, mint a 2. feladat, de csak akkor működjön, ha valamelyik gomb nyomva van tartva
