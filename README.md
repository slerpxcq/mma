# MMX (MM eXtended)
A source-opened implementation of MikuMikuDance with modern OpenGL 

## Build
Windows (VS2022)
```
gen.bat
```
## Getting started
The GUI should be self explanatory enough. But here comes some hotkeys 

| Key | Description
| --- | --- |
```T``` | translate
```R``` | rotate
```W``` | toggle gizmo mode (world/local)
```X/Y/Z``` | toggle axes 

## Features
- Full GPU skinning and morphing (position, uv)
## References

Hazel game engine
 - https://github.com/TheCherno/Hazel

ShiftJIS conversion
- https://stackoverflow.com/questions/33165171/c-shiftjis-to-utf8-conversion

Inverse kinematic 
- https://rodolphe-vaillant.fr/entry/114/cyclic-coordonate-descent-inverse-kynematic-ccd-ik

Spherical blending
- https://users.cs.utah.edu/~ladislav/kavan05spherical/kavan05spherical.html
- http://image.diku.dk/projects/media/kasper.amstrup.andersen.07.pdf

MMD SDEF:
- https://github.com/powroupi/blender_mmd_tools/issues/162

Morph (SSBO):
- https://techblog.sega.jp/entry/2016/10/27/140454

VMD file specification:
- https://hariganep.seesaa.net/article/201103article_1.html

PMX file specification:
- That one comes with PMXEditor, or for English version
- https://gist.github.com/felixjones/f8a06bd48f9da9a4539f
- https://gist.github.com/DeXP/16ccdd09841bdc1961e0
