#include <stdio.h>
#include <math.h>
#include <GL/gl.h>


#include <iostream>
#include <fstream>


#define checkImageWidth 512
#define checkImageHeight 512



using namespace std;

typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char uchar;

fstream inf;

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];


class mRGB
{
public:
	uchar r,g,b,a;
	mRGB(){r = g = b = 0,a=255;}
};

class RGBpixmap
{
	public:
		int nRows,nCols;
		mRGB *pixel;
		void readBMPFile(char *frame);
		void makeCheckImage();
		void setTexture(GLuint textureName);
};


void RGBpixmap::makeCheckImage()
{
	int i, j, c;
	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = ( ((i&0x8)==0) ^ ((j&0x8)==0) )*255;
			checkImage[i][j][0] = (GLubyte) c;
			checkImage[i][j][1] = (GLubyte) c;
			checkImage[i][j][2] = (GLubyte) c;
			checkImage[i][j][3] = (GLubyte) 255;
		}
	}
}


void RGBpixmap::setTexture(GLuint textureName)
{
	glBindTexture(GL_TEXTURE_2D,textureName);
	//This function enables you to create or switch to a named texture state.
	// On first use, this function creates a new texture state
	//identified by the texture name, which is an unsigned integer.
	//Subsequent calls with the same texture identifier select that texture state and make it current.
	//parameters:
	//P1: GLenum: The texture target to bind to.
		//It must be GL_TEXTURE_1D, GL_TEXTURE_2D, or GL_TEXTURE_3D.
	//P2: GLuint: The name or handle of the texture object.

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//This function sets several texture mapping parameters.
	//These parameters are bound to the current texture state
	//that can be made current with glBindTexture.
	//parameters:
	//P1: GLenum: The texture target for which this parameter applies.
		//Must be one of GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, or GL_TEXTURE_CUBE_MAP.
	//P2: GLenum: The texturing parameter to set.
		//GL_TEXTURE_MAG_FILTER  Returns the texture magnification filter value
	//P3: GLfloat or GLfloat* or GLint or GLint*: Value of the parameter specified by pname.

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//GL_TEXTURE_MIN_FILTER: Specifies the texture image minification (reduction) method or filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	//Defines parameters for texture coordinate generation
	//P1: The texture coordinate to map. It must be one of GL_S, GL_T, GL_R, or GL_Q.
	//P2: The parameter to set. It must be one of GL_TEXTURE_GEN_MODE, GL_OBJECT_PLANE, or GL_EYE_PLANE.
	//P3: GL_OBJECT_LINEAR: Texture coordinates are calculated from object (vertex) coordinates.

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//set texture mapping environment parameters
	//P1: The texture environment to define. It must be GL_TEXTURE_ENV or GL_TEXTURE_FILTER_CONTROL.
	//P2: When the target is GL_TEXTURE_ENV, the parameter name may be GL_TEXTURE_ENV_MODE, GL_TEXTURE_ENV_COLOR, GL_COMBINE_RGB, or GL_COMBINE_ALPHA.
    //P3: When the parameter name is GL_TEXTURE_ENV_MODE, the valid parameters are GL_REPLACE, GL_DECAL, GL_MODULATE, GL_BLEND, GL_ADD, or GL_COMBINE.
    //    GL_MODULATE :  Texel color values are multiplied by the geometry fragment color values.


	if(textureName==1)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	//This function defines a one-, two-, or three-dimensional texture image
	//parameters:
	//P1: GLenum: The texture target being specified
	//P2: GLint:The level of detail. Usually, 0 unless mipmapping"is used
	//P3: GLint:The internal format of the image data
	//P4: GLsizei: The width of the one-, two-, or three-dimensional texture image. This must be a power of 2 but may include a border.
	//P5: GLsizei: The height of the two- or three-dimensional texture image. This must be a power of 2 but may include a border.
	//P6: GLint: The width of the border. All implementations must support 0, 1, and 2 texel borders
	//P7: GLenum: The format of the pixel data.
	//P8: GLenum: The data type of each texel value
	//P9: GLvoid *: The pixel data.
	else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nRows, nCols, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

}

ushort getShort(){
	char ic;
	ushort ip;
	inf.get(ic);
	ip = ic;
	inf.get(ic);
	ip |= ((ushort)ic<<8);
	return ip;
}

ulong getLong()
{
	ulong ip = 0;
	char ic = 0;
	unsigned char uc = ic;
	inf.get(ic);
	uc = ic; ip = uc;
	inf.get(ic); uc = ic; ip |= ((ulong)uc<<8);
	inf.get(ic); uc = ic; ip |= ((ulong)uc<<16);
	inf.get(ic); uc = ic; ip |= ((ulong)uc<<24);
	return ip;
}

void RGBpixmap::readBMPFile( char *fname)
{
	inf.open(fname, ios::in | ios::binary); //read binary characters
	if(!inf)
	{
		cout<<"cannot open file!!!!"<<fname<<endl;
        return;
	}
	int k, row, col, numPadBytes, nBytesInRow;
	// keep the file header information
	char ch1, ch2;
	inf.get(ch1);
	inf.get(ch2); // type: always 'BM'
	ulong fileSize = getLong();
	ushort reserved1 = getShort(); //always 0
	ushort reserved2 = getShort(); //always 0
	ulong offBits = getLong(); //offset to image unreliable
	ulong headerSize = getLong(); //always 40
	ulong numCols = getLong(); //number of colums in image
	ulong numRows = getLong(); //number of rows in image
	ushort planes = getShort(); //always 1
	ushort bitsPerPixel = getShort(); //8 or 24; allow only 24 here
	ulong compression = getLong(); //must be 0 for uncompressed
	ulong imageSize = getLong(); //total byte in image
	ulong xPels = getLong(); //always 0
	ulong yPels = getLong(); //always 0
	ulong numLUTentries = getLong(); //256 for 8 bit, otherwise 0
	ulong impColors = getLong(); //always 0
	if(bitsPerPixel != 24)
	{
		//error must be a 24 bit uncompressed image
		cout<<"Not a 24 bit pixel image or is compressed"<<endl;
		inf.close();
		return ;
	}
	//add bytes at the end of each row so total # is a multiple of 4
	//round up 3 into numCols then 2 next mult. of 4
	nBytesInRow = ((3*numCols + 3)/4)*4;
	numPadBytes = nBytesInRow - 3 * numCols; //need this many
	nRows = numRows; //set classes data members
	nCols = numCols;
	pixel = new mRGB[nRows * nCols]; //make space for array
	if(!pixel)
		return ; //out of memory
	long count = 0;
	char dum;
	for(row = 0; row < nRows; row++) //read pixel values
	{
		for (col = 0; col < nCols; col++)
		{
			char r,g,b;
			inf.get(b); inf.get(g); inf.get(r); //read bytes
			pixel[count].r = r; //place them in colors
			pixel[count].g = g;
			pixel[count++].b = b;
		}
		for (k = 0; k < numPadBytes; k++) //skip padBytes at rows end
		{
			inf >> dum;
		}
	}
	inf.close();
	return ;
	//success
}
