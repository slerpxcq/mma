
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
- [ ] EditorLayer
    - [ ] ViewportPanel class
        - [ ] FrameBuffer class

--- 
# TO BE DONE:
- [ ] Drawable interface
- [ ] ModelNode class
    - [ ] Load from PMX file
    - [ ] Keep references of textures, materials and meshes
