// stdafx.cpp : 只包括标准包含文件的源文件
// OpenGLProject01.pch 将作为预编译标头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用
#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  在此处引用程序需要的其他头文件
#include <glew.h> // 注意glew.h在前面，然后才是freeglut.h
#include <freeglut.h>
#pragma comment(lib,"glew32.lib") // 你也可以在项目->属性->链接器->输入->附加依赖项中添加这个库文件
#include <iostream>
using namespace std;