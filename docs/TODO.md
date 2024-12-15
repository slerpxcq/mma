
# 2024/9/28
- [ ] Command line argument handling
- [x] Global variable handling 

# 2024/10/2
- [x] Integrate google test
    - [x] Incoperate premake and CMake
- [x] Change to native building system for each submodule
    Submodules to change:
    - [x] bullet3
    - ~~[ ] icu~~
    - [x] glfw
    - [x] EventBus
    - [x] spdlog
    - [x] GoogleTest
    - [x] yaml-cpp

# 2024/10/3
- [x] Get an idea of scene graph system
    - [x] Node based? - Yes, node stores references to underlying components (ECS-like)
- [x] Get logger working

# 2024/10/4
- [x] Get rid of all singleton class

- [x] PMX file loading
    - [x] Refactor PMXFile class
    - [x] Add PMXParser class
        - [x] Stream reading and trimming
# 2024/10/5
- [x] Testing PMX file loading

# 2024/10/27

# 2024/10/29
- [x] Mesh class
    - [ ] Load mesh from PMX file
        - [x] IndexBuffer VertexBuffer 
        - [x] Load index data to IndexBuffer
        - [x] Load vertex data to VertexBuffer
            - [x] VertexLayout
        - [x] Load VertexArray and set attributes
            - [x] VertexArray class, functions
        - [x] Load submesh offsets
- [x] Move Root node to global attributes

# 2024/10/30
- [x] Load Textures from PMXFile
    - [x] Texture class
    - [x] Texture2D class
- [x] FrameBuffer class

# 2024/10/31
- [x] EditorLayer
- [x] ViewportPanel class
    - [x] FrameBuffer related functions

# 2024/11/1
- [x] Shader related
    - [x] Shader loading
        - [x] Text class
        - [x] Shader creation/deletion related functions
        - [x] Program related functions
        - [x] Uniform related functions
    - [x] Shader testing
        - [x] Load text file

# 2024/11/2
- [x] Draw model  
    - [x] Setup uniforms 
    - [x] Bind framebuffer
    - [x] Draw meshes
- [x] Framebuffer resize from Viewport

# 2024/11/4
- [x] Camera class
- [x] Viewport class
- [x] Link Viewport to ViewportPanel
    - [x] CameraController

# 2024/11/5
- [x] Draw model with texture
- [x] Add blending
- [x] Draw grid
    - [x] Grid shader
- [x] Fix camera controller (world/local transform)
    - View matrix = world matrix inverse
    - View vector(world)

# 2024/11/6
- [x] Change PMXFile access permission -> private
- [x] SceneHierarchy panel
- [x] Load bones

# 2024/11/8
- [x] Skinning 
    - [x] Shader storage buffer
    - [x] VPDFile class

# 2024/11/9
- [x] Changed node and SceneObject ownership

# 2024/11/11
- [x] Transform assignment 

# 2024/11/12
- [x] IK solver
- [ ] PoseEditorOverlay
    - [x] Load extra bone info from PMX
        - [x] Bone tip
        - [x] Fix unconnected bone tip position calculation

# 2024/11/16
- [x] Bone transform edit/Gizmo
    - [x] Change bone bindLocal calculation to pass 2
    - [x] Load local axis/fixed axis to bone
    - [x] Gizmo context compute

- [x] Load rigidbodies 

# 2024/11/17
[x] Physics debug draw overlay

# 2024/12/14
- [ ] Assert pipeline
    - [ ] Investigate GLTF
    - [ ] Convert PMX model to unified format
        - [ ] .mesh: vertices, indices; binary
        - [ ] .morph: morphs; binary
        - [ ] .model: yaml script; text

--- 
# TODO
Editor:
[ ] Global keymap
[ ] TransformEditOverlay state machine
[ ] Overlap bone sorting
[ ] Curve editor
[ ] Editor global event

Core:
[ ] Animation loading
[ ] Scene serialization
[ ] ResourceManager, ResourceHandle (auto release)
[ ] Multi lighting



