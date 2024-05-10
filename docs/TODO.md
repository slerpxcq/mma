
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

# 2024/5/8
# DONE
- Sequencer copy and paste
    - Keyframe dragged
    - Delete keyframe
    - Cut keyframes

# 2024/5/9
# DONE
- Set the container when click on bone
    - Add buttons in Sequencer legends

    - Display a curve and able to change the min and max frame
    - Grid

# 2024/5/10
# DONE
    - Bezier handle
        - How to identify current selected dope? - Use pointer
    - Mouse drag to pan

# DOING
- Curve editor
    - Horizontal/vertical scale
    - Rotation bezier???
        - Interpolate Euler is not equivalent to slerp
            - Draw the curve by evaluating slerp at each frame, but they cannot be directly edited
            - Must be edited through a single Bezier 
            - Try to draw the curve first

# TODO
- Entity/Property panel
- Entity base class
    - ShowPropertyPanel()
- Entity derived class for 
	TYPE_BONE,
    TYPE_MESH,
    TYPE_CAMERA,
    TYPE_LIGHT,
    TYPE_MORPH,
    TYPE_ANIMATION,
    TYPE_ARMATURE,
    TYPE_WORLD,
    TYPE_PHYSICS_WORLD,
    TYPE_MODEL

- The GPU resources, such as GLTexture and GLBuffer, should be owned by a seperate class like GLResource,
and other classes should only holds a pointer to the actual resource;

- The ResourceManager should hold only resources that are not a GPU resource, such as files
So it should be called FileManager to be precise

- Better logging info

- ImGuizmo Local ring and Screen ring radius too close

- All undo redo commands
    - Sequencer
        - Insert keyframe?
        - Delete keyframe
        - Drag keyframe
    - Pose Editor
        - change transform
    - Properties
        - Change any field

- Copy and paste bone transforms
    - If single bone transform is copied, it can be pasted on any other bone
    - If multiple bone transforms are copied, they can be only pasted on the same bone

- PoseEditor should be able to undo even when editing
