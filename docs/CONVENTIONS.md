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
| Member variable | m_camelCase | |
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