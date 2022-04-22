# Лабораторная работа №2 по дисциплине "Инженерная и компьютерная графика"
***
## 1) Перемещение треугольника по координате X
```c++
GLuint gWorldLocation; //создание глобальной переменной 
```
Добавление кода для вершинного шейдера
```c++
static const char* pVS = "                                                         
#version 330 //версия шейдера                                                                                                                                          
layout (location = 0) in vec3 Position; // Входной тип данных - vec3 в позиции 0                                                         
uniform mat4 gWorld;   // uniform-переменная типа mat4 
                                                                                    
void main()                                                                        
{                                                                                   
    gl_Position = gWorld * vec4(Position, 1.0);  // Умножаем вектор вершин на всемирную матрицу для смещения треугольника                                   
}";
```
Добавление кода для пиксельного шейдера
```c++
static const char* pFS = "                                                          
#version 330 //версия шейдера                                                                                                                         
out vec4 FragColor;                                                                                                                                 
void main()                                                                        
{                                                                                  
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);                                          
}";
```


## 2) 
```c++

```
