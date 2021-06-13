#include "functions.h"




GLuint	gTexture;


void SizeOpenGLScreen(int width, int height){

	if (height == 0){
		height = 1;
	}

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, .5f, 150.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void Read_TGA( tImageData * data, FILE * pFile){

	//					 ** channel  ** / **  width   **
	GLuint		stride = data->image[0] * data->image[1];	
	int 		e;

	for(e = 0; e < data->image[2]; e++){
		GLubyte * pLine = &(data->data[stride * e]);
		fread(pLine, stride, 1, pFile);
	}
}


void Read_RLE( tImageData * data, FILE * pFile){

	// printf("\nFUNCTION READING RLE COMPRESSION");
	// printf("\nuse GL_BGR_EXT verses GL_RGB");
	GLint			bSize	=  sizeof(GLubyte) * data->image[0];
	GLint			i		= 0;
	GLint			cRead	= 0;
	GLubyte			rleID	= 0;
	GLubyte		*	rColors = (GLubyte *)malloc(bSize);
	
	while( i < data->image[1] * data->image[2]){
	
		fread( &rleID, sizeof(GLubyte), 1, pFile);

		if(rleID < 128){
			rleID++;	

			while(rleID){

				fread( rColors, bSize, 1, pFile);

				data->data[cRead + 0]		= rColors[0];
				data->data[cRead + 1]		= rColors[1];
				data->data[cRead + 2]		= rColors[2];

				if(data->image[0] == 4)
					data->data[cRead + 3]	= rColors[3];

				i++;
				rleID--;
				cRead += data->image[0];
			}
		}else{
			rleID -= 127;
			fread( rColors, bSize, 1, pFile);

			while(rleID){

				data->data[cRead + 0]		= rColors[0];
				data->data[cRead + 1]		= rColors[1];
				data->data[cRead + 2]		= rColors[2];

				if(data->image[0] == 4)
					data->data[cRead + 3]	= rColors[3];

				i++;
				rleID--;
				cRead += data->image[0];
			}
		}
	}

	free(rColors);
}



void LoadTGA(const char *path){

	tImageData		* tData;	
	TGA_HEADER		* Header;
	FILE			* pFile = NULL;

	if ((pFile = fopen(path, "rb")) == NULL){
		printf("\n file %s missing, please check again", path);
		return;
	}

	Header = (TGA_HEADER *)malloc(sizeof(TGA_HEADER));

	fread(Header->info, 3 * sizeof(GLubyte), 1, pFile);
	fread(&Header->colourmapOrigin, 	sizeof(GLushort),	1, pFile);
	fread(&Header->colourmapLength, 	sizeof(GLushort),	1, pFile);

	fread(&Header->colourmapDepth, 		sizeof(GLubyte),	1, pFile);
	fread( Header->data, 			4 * sizeof(GLushort),	1, pFile);
	fread(&Header->bitsperPixel, 		sizeof(GLubyte),	1, pFile);
	fread(&Header->imageDescriptor, 	sizeof(GLubyte),	1, pFile);

	tData				=  (tImageData *)malloc(sizeof(tImageData));

	tData->image[0]		= (GLuint)Header->bitsperPixel / 8;
	tData->image[1]		= (GLuint)Header->data[2];
	tData->image[2]		= (GLuint)Header->data[3];
	tData->data			= (GLubyte* )malloc(tData->image[0] * tData->image[1] * tData->image[2]);		//	<< === allocated data

	// printf("\nImage size:	 %i", tData->image[1] * tData->image[2]);
	// printf("\nImage channel: %i", tData->image[0]);

	switch(Header->info[2]){
		case TGA_RGB:
			Read_TGA(tData, pFile);
			break;

		case TGA_A:
			
			break;

		case TGA_RLE:
			Read_RLE(tData, pFile);
			break;

		default:
			break;
	}

	free( Header);
	
	glGenTextures( 1, &gTexture);
	glBindTexture( GL_TEXTURE_2D, gTexture);

	gluBuild2DMipmaps(GL_TEXTURE_2D, tData->image[0], tData->image[1], tData->image[2], GL_BGRA, GL_UNSIGNED_BYTE, tData->data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);		

	free(tData->data);
	free(tData);

	fclose(pFile);
}


void Mainloop(GLFWwindow *wnd){

	SizeOpenGLScreen(800, 600);

	while (!glfwWindowShouldClose(wnd)){

		if (glfwGetKey(wnd, GLFW_KEY_ESCAPE) == GLFW_PRESS){
			glfwSetWindowShouldClose(wnd, 1);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		gluLookAt(0, 0, 6, 0, 0, 0, 0, 1, 0);

		glBindTexture( GL_TEXTURE_2D, gTexture);
		glEnable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0,  1.0, 0.0);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0, -1.0, 0.0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f( 1.0, -1.0, 0.0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f( 1.0,  1.0, 0.0);
		glEnd();

		glfwPollEvents();
		glfwSwapBuffers(wnd);
	}

	glDeleteTextures( 0, &gTexture);
}
