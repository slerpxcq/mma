# TODO
- The GPU resources, such as GLTexture and GLBuffer, should be owned by a seperate class like GLResource,
and other classes should only holds a pointer to the actual resource;

- The ResourceManager should hold only resources that are not a GPU resource, such as files
So it should be called FileManager to be precise

- Better logging info


# DOING
- The sequencer class is too messy, try to refractor
    - Refactor selection logic: State machine

- Sequencer copy and paste
    - Find a uniform way to handle morph and bone keyframes
    - How to get the container that a keyframe is belong to? Need type and index;
      The underlying Animation class has a seperate storage for different types of keyframes.
      However, they should be treated as one type in the Sequencer

# 2024/5/3
# DONE
- Read IBL-Diffuse irradiance (LearnOpenGL)
    - Equirectangular map -> Cube map

- Handling multi-selection
- Handling pose commitment 

# 2024/5/4
# DONE
- Copy paste bone transforms 
    - DO NOT use glfw clipboard! Create a custom one
- OnFrameSet event

# 2024/5/6