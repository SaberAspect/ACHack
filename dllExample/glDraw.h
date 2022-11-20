#pragma once
#pragma comment(lib, "OpenGL32.lib")
#include <Windows.h>
#include <stdio.h>
#include <gl/GL.h>

namespace rgb
{
	const GLubyte red[4] = { 255,0,0, 255};
	const GLubyte green[4] = {0,255,0,255 };
	const GLubyte gray[4] = { 55,55,55,255 };
	const GLubyte lightgray[4] = { 192,192,192,255 };
	const GLubyte black[4] = { 0,0,0,255 };
}

namespace GL
{
	void SetupOrtho();
	void RestoreGL();

	void DrawFilledRect(float x, float y, float width, float height, const GLubyte color[4]);
	void DrawOutline(float x, float y, float width, float height, float lineWidth, const GLubyte color[4]);

}