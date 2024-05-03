# TODO
- The GPU resources, such as GLTexture and GLBuffer, should be owned by a seperate class like GLResource,
and other classes should only holds a pointer to the actual resource;

- The ResourceManager should hold only resources that are not a GPU resource, such as files
So it should be called FileManager to be precise

- Better logging info

- Copy paste bone transforms

# DOING

# 2024/5/3
# DONE
- Read IBL-Diffuse irradiance (LearnOpenGL)
    - Equirectangular map -> Cube map

- Handling multi-selection
- Handling pose commitment 