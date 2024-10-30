# Naming convensions
## Case 
| Entity | Convention | Note/Example |
| --- | --- | --- |
| Project | PascalCase | |
| Namespace | PascalCase | Except ```mm```  |
| Class | PascalCase | | |
| Member function | PascalCase | | 
| Free function | PascalCase | | 
| Local variable | camelCase | |
| Private member variable | m_camelCase | |
| Public member variable | camelCase | |
| Static variable | s_camelCase | |
| Global variable | g_camelCase | |
| Constant | ALL_CAPS | | 
| Marco | ALL_CAPS | | 

# Naming
| Entity | Convention | Note/Example |
| --- | --- | --- |
| Function | DoSomething() |
| Query function | IsEnabled()/HasProperty() | 

# Initialization
| Entity | Convention | Note/Example |
| --- | --- | --- |
| All variables | Trailing {} | ```int x{};``` |
| Initializer list | Use {} | ```X(int x) : m_x{x} {}``` |
| Default argument | Use {} | ```X(int x = {}) : m_x{x} {}``` |

# Runtime
All project and modules should use /MDd for debug and /MD for release. **DO NOT** use /MTd or /MT

# Type
| Entity | Convention | Note/Example |
| --- | --- | --- |
| Loop counter | i32 | To prevent down counting overflow |
| Size/Count | i32 | Same as above |
| Bit field | u<8/16/32> |  |

# Indentation, braces
For all statements, even single line, wrap with ```{}```
```C++
if (true) {
    Foo();
} 
else {
    Bar();
}
```

For top level scope, ```{``` in new line
```C++
void func() 
{
}
```

For non-top level scope, ```{``` keeps in the same line
```C++
void func() 
{
    if (true) {

    } 
    else {

    }
}
```

# OpenGL
All textures are immutable 
