#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>




#define TGA_RGB		2						// This tells us it's a normal RGB (really BGR) file
#define TGA_A		3						// This tells us it's a ALPHA file
#define TGA_RLE		10						// This tells us that the targa is Run-Length Encoded (RLE)


typedef struct{
	GLuint			image[3];				// channels / width / height
	GLubyte		*	data;					// hold data
}tImageData; 


typedef struct{
	GLubyte			info[3];				// ID length/Colourmap type/datatype code
	GLushort		colourmapOrigin;
	GLushort		colourmapLength;
	GLubyte			colourmapDepth;
	GLushort		data[4];				// X/Y/Width/Height origin
	GLubyte			bitsperPixel;
	GLubyte			imageDescriptor;
}TGA_HEADER;


void		LoadTGA(const char *);
void		Read_TGA();


void		Mainloop(GLFWwindow*);

#endif
