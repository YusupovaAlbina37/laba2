#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
using namespace glm;
GLuint VBO; //глобальная переменная, хранящая указатель на буфер вершин
GLuint gWorldLocation; //создание глобальной переменной 

//вершинный шейдер
static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWorld;                                                                \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWorld * vec4(Position, 1.0);                                     \n\
}";

//пиксельный шейдер
static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);                                           \n\
}";

static void RenderSceneCB() // функция отрисовки
{
	glClear(GL_COLOR_BUFFER_BIT); //очистка буфера кадра

	static float Scale = 0.0f; //переменная для изменения значения X

	Scale += 0.001f; //увеличиваем данную переменную с каждой отрисовкой

	//перемещение треугольника по координате X
	mat4x4 translate2;
	translate2[0][0] = 1.0f; translate2[0][1] = 0.0f; translate2[0][2] = 0.0f; translate2[0][3] = sinf(Scale);
	translate2[1][0] = 0.0f; translate2[1][1] = 1.0f; translate2[1][2] = 0.0f; translate2[1][3] = 0.0f;
	translate2[2][0] = 0.0f; translate2[2][1] = 0.0f; translate2[2][2] = 1.0f; translate2[2][3] = 0.0f;
	translate2[3][0] = 0.0f; translate2[3][1] = 0.0f; translate2[3][2] = 0.0f; translate2[3][3] = 1.0f;

	//вращение треугольника
	mat4x4 translate4;
	translate4[0][0] = cosf(Scale); translate4[0][1] = -sinf(Scale); translate4[0][2] = 0.0f; translate4[0][3] = 0.0f;
	translate4[1][0] = sinf(Scale); translate4[1][1] = cosf(Scale);  translate4[1][2] = 0.0f; translate4[1][3] = 0.0f;
	translate4[2][0] = 0.0f;        translate4[2][1] = 0.0f;         translate4[2][2] = 1.0f; translate4[2][3] = 0.0f;
	translate4[3][0] = 0.0f;		   translate4[3][1] = 0.0f;         translate4[3][2] = 0.0f; translate4[3][3] = 1.0f;

	//преобразование масштаба треугольника
	mat4x4 translate3;
	translate3[0][0] = sinf(Scale); translate3[0][1] = 0.0f;        translate3[0][2] = 0.0f;        translate3[0][3] = 0.0f;
	translate3[1][0] = 0.0f;        translate3[1][1] = cosf(Scale); translate3[1][2] = 0.0f;        translate3[1][3] = 0.0f;
	translate3[2][0] = 0.0f;        translate3[2][1] = 0.0f;        translate3[2][2] = sinf(Scale); translate3[2][3] = 0.0f;
	translate3[3][0] = 0.0f;        translate3[3][1] = 0.0f;        translate3[3][2] = 0.0f;        translate3[3][3] = 1.0f;


	mat4x4 translate = translate2 * translate4 * translate3; //объединение преобразований

	//загрузка матрицы размером 4x4
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &translate[0][0]);
	//Первый параметр - адрес uniform-переменной
	//Второй параметр - количество матриц, значения которых мы обновляем. 
	//Третий параметр - параметр, определяющий передается ли матрица по строкам
	//Четвертый параметр - указатель на первый элемент матрицы

	glEnableVertexAttribArray(0); //координаты вершин, используемые в буфере, рассматриваются как атрибут вершины с индексом 0
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //обратно привязываем наш буфер, приготавливая его для отрисовки
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // вызов говорит конвейеру как воспринимать данные внутри буфера

	glDrawArrays(GL_TRIANGLES, 0, 3); //вызвали функцию для отрисовки (Мы рисуем треугольники вместо точек и принимаем 3 вершины вместо 1)

	glDisableVertexAttribArray(0); //отключаем каждый атрибут вершины

	glutSwapBuffers(); //функция просит GLUT поменять фоновый буфер и буфер кадра местами
}


static void InitializeGlutCallbacks() //инициализация GLUT-вызовов
{
	glutDisplayFunc(RenderSceneCB); //отрисовываем 1 кадр
	glutIdleFunc(RenderSceneCB); //функция устанавливает функцию в программе, которая будет отвечать за перерисовку окна
}

static void CreateVertexBuffer() //функция создания буфера вершин
{
	vec3 Vertices[3]; //массив векторов для трёх вершин
	Vertices[0] = vec3(-1.0f, -1.0f, 0.0f); //создаём вектор с координатами x=-1, y=-1, z=0
	Vertices[1] = vec3(1.0f, -1.0f, 0.0f); //создаём вектор с координатами x=1, y=-1, z=0
	Vertices[2] = vec3(0.0f, 1.0f, 0.0f); //создаём вектор с координатами x=0, y=1, z=0

	glGenBuffers(1, &VBO); //генерация объектов переменных типов (количество объектов, ссылка на массив для хранения)
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //буфер будет хранить массив вершин
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); //наполняем объект данными
	//вызов выше принимает название цели (такое же как и при привязывании), размер данных в байтах, 
	//адрес массива вершин, и флаг, который обозначает использование паттернов для этих данных
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType) //функция добавления шейдера к программе
{
	GLuint ShaderObj = glCreateShader(ShaderType); //создание шейдера

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	//сохранение исходного кода шейдера в массиве
	const GLchar* p[1]; //указатель на массив типа char
	p[0] = pShaderText;
	GLint Lengths[1]; //массив для кодов шейдера
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths); //функция указания исходного кода (тип шейдера, размерность обоих массивов, название шейдера, длина)
	glCompileShader(ShaderObj); //компиляция шейдера

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	} //проверка успешности компиляции шейдера

	glAttachShader(ShaderProgram, ShaderObj); //присоединяем скомпилированный объект шейдера к объекту программы
}

static void CompileShaders() //функция компиляции программы-шейдера
{
	GLuint ShaderProgram = glCreateProgram(); //создание программы-шейдера

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER); //функция в качестве параметров принимает (программный объект, название шейдера, тип шейдера), добавление шейдера для вершин
	AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER); //добавление пиксельного шейдера
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram); //линковка программы
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	} //проверка успешности линковки

	glValidateProgram(ShaderProgram); //вызов проверяет сможет ли программа запуститься с текущим состоянием конвейера

	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	} //проверка успешности валидации программы

	glUseProgram(ShaderProgram); //для использования отлинкованной программы шейдеров мы назначаем её для конвейера используя этот вызов

	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld"); //сохранение местоположения переменной gWorld
	assert(gWorldLocation != 0xFFFFFFFF);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); //здесь мы инициализируем GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //так настраиваются некоторые опции GLUT
	//GLUT_DOUBLE включает двойную буферизацию
	glutInitWindowSize(1024, 768); //задание размера окна (ширина, высота)
	glutInitWindowPosition(100, 100); //задание начальной позиции окна
	glutCreateWindow("Tutorial 06"); // создаём окно с названием "Tutorial 06"

	InitializeGlutCallbacks(); //вызов функции инициализации GLUT-вызовов

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	} // инициализируем GLEW и проверяем на ошибки.

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Устанавливаем цвет, который будет использован во время очистки буфера кадра.
	//Цвет имеет 4 канала (красный, зелёный, синий, альфа-канал) и принимает значения от 0.0 и до 1.0

	CreateVertexBuffer(); //вызов функции создания буфера вершин 

	CompileShaders(); //вызов функции компиляции программы-шейдера

	glutMainLoop(); //этот вызов передаёт контроль GLUT'у, который теперь начнёт свой собственный цикл

	return 0;
}