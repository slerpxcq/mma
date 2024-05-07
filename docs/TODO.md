# TODO
- The GPU resources, such as GLTexture and GLBuffer, should be owned by a seperate class like GLResource,
and other classes should only holds a pointer to the actual resource;

- The ResourceManager should hold only resources that are not a GPU resource, such as files
So it should be called FileManager to be precise

- Better logging info

- ImGuizmo Local ring and Screen ring radius too close

- Copy and paste bone transforms
    - If single bone transform is copied, it can be pasted on any other bone
    - If multiple bone transforms are copied, they can be only pasted on the same bone
- Cut/Delete keyframes

# DOING
- Sequencer copy and paste
    - Undo for 
        - Keyframe paste

# 2024/5/3
# DONE
- Read IBL-Diffuse irradiance (LearnOpenGL)

- Handling multi-selection
- Handling pose commitment 

# 2024/5/4
# DONE
- Copy paste bone transforms 
    - DO NOT use glfw clipboard! Create a custom one
- OnFrameSet event

# 2024/5/6
# DONE
- The sequencer class is too messy, try to refractor
    - Refactor selection logic: State machine

    - Find a uniform way to handle morph and bone keyframes
    - How to get the container that a keyframe is belong to? Need type and index;
      The underlying Animation class has a seperate storage for different types of keyframes.
      However, they should be treated as one type in the Sequencer
    - Solved with template

# 2024/5/7
# DONE
- Need other method to identify keyframes - solved by using std::list instead of std::vector
    - Underlying pointer changed due to insertion!

    - Box select has too many items - Check existance before emplace

    - Added auto-scrolling when click on scale
    - Duplicated dopes needs to be eliminated

        - Keyframe dragged