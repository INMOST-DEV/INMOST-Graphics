#include "inc_glut.h"
#include "tga.h"
#include <stdio.h>

extern void draw_screen(); //global openl drawing routine

void screenshot(int tiles)
{
	size_t height = glutGet(GLUT_WINDOW_HEIGHT);
	size_t width = glutGet(GLUT_WINDOW_WIDTH);
	size_t oldwidth = width;
	size_t oldheight = height;
	width *= tiles;
	height *= tiles;

	char * pixelbuffer = new char[width*height * 3 + oldwidth*oldheight * 3];
	char * tempbuffer = pixelbuffer + width*height * 3;
	//int WH[2];
	//glGetIntegerv(GL_VIEWPORT,WH);
	//printf("W %d H %d\n",WH[0],WH[1]);

	glPrintError();
	for (int i = 0; i < tiles; ++i)
	{
		for (int j = 0; j < tiles; ++j)
		{
			glViewport(-oldwidth*i, -oldheight*j, width, height);
			glPrintError();
			draw_screen();
			glPrintError();
			glReadBuffer(GL_BACK);
			glPrintError();
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			//glReadPixels(0, 0, oldwidth, oldheight, GL_BGR_EXT, GL_UNSIGNED_BYTE, tempbuffer);
			glReadPixels(0, 0, oldwidth, oldheight, GL_RGB, GL_UNSIGNED_BYTE, tempbuffer);
			glPrintError();

			size_t koff = oldwidth*(i);
			size_t loff = oldheight*(j);

			for (size_t l = 0; l < oldheight; ++l)
			for (size_t k = 0; k < oldwidth; ++k)
			for (int m = 0; m < 3; ++m)
				pixelbuffer[((koff + k) + (loff + l)*width) * 3 + m] = tempbuffer[(k + l*oldwidth) * 3 + (2-m)];

			//filename[0] += i;
			//filename[1] += j;
			//write_tga(filename,width,height,pixelbuffer);
			//filename[0] = filename[1] = '0';
		}
	}

	/*
	glViewport(-oldwidth*3,-oldheight*3,width,height);
	draw_screen();
	glReadBuffer(GL_BACK);
	glReadPixels(0,0,width,height,GL_BGR_EXT,GL_UNSIGNED_BYTE,pixelbuffer);
	*/





	write_tga("screenshot.tga", width, height, pixelbuffer);
	delete[] pixelbuffer;
	width = oldwidth;
	height = oldheight;
//#if !(defined(MAC_WORKAROUND) && (defined (__APPLE__) || defined(MACOSX)))
	glViewport(0, 0, width, height);
//#endf //MAC_WORKAROUND
}

#if defined(USE_PNG)
#include <png.h>
#endif

void screenshot_png(int tiles, std::string fname)
{
	size_t height = glutGet(GLUT_WINDOW_HEIGHT);
	size_t width = glutGet(GLUT_WINDOW_WIDTH);
	size_t oldwidth = width;
	size_t oldheight = height;
	width *= tiles;
	height *= tiles;
	
	unsigned char* pixelbuffer = new unsigned char[width * height * 3 + oldwidth * oldheight * 3];
	unsigned char* tempbuffer = pixelbuffer + width * height * 3;
	//int WH[2];
	//glGetIntegerv(GL_VIEWPORT,WH);
	//printf("W %d H %d\n",WH[0],WH[1]);

	glPrintError();
	for (int i = 0; i < tiles; ++i)
	{
		for (int j = 0; j < tiles; ++j)
		{
			glViewport(-oldwidth * i, -oldheight * j, width, height);
			glPrintError();
			draw_screen();
			glPrintError();
			glReadBuffer(GL_BACK);
			glPixelStorei(GL_PACK_ALIGNMENT,1);
			glPrintError();
			glReadPixels(0, 0, oldwidth, oldheight, GL_RGB, GL_UNSIGNED_BYTE, tempbuffer);
			glPrintError();
			size_t koff = oldwidth * (i);
			size_t loff = oldheight * (j);
			

			for (size_t l = 0; l < oldheight; ++l)
				for (size_t k = 0; k < oldwidth; ++k)
					for (int m = 0; m < 3; ++m)
						pixelbuffer[((koff + k) + (loff + l) * width) * 3 + m] = tempbuffer[(k + l * oldwidth) * 3 + m];
		}
	}


#if defined(USE_PNG)
	{
		png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr)
		{
			png_infop info_ptr = png_create_info_struct(png_ptr);
			if (info_ptr)
			{
				if (!setjmp(png_jmpbuf(png_ptr)))
				{
					FILE* fp = fopen(fname.c_str(), "wb");
					if (fp)
					{
						png_init_io(png_ptr, fp);

						if (!setjmp(png_jmpbuf(png_ptr)))
						{
							png_set_IHDR(png_ptr, info_ptr, width, height,
								8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
								//PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
								PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

							png_write_info(png_ptr, info_ptr);

							if (!setjmp(png_jmpbuf(png_ptr)))
							{
								png_bytep* row_pointers = new png_bytep[height];
								for (int k = 0; k < height; k++) 
									row_pointers[k] = pixelbuffer + (height - k - 1) * width * 3;

								png_write_image(png_ptr, row_pointers);
								if (!setjmp(png_jmpbuf(png_ptr)))
									png_write_end(png_ptr, NULL);
								else std::cout << "[write_png_file] Error during end of write" << std::endl;

								delete[] row_pointers;
							}
							else std::cout << "[write_png_file] Error during writing bytes" << std::endl;
						}
						else std::cout << "[write_png_file] Error during writing header" << std::endl;
						fclose(fp);
					}
					else std::cout << "[write_png_file] File %s could not be opened for writing" << std::endl;
				}
				else std::cout << "[write_png_file] Error during init_io" << std::endl;
			}
			else std::cout << "[write_png_file] png_create_info_struct failed" << std::endl;
		}
		else std::cout << "[write_png_file] png_create_write_struct failed" << std::endl;
	}
#else // USE_PNG
	std::cout << "Cannot output png without libpng, activate USE_PNG in cmake!" << std::endl;
#endif // USE_PNG



	//write_tga("screenshot.tga", width, height, pixelbuffer);
	delete[] pixelbuffer;
	width = oldwidth;
	height = oldheight;
	//#if !(defined(MAC_WORKAROUND) && (defined (__APPLE__) || defined(MACOSX)))
	glViewport(0, 0, width, height);
	//#endf //MAC_WORKAROUND
}
