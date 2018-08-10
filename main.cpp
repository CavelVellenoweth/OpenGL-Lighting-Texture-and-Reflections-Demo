//--------------------------------------------------------------------------------------------
// File:		main.cpp
// Version:		V2.1
// Author:		Daniel Rhodes
// Date:		16/12/2005
// Description:	OpenGL Framework
// Notes:		Has facilities to output floating point data to file from a pixel shader
//				21/12/2005 - Added preprocessor stuff to switch reading pixels,
//							 full screen quad, and the window border on / off.
//				06/01/2006 - Added facility to toggle readpixels on and off during execution,
//							 WARNING: using this can cause a massive slowdown dependant on
//									  window size
//--------------------------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN
#define VC_LEANMEAN

// RECOMMENDED USAGE:  when READPIX on: WINDOWBORDER off, FULLSCREENQUAD on
//					   prevents background pixels appearing in output.
//					   when READPIX off: WINDOWBORDER on, FULLSCREENQUAD off
#define READPIX			0	// Enables debug output from pixel shander to file
							// WARNING: will stop display on screen. 1 on, 0 off.
#define WINDOWBORDER	1	// When false (0) turns off window border so render surface can be
							// the exact size of WINDOWWIDTH x WINDOWHEIGHT. 1 for normal window
#define FULLSCREENQUAD	0	// Draw a full screen quad instead of the cube; aids debugging
							// with bReadPixels. 1 = on, 0 = off.
#define WINPOSX			100		// Window x coordinate
#define WINPOSY			100		// Window y coordinate
#define LWIDTH			512
#define LHEIGHT			512
#define TEXWIDTH		128
#define TEXHEIGHT		128
#define MAXTEX			4		// Maximum number of textures
#define iTexID0			0		// Texture ID's, for multitexturing in OpenGL
#define iTexID1			1
#define iTexID2			2
#define iTexID3			3

#define MAX_TEXTURES 200
#if FULLSCREENQUAD	// Need to use texture size as window size for fullscreen quad
	//#define WINDOWWIDTH		TEXWIDTH	// Window width
	//#define WINDOWHEIGHT	TEXHEIGHT	// Window height
	#define WINDOWWIDTH		LWIDTH	// Window width
	#define WINDOWHEIGHT	LHEIGHT	// Window height
#else
	#define WINDOWWIDTH		1024	// Window width
	#define WINDOWHEIGHT	768		// Window height
	//#define WINDOWWIDTH		512	// Window width
	//#define WINDOWHEIGHT	512		// Window height
#endif

//-----------------------------------------------------------------------------
// Key definitions
// Desc:	Allow NON CASE SENSITIVE key controls
//-----------------------------------------------------------------------------
#define	KEYDOWN	0x8000

#define	VK_SHIFT	0x10	// SHIFT key  
#define	VK_CONTROL	0x11	// CTRL key  
#define	VK_MENU		0x12	// ALT key 
#define	VK_LEFT		0x25	// LEFT ARROW key  
#define	VK_UP		0x26	// UP ARROW key  
#define	VK_RIGHT	0x27	// RIGHT ARROW key  
#define	VK_DOWN		0x28	// DOWN ARROW key  
#define VK_ESCAPE	0x1B	// ESC key 
#define	VK_0		0x30
#define	VK_1		0x31
#define	VK_2		0x32
#define	VK_3		0x33
#define	VK_4		0x34
#define	VK_5		0x35
#define	VK_6		0x36
#define	VK_7		0x37
#define	VK_8		0x38
#define	VK_9		0x39
#define	VK_A		0x41
#define	VK_B		0x42
#define	VK_C		0x43
#define	VK_D		0x44
#define	VK_E		0x45
#define	VK_F		0x46
#define	VK_G		0x47
#define	VK_H		0x48
#define	VK_I		0x49
#define	VK_J		0x4A
#define	VK_K		0x4B
#define	VK_L		0x4C
#define	VK_M		0x4D
#define	VK_N		0x4E
#define	VK_O		0x4F
#define	VK_P		0x50
#define	VK_Q		0x51
#define	VK_R		0x52
#define	VK_S		0x53
#define	VK_T		0x54
#define	VK_U		0x55
#define	VK_V		0x56
#define	VK_W		0x57
#define	VK_X		0x58
#define	VK_Y		0x59
#define	VK_Z		0x5A
#define	VK_NUMPAD0  0x60	// Numeric keypad 0 key  
#define	VK_NUMPAD1  0x61	// Numeric keypad 1 key  
#define	VK_NUMPAD2  0x62	// Numeric keypad 2 key  
#define	VK_NUMPAD3  0x63	// Numeric keypad 3 key  
#define	VK_NUMPAD4  0x64	// Numeric keypad 4 key  
#define	VK_NUMPAD5  0x65	// Numeric keypad 5 key  
#define	VK_NUMPAD6  0x66	// Numeric keypad 6 key  
#define	VK_NUMPAD7  0x67	// Numeric keypad 7 key  
#define	VK_NUMPAD8  0x68	// Numeric keypad 8 key  
#define	VK_NUMPAD9  0x69	// Numeric keypad 9 key  
#define	VK_SUBTRACT	0x6D	// Minus key (numpad)
#define	VK_ADD		0x6B	// Plus key (numpad)
#define	VK_OEM_4	0xDB	// [{ key
#define	VK_OEM_6	0xDD	// ]} key
#define	VK_OEM_COMMA 0xBC	// , key
#define	VK_OEM_PERIOD 0xBE	// . key
#define	VK_OEM_2	0xBF	// /? key

// System header files
#include <fstream>
#include <string>
#include <stdio.h>

// Windows header files
#include <windows.h>

// OpenGL header files
#include <gl/gl.h>
#include <gl/glu.h>

#include <gl/glext.h>
#include <gl/wglext.h>
#include <math.h>
#include "trans.h"

// Custom header files
// Insert your header files here

// Namespaces
// DON'T use 'using std;' it confuses other namespaces we may need
using std::ifstream;
using std::ofstream;
using std::endl;
using std::string;

// GL functions

PFNGLGETINFOLOGARBPROC				glGetInfoLogARB					= NULL;

// OGL3.2 versions
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLDETACHSHADERPROC glDetachShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLUNIFORM1IVPROC glUniform1iv = NULL;
PFNGLUNIFORM2IVPROC glUniform2iv = NULL;
PFNGLUNIFORM3IVPROC glUniform3iv = NULL;
PFNGLUNIFORM4IVPROC glUniform4iv = NULL;
PFNGLUNIFORM1FPROC glUniform1f = NULL;
PFNGLUNIFORM1FVPROC glUniform1fv = NULL;
PFNGLUNIFORM2FVPROC glUniform2fv = NULL;
PFNGLUNIFORM3FVPROC glUniform3fv = NULL;
PFNGLUNIFORM3FPROC glUniform3f = NULL;
PFNGLUNIFORM4FVPROC glUniform4fv = NULL;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = NULL;
PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f = NULL;
PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv = NULL;
PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv = NULL;
PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv = NULL;
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = NULL;
PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;


// Shader
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;

// VBO
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLBINDBUFFERPROC	glBindBuffer = NULL;
PFNGLBINDVERTEXARRAYPROC	glBindVertexArray = NULL;

PFNGLMAPBUFFERPROC	glMapBuffer = NULL;
PFNGLUNMAPBUFFERPROC	glUnmapBuffer = NULL;
PFNGLMAPBUFFERRANGEPROC	glMapBufferRange = NULL;

PFNGLUNIFORMBLOCKBINDINGPROC	glUniformBlockBinding = NULL;
PFNGLBUFFERDATAPROC	glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC	glBufferSubData = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
//FBO
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = NULL;
//extras

PFNGLPROGRAMPARAMETERIPROC glProgramParameteriEXT =NULL;

//ENd of OGL 3.2 versions
// Global device context.
HDC g_HDC;
//global shade for uniform demo
float myshade=0.5;
int my_aniso_on=0;
float filterwidth=0.5;
int adjust_vert=0;
float myuvshadearray[6]={1.0,1.0,1.0,1.0,1.0,1.0};
// xRotation and yRotation is used to rotate the entire scene.
//float xRotation = 45.0f;
float nearplane=1.0;
float vang=0;
float ldist=250;
float angleofview=3.14159/8.0;
float xRotation = 0.0f;
float yRotation = 0.0f;
float zRotation = 0.0f;
float fMoveX = 0.0;
float fMoveY = 0.0;
#if FULLSCREENQUAD // only move back 5 for non fullscreen quads
	float fMoveZ = 0.0;
#else
	float fMoveZ = -1000.0;	// Put in a big number for our poly modelsDoesn't have to be five but can't be zero for non fullscreen
#endif

//Window stuff - seems like it needs to be global now
		static HGLRC hRC,hRCtmp;                   // Rendering context
	static HDC hDC;                     // Device context
bool bReadPixOn = false;

// Stores global width and height for our calculations.
int gWidth	= 0;
int gHeight = 0;

// Shader variables we will bind to.
UINT glslTexture;
UINT myshade_index;
UINT mytexture_index;
UINT mytexturesize_index;
UINT myspecmap_index;
UINT mynormalmap_index;
UINT myvshade_index;
UINT myambient_index;
UINT mydirect_index;
UINT mylightpos_index;
UINT myhasspec_index;
UINT myhasnormal_index;
VECTOR lightpos{ 0,1,1 };
int selected;
int mat3rotation_projectionindex;
int vec3objcentre_to_eye_projectedindex;
int vec3centreindex;
int vec3light_in_object_coordsindex;
int vec3view_in_object_coordsindex;
int pscaleindex;
int myvmin_index;
int vmin=0;
// Map buffer addresses
float *colourpointer;
VECTOR centre;
// GLSlang objects.
GLhandleARB glContext0;
GLhandleARB glContext1;

GLhandleARB glVertexShader0;
GLhandleARB glVertexShader1;
GLhandleARB glGeomShader0;
GLhandleARB glPixelShader0;
GLhandleARB glPixelShader1;


//bool fourier = true;
int iShader = 0;

// Textures we will use
float *fTexture;			// Input texture
float *fTextureOut;			// Output texture
GLuint iTextures[MAXTEX];	// Array of texture ID's (makes it simpler to swap textures)
// Framebuffer ID
GLuint iFrameBuffer;
//Scene object types
typedef struct colour
{
	float r, g, b;
} COLOUR;

colour direct = { 1,1,1 };
colour ambient = { 0.1,0.1,0.1 };

//-----------------------------------------------------------------------------
// Name: texCoord & texMap
// Desc: Some handy definitions for texture mapping !
//-----------------------------------------------------------------------------
typedef struct texCoord
{
   float u, v;	// x & y, called u & v under texture mapping conventions
} TEXCOORD;
typedef struct texture_mip_map
{
   unsigned char **texMap;
   int sx,sy,depth,comp;
}TEXTURE_MIP_MAP;

typedef struct polygon
{
	// Basic information for 2D/3D Part A/B
	int nv;           // Number of vertices
	VECTOR vert[20];  // Array of vertices - position vectors
	COLOUR colour;       // Colour, struct of 3 BYTE's R, G, B

	// Extra information for back face removal, part B
	VECTOR normal;    //Surface Normal Vector

	// Extra information for shading and texturing, part C
	int vertVecNo[20];		// holds vertex vector number
	int texindex; // pointer to texture map in global texture array
	int specindex;
	int normalindex;
	TEXCOORD texPos[20];	//Position of corresponding numbered vertex in texture map
	TEXCOORD texsize;
	//info for opengl 3.2 version only
	int vstart,length;//first and size for drawarrays
	int index;//position of this polygon in polylist
} POLYGON;
TEXTURE_MIP_MAP global_textures[MAX_TEXTURES]; //global array of texture
int used_textures=0;// of which this number are actually used


//Scene object data
int m_iNumOfPolys=0;
char m_sFilename[200];
VECTOR m_vLight;
POLYGON polylist[2000];  // Array to store a list of polygons.
float **polyarrays;//all the data to be loaded into the buffers
int polyarraysizes=0;
// My Variables
typedef	struct {
	int i;
	int j;
	float redR;
	float redI;
	float greenR;
	float greenI;
	float blueR;
	float blueI;
	float mag;
} FREQPOINT;

float *fLShape;

// Function Prototypes
bool bReadTexture( );
bool bSetupExtensions( );
bool bCheckFramebufferStatus( );
char *sLoadShader( char *sShaderName );
void SetupPixelFormat( HDC hDC );
void bReadPixels( );
bool bInitialiseGLTextures( );
bool bInitialiseFBO( );
bool bInitialiseGLSL( );
bool bInitialiseGL( );
void RenderScene( );
void Shutdown( );
void ReadFile(char *filename);
int set_vertex_arrays(float **arrays,int n_polys, POLYGON *plist);
void DrawPolygon(POLYGON *p);

// My Functions
//-----------------------------------------------------------------------------
// Name:	mip_map_texture_file_read
// Parms:	 texture input file, string containing sizes as read from polygon file current texture count
// Returns:	new texture count
// Desc:	read a mip mapped texture from a raw file.
//-----------------------------------------------------------------------------
int mip_map_texture_file_read(FILE *texfile,char *instring,int max_tex_n)
{
		int i,j,depth;
		double size;

	sscanf(instring,"%d%d",&(global_textures[max_tex_n].sx),&(global_textures[max_tex_n].sy));
	
	if (global_textures[max_tex_n].sy>=global_textures[max_tex_n].sx)	size=global_textures[max_tex_n].sx;
	else	size=global_textures[max_tex_n].sy;
	size=frexp(size,&depth);
	global_textures[max_tex_n].depth=depth;
	global_textures[max_tex_n].comp=3;
	//allocate memory for  texture table
	global_textures[max_tex_n].texMap=(unsigned char **) malloc((global_textures[max_tex_n].depth)*sizeof(unsigned char *));
	for (i=0;i<depth;i++)
	{
		global_textures[max_tex_n].texMap[i]=(unsigned char *)malloc(sizeof(unsigned char)*3*(global_textures[max_tex_n].sx>>i)*(global_textures[max_tex_n].sy>>i));
		for(j=0;j<(global_textures[max_tex_n].sy>>i);j++)
			fread(global_textures[max_tex_n].texMap[i]+j*3*(global_textures[max_tex_n].sx>>i),sizeof(unsigned char),3*(global_textures[max_tex_n].sy>>i),texfile);
		//read file 
	}
	max_tex_n++;
	fclose(texfile);
	return max_tex_n;

}

//-----------------------------------------------------------------------------
// Name:	GLGETERROR
// Parms:	sLoc = Location of error
// Returns:	true if no error - else false
// Desc:	Displays a message box when an error occurs giving the error type
//			and location
//-----------------------------------------------------------------------------
bool GLGETERROR(char *sLoc )														
{																			
	GLenum error = glGetError();											
	string sError = "";														
	string sLocation = "OpenGL error: ";									
	sLocation.append( sLoc );												
	char buffer[100] = "";													
	if( error != GL_NO_ERROR )												
	{																		
		sprintf( buffer, "[%s line %d] GL Error: %s\n",						
				__FILE__, __LINE__, gluErrorString( error ) );				
		sError.append( buffer );											
		MessageBox( NULL, sError.c_str( ), sLocation.c_str( ), MB_OK );	
		return false;
	}				
	return true;
}
void GLCLEARERROR( )	//cal get error but don't interrupt processing - clears errors													
{																			
	GLenum error = glGetError();											
	return;														
}



//-----------------------------------------------------------------------------
// Name:	bSetupExtensions
// Parms:	None
// Returns:	bool - true for success, false for failure
// Desc:	Load all require OpenGL extensions
//-----------------------------------------------------------------------------
bool bSetupExtensions( )
{
	

	// OGL3.2 version
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
	glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
	glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
	glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)wglGetProcAddress("glVertexAttrib1f");
	glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)wglGetProcAddress("glVertexAttrib1fv");
	glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)wglGetProcAddress("glVertexAttrib2fv");
	glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)wglGetProcAddress("glVertexAttrib3fv");
	glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)wglGetProcAddress("glVertexAttrib4fv");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glActiveTexture		=(PFNGLACTIVETEXTUREPROC) wglGetProcAddress("glActiveTexture");

	// Shader
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");

	// VBO
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glMapBuffer = (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");
	glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
	glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)wglGetProcAddress("glMapBufferRange");

	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
	glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)wglGetProcAddress("glUniformBlockBinding");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
//FBO
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");

//end of OGL3.2 version
	//some more extras
	glProgramParameteriEXT=(PFNGLPROGRAMPARAMETERIPROC)wglGetProcAddress("glProgramParameteriEXT");

	return true;
}


//-----------------------------------------------------------------------------
// Name:	loadShader
// Parms:	shaderName = Shader name
// Returns:	Shader code
// Desc:	Render to Screen
//-----------------------------------------------------------------------------
char* sLoadShader( char *sShaderName )
{
	char tempLine[256] = {0};
	int count = 0;

	// input object.
	ifstream tempInput;

	// Open the shader file
	tempInput.open( sShaderName );

	// If there are errors then return false
	if( !tempInput.is_open( ) )
		return NULL;

	// Loop through each line of the file and get the total size
	while( !tempInput.eof( ) )
	{
		tempInput.getline( tempLine, 256, '\n' );
		count++;
	}

	// Close the shader
	tempInput.close( );

	if( count == 0 )
		return NULL;

	// Read in the data for use this time
	ifstream input;

	// Create array to hold shader code
	char *ShaderSource = new char[256 * count];

	// Re-open the shader and read in the whole thing into the array
	input.open( sShaderName );
	input.getline( ShaderSource , 256 * count, '\0' );

	input.close( ); // Close the shader

	return ShaderSource;
}

//-----------------------------------------------------------------------------
// Name:	SetupPixelFormat
// Parms:	hDC = handle to device context
// Returns:	None
// Desc:	Sets the pixel format
//-----------------------------------------------------------------------------
void SetupPixelFormat( HDC hDC )
{
	int nPixelFormat;

	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 32; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	// Chooses the best pixel format and returns index
	//GLGETERROR("Pre Pixelformat");
	nPixelFormat = ChoosePixelFormat( hDC, &pfd );
	//GLGETERROR("In Pixelformat");

	// Set pixel format to device context
	if ( !SetPixelFormat( hDC, nPixelFormat, &pfd ) )
		MessageBox( NULL, "Pixel Format not supported", "Error! ( bCheckFramebufferStatus )", MB_OK );
//	GLGETERROR("In Pixelformat2");
		
}

//-----------------------------------------------------------------------------
// Name:	bInitialiseGLTextures
// Parms:	None
// Returns:	bool - true for success, false for failure
// Desc:	Sets up textures
//---------------------------------

bool bInitialiseGLTextures( )
{
	unsigned int texhandles[MAX_TEXTURES];
	glGenTextures(used_textures,texhandles);
	for (int i=0;i<used_textures;i++)
	{
//			glEnable( GL_TEXTURE_2D );
			GLGETERROR("Texture set up a");
			glActiveTexture(GL_TEXTURE0+i);
			
		glBindTexture(GL_TEXTURE_2D,texhandles[i]);//bind texture - assume all are 2D
		GLGETERROR("Texture set up aa");
		for (int d=0;d<global_textures[i].depth;d++)
		{
			switch(global_textures[i].comp)
			{
			case 1:
				glTexImage2D( GL_TEXTURE_2D,  d, GL_R32F, global_textures[i].sx>>d, global_textures[i].sy>>d, 0, GL_R, GL_UNSIGNED_BYTE, global_textures[i].texMap[d] );
				break;
			case 2:
				glTexImage2D( GL_TEXTURE_2D,  d, GL_RG32F, global_textures[i].sx>>d, global_textures[i].sy>>d, 0, GL_RG, GL_UNSIGNED_BYTE, global_textures[i].texMap[d] );
				break;
			case 3:
				glTexImage2D( GL_TEXTURE_2D,  d, GL_RGB32F, global_textures[i].sx>>d, global_textures[i].sy>>d, 0, GL_RGB, GL_UNSIGNED_BYTE, global_textures[i].texMap[d] );
				break;
			case 4:
				glTexImage2D( GL_TEXTURE_2D,  d/*mip0 map level*/, GL_RGBA32F/*internal format used for the texture. Use this version to guarantee a floating point texture*/, global_textures[i].sx>>d/*image width*/, global_textures[i].sy>>d/*image height*/, 0, GL_RGBA/*format of the image data  being loaded*/, GL_UNSIGNED_BYTE/* number format for the image data being loaded*/, global_textures[i].texMap[d]/*pointer to the start of the texture data*/ );
				break;
			}
		}
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );//force the crudest way of doing it for demo purposes
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		//glGenerateMipmap(GL_TEXTURE_2D); routine seems to be missing from headers - probably not good anyway - replace with a better one from own library


	}
	return GLGETERROR("Texture set up");

}

//-----------------------------------------------------------------------------
// Name:	InitialiseGLSL
// Parms:	None
// Returns:	bool - true for success, false for failure
// Desc:	Initialises GLSL
//-----------------------------------------------------------------------------	
float *uniformpointer;
GLuint uniformbuffer,vertexbuffer,colourbuffer,normalbuffer,tcoordbuffer;

bool bInitialiseGLSL( )
{
	char *extensions = (char*)glGetString( GL_SHADING_LANGUAGE_VERSION );
	float lanver;
	sscanf(extensions,"%f ",&lanver);// Check glsl version
	if (lanver<1.5)
		return false;


	char error[4096];
	int result_v=0,result_s=0,result_l=0,result_g=0;

	char *ShaderCode = "";// a Pointer to the text of the shader program

	int vertexindex=97,colourindex=432,normalindex,tcoordindex;


	// Enable shaders
	switch ( iShader )//can add other shaders with different filenames using different values of iShader
	{
	case 0:
		// Create the program object - will contain vertex and pixel/fragment shaders
		glContext0 = glCreateProgram();
		//Program and shader objects are labelled by integers - starting at 1
		glProgramParameteriEXT(glContext0, GL_GEOMETRY_VERTICES_OUT_EXT, 160); 
		// Load the vertex shader source code
		ShaderCode = sLoadShader( "glsl.vs" );
		//shadercode will now be a text string containing the whole vertex shader program!

		//Now create a vertex shader object
		glVertexShader0 = glCreateShader( GL_VERTEX_SHADER );


		//and put our source code into it from the char string ShaderCode
		glShaderSource( glVertexShader0, 1, (const char**)&ShaderCode, NULL );

		//Compile - note that this occurs whilst the main program is running - 
		// and can happen any time you want by just running this function
		glCompileShader( glVertexShader0 );

		//Get the flag that tells us whether it failed or not, and store in result_v 

		glGetShaderiv( glVertexShader0, GL_COMPILE_STATUS, &result_v );
		//make sure compile successful
		if(!(result_v ))
		{
			//if not OK retrieve error string
			glGetShaderInfoLog( glVertexShader0, sizeof(error), NULL, error );
			//and output it
			MessageBox( NULL, error, "Error Compiling Vertex Shader...", MB_OK );
			//then abort the set up
			return false;
		}
		//Load vertex shader executable code into our program object
		glAttachShader( glContext0, glVertexShader0 );
		//Don't need the shader source code anymore (it's compiled), 
		// so throw away the contents of the char string whic had been acquired when you loaded the shader
		delete[] ShaderCode;
		ShaderCode = NULL;
		//Now the geometry shader
			ShaderCode = sLoadShader( "geomdemo.gs" );

		glGeomShader0 = glCreateShader( GL_GEOMETRY_SHADER );
		glShaderSource( glGeomShader0, 1, (const char**)&ShaderCode, NULL );
		glCompileShader( glGeomShader0 );
		glGetShaderiv( glGeomShader0, GL_COMPILE_STATUS, &result_g );
		//make sure compile successful
		if(!(result_g ))
		{
			//if not OK retrieve error string
			glGetShaderInfoLog( glGeomShader0, sizeof(error), NULL, error );
			//and output it
			MessageBox( NULL, error, "Error Compiling Geometry Shader...", MB_OK );
			FILE *gerr=fopen("geom_err.txt","w");
			fwrite(error,sizeof(char),4096,gerr);
			fclose (gerr);

			//then abort the set up
			return false;
		}
		glAttachShader( glContext0, glGeomShader0 );

		delete[] ShaderCode;
		ShaderCode = NULL;

		// Load the pixel shader - repeat all the above steps
		// (You can reuse the ShaderCode variable)
		ShaderCode = sLoadShader( "glsuniformdemol.ps" );

		glPixelShader0 = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource( glPixelShader0, 1, (const char**)&ShaderCode, NULL );
		glCompileShader( glPixelShader0 );
		glGetShaderiv( glPixelShader0, GL_COMPILE_STATUS, &result_s );
		//make sure compile successful
		if(!(result_s ))
		{
			//if not OK retrieve error string
			glGetShaderInfoLog( glPixelShader0, sizeof(error), NULL, error );
			//and output it
			MessageBox( NULL, error, "Error Compiling Fragment Shader...", MB_OK );
			//then abort the set up
			return false;
		}
		glAttachShader( glContext0, glPixelShader0 );

		// empty the string again when we've finished with it - don't want to have memory leaks!
		delete[] ShaderCode;
		ShaderCode = NULL;

		// Link shaders
		glLinkProgram( glContext0 );
		glGetProgramiv( glContext0, GL_LINK_STATUS, &result_l );

		//make sure link was successful
		if(!( result_l))
		{
			//if not OK retrieve error string
			glGetProgramInfoLog( glContext0, sizeof(error), NULL, error );
			//and output it
			MessageBox( NULL, error, "Error linking shaders...", MB_OK );
			//then abort the set up
			return false;
		}
		
	// Find the index values for shader variables
		myvmin_index=glGetUniformLocation( glContext0, "vmin" );
		myshade_index=glGetUniformLocation( glContext0, "shade" );
		mytexture_index=glGetUniformLocation( glContext0, "mytexture" );
		myspecmap_index = glGetUniformLocation(glContext0, "myspecmap");
		mynormalmap_index = glGetUniformLocation(glContext0, "mynormalmap");
		myambient_index=glGetUniformLocation(glContext0, "ambient");
		mydirect_index = glGetUniformLocation(glContext0, "direct");
		mylightpos_index = glGetUniformLocation(glContext0, "lightpos");
		myhasspec_index = glGetUniformLocation(glContext0, "hasspec");
		myhasnormal_index = glGetUniformLocation(glContext0, "hasnormal");
//		mytexturesize_index=glGetUniformLocation( glContext0, "TexMapSize" );
		mat3rotation_projectionindex=glGetUniformLocation( glContext0, "rotation_projection" );
		vec3objcentre_to_eye_projectedindex=glGetUniformLocation( glContext0, "objcentre_to_eye_projected" );
		vec3centreindex=glGetUniformLocation( glContext0, "centre" );
		vec3light_in_object_coordsindex=glGetUniformLocation( glContext0, "light_in_object_coords" );
		vec3view_in_object_coordsindex=glGetUniformLocation( glContext0, "view_in_object_coords" );
		
		pscaleindex=glGetUniformLocation( glContext0, "pscale" );
					GLGETERROR( "PreGenBuffers" );

        GLuint vertexarray;
		glGenVertexArrays(GLsizei(1),&vertexarray);
			GLGETERROR( "GenVertexArray" );

		glBindVertexArray(vertexarray );
			GLGETERROR( "BindVertexArray" );
		glGenBuffers(1,&vertexbuffer);//allocate name
			GLGETERROR( "GenBuffers" );
		glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);//bind as an array buffer
		colourindex=glGetAttribLocation( glContext0,"vshade");
		normalindex=glGetAttribLocation( glContext0,"normal");
		tcoordindex=glGetAttribLocation( glContext0,"tcoord");
			GLGETERROR( "glGetAttribLocation-c" );
		vertexindex=glGetAttribLocation( glContext0,"pos");
			GLGETERROR( "glGetAttribLocation-v" );
		glVertexAttribPointer(vertexindex, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glBufferData(GL_ARRAY_BUFFER,3*polyarraysizes*sizeof(GLfloat),polyarrays[0],GL_STATIC_DRAW);
			GLGETERROR( "bufferdata1" );
		glEnableVertexAttribArray(vertexindex);
		glGenBuffers(1,&colourbuffer);//allocate name
			GLGETERROR( "GenBuffers" );
		glBindBuffer(GL_ARRAY_BUFFER,colourbuffer);//bind as an array buffer
		glVertexAttribPointer(colourindex, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glBufferData(GL_ARRAY_BUFFER,3*polyarraysizes*sizeof(GLfloat),polyarrays[1],GL_STATIC_DRAW);
		glEnableVertexAttribArray(colourindex);
			GLGETERROR( "bufferdata2" );
				glGenBuffers(1,&normalbuffer);//allocate name
			GLGETERROR( "GenBuffers" );
		glBindBuffer(GL_ARRAY_BUFFER,normalbuffer);//bind as an array buffer
			GLGETERROR( "bufferdata3" );
		glVertexAttribPointer(normalindex, 3, GL_FLOAT, GL_FALSE, 0, 0); 
			GLGETERROR( "bufferdata4" );
		glBufferData(GL_ARRAY_BUFFER,3*polyarraysizes*sizeof(GLfloat),polyarrays[2],GL_STATIC_DRAW);
			GLGETERROR( "bufferdata5" );
		glEnableVertexAttribArray(normalindex);
			GLGETERROR( "bufferdata6" );
		glGenBuffers(1,&tcoordbuffer);//allocate name
			GLGETERROR( "GenBuffers" );
		glBindBuffer(GL_ARRAY_BUFFER,tcoordbuffer);//bind as an array buffer
			GLGETERROR( "bufferdata3" );
		glVertexAttribPointer(tcoordindex, 2, GL_FLOAT, GL_FALSE, 0, 0); 
			GLGETERROR( "bufferdata4" );
		glBufferData(GL_ARRAY_BUFFER,2*polyarraysizes*sizeof(GLfloat),polyarrays[3],GL_STATIC_DRAW);
			GLGETERROR( "bufferdata5" );
		glEnableVertexAttribArray(tcoordindex);
			GLGETERROR( "bufferdata6" );
		glBindBuffer(GL_ARRAY_BUFFER,colourbuffer);//rebind so we can change it later


			GLGETERROR( "bufferdata7" );
			break;

	
	}

	//--------------------------------------------------------------

	

	GLGETERROR( "bInitialiseGLSL" );

	return true;
}

//-----------------------------------------------------------------------------
// Name:	InitialiseGL
// Parms:	None
// Returns:	bool - true for success, false for failure
// Desc:	Initialises OpenGL and GLSL
//-----------------------------------------------------------------------------
bool bInitialiseGL( )
{
	GLCLEARERROR();
	glClearColor( 0.6f, 0.6f, 0.8f, 0.0f );					// Clear the screen to light blue
	GLGETERROR("Ex_texture");
	glEnable( GL_DEPTH_TEST );								// Enable desth testing for hidden surface removal
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	GLGETERROR("Ex_texture");
	if ( !bSetupExtensions( ) )
	{
		MessageBox( NULL, "Error loading Extensoions.", "Error! ( InitialiseGL )", MB_OK );
		return false;
	}


	if ( !bInitialiseGLSL( ) )
	{
		MessageBox( NULL, "Error loading GLSlang functions.", "Error! ( InitialiseGL )", MB_OK );
		return false;
	}



	GLGETERROR( "bInitialiseGL" );

	glClearDepth( 1.0f );										// Depth Buffer Setup
		GLGETERROR( "bInitialiseGL_end0" );

	glDepthFunc( GL_LEQUAL );	
		GLGETERROR( "bInitialiseGL_end1" );
// The Type Of Depth Testing To Do

	return true;
}

//-----------------------------------------------------------------------------
// Name:	RenderScene
// Parms:	None
// Returns:	None
// Desc:	Render Scene: main part of the rendering loop
//-----------------------------------------------------------------------------
void RenderScene( )
{
	static float pscale = -2.0;
	const float aspect=float(WINDOWHEIGHT)/float(WINDOWWIDTH);
	float initarray[14];
	VECTOR light={ldist*sin(vang),0, ldist* cos(vang)};//light centred on object
	pscale =2.0*nearplane;
	    GLGETERROR( "PreRenderScene" );
		wglMakeCurrent(hDC, hRC);
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		glUseProgram( glContext0 );
		VECTOR disp={fMoveX,fMoveY,fMoveZ};
		TRANSFORM t=BuildTrans(-yRotation*3.14159/180.0,-xRotation*3.14159/180.0,0,disp);
		VECTOR lightobj=MOnV(InverseRotationOnly(t),VectorDiff(light,disp));
		VECTOR viewobj=MOnV(InverseRotationOnly(t),disp);
		double cotang=1.0/tan(angleofview);
		MATRIX viewangles={{cotang*aspect,0,0},{0,cotang,0},{0,0,-1.0}};
		t.rotate=Product(viewangles,t.rotate);
		t.shift=MOnV(viewangles,t.shift);
		glUniformMatrix3fv(mat3rotation_projectionindex,1,GL_FALSE,&(t.rotate.cx.x));
		glUniform3fv(vec3objcentre_to_eye_projectedindex,1,&(t.shift.x));
		glUniform3fv(vec3centreindex,1,&(centre.x));
		glUniform3fv(vec3light_in_object_coordsindex,1,&(lightobj.x));
		glUniform3fv(vec3view_in_object_coordsindex,1,&(viewobj.x));
		glUniform3fv(myambient_index, 1, &(ambient.r));
		glUniform3fv(mydirect_index, 1, &(direct.r));
		glUniform3fv(mylightpos_index, 1, &(lightpos.x));
		glUniform1f( pscaleindex, pscale);
		glUniform1i( myvmin_index, vmin);

	//--------------------------------
	    GLGETERROR( "RenderScene1" );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );   
	GLGETERROR( "RenderScene2" );

	for (int i=0;i<m_iNumOfPolys;i++)
	{
		DrawPolygon(&polylist[i]);

		//test evaluation of vertex shader code for debugging
		VECTOR v,h,l,n;
		v=Normalise(viewobj);
		l=Normalise(lightobj);
		h=VectorDiff(v,l);
		h=Normalise(h);
		double s=Dot(h,polylist[i].normal);
		s*=s;

	}
	//--------------------------------
	glFlush (); 
	    GLGETERROR( "RenderScene4" );
	SwapBuffers(hDC);
		    GLGETERROR( "RenderScene5" );
}

//-----------------------------------------------------------------------------
// Name:	WndProc
// Parms:	hwnd		= handle to window context
//			message		= message to process
//			wParam		= message params
//			lParam		= message params
// Returns:	Unhandled messages, 0 if all messages handled
// Desc:	Message handling function
//-----------------------------------------------------------------------------

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int	width, height;                  // The window width and height
	static POINT oldMousePos;           // Last mouse position
	static POINT currentMousePos;       // Current mouse position
	static bool isMouseActive;          // Is the left mouse button down
	char* ver;
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

	int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 2, 
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,// | WGL_CONTEXT_DEBUG_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, //0x9126 , 0x00000001,
		0
    };
	//local veriables for inside of switch
	float *polyshades;
	POLYGON *p;

	switch( message )
	{
	case WM_CREATE:						// Window creation
		hDC		= GetDC( hwnd );		// Get the device context for our window
		g_HDC	= hDC;					// Assign the global device context to this one
		//GLGETERROR("b4Pixelformat");

		SetupPixelFormat( hDC );		// Call the pixel format function
		//GLGETERROR("Pixelformat");
		hRCtmp = wglCreateContext( hDC );	// Create a temporary rendering context
		wglMakeCurrent( hDC, hRCtmp );		// Make the tmp rendering context ( hDC = Device context, hRC = OpenGl rendering context )

		 ver = (char*)glGetString(GL_VERSION); // ver = "3.2.0"
		//GLGETERROR("Pixelformat2");
		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
		if(wglCreateContextAttribsARB != NULL)
		{
			hRC = wglCreateContextAttribsARB(hDC,0, attribs);
		}
		//GLGETERROR("Pixelformat3");
		
		wglMakeCurrent(NULL,NULL); 
		wglDeleteContext(hRCtmp);
		wglMakeCurrent(hDC, hRC);
//GLGETERROR("Pixelformat4");
		



		return 0;
		break;
	case WM_CLOSE:						// Close message
	case WM_DESTROY:
		wglMakeCurrent( hDC, NULL );
		wglDeleteContext( hRC );		// Delete the rendering context
		
		PostQuitMessage( 0 );				// Close the program
		return 0;
		break;
	case WM_SIZE:						// re-size message
		height = HIWORD( lParam );		// Get the height of the window
		width = LOWORD( lParam );		// Get the width of the window

		if( height == 0 )				// Don't want a height of 0. If it is 0 make it 1
			height = 1;

		gHeight = height;
		gWidth  = width;

		glViewport( 0, 0, width, height );	// resets the viewport to new dimensions
		glMatrixMode( GL_PROJECTION );		// Set the projection matrix
		glLoadIdentity( );					// Reset the modelview matrix

		// calculate the aspect ratio of the window.
		gluPerspective( 45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f );

		glMatrixMode( GL_MODELVIEW );   // Set the projection matrix
		glLoadIdentity( );               // Reset the modelview matrix

		return 0;
		break;
	case WM_KEYDOWN:
		switch( wParam ) 
		{

		case VK_ESCAPE:
			PostQuitMessage( 0 );
			break;

		case VK_A:
			myshade-=0.01;
			myuvshadearray[adjust_vert]-=0.01;
			if (myuvshadearray[adjust_vert]<0)
				myuvshadearray[adjust_vert]=0;

			filterwidth*=0.9;
		break;
		case VK_C:
			//delete the existing shaders 
			glDeleteShader(glPixelShader0);
			glDeleteShader(glGeomShader0);
			glDeleteShader(glVertexShader0);
			glDeleteProgram(glContext0);
			//and recompile
			bInitialiseGLSL();
			break;
		case VK_D:
			//glBindbuffer not needed as colourbuffer already bound 
			//however you would in general need the following line
			//	glBindBuffer(GL_ARRAY_BUFFER,colourbuffer);
			//reset previous poly
			p=&polylist[adjust_vert];
			polyshades=new float[p->length*3];
			//get hold of old array data from polygon
			for (int i=0;i<p->length*3;i+=3)
			{
					polyshades[i]=p->colour.r;
					polyshades[i+1]=p->colour.g;
					polyshades[i+2]=p->colour.b;
			}
			glBufferSubData(GL_ARRAY_BUFFER,p->vstart*3*sizeof(GLfloat),p->length*3*sizeof(GLfloat),polyshades);

					//now change to new adjust_vert
			adjust_vert++;
			adjust_vert%=6;
			delete polyshades;
			p=&polylist[adjust_vert];
			polyshades=new float[p->length*3];//(float *)glMapBufferRange(GL_ARRAY_BUFFER,p->vstart*3, p->length*3,GL_READ_WRITE);
			GLGETERROR( "Drawpoly" );
				for (int i=0;i<p->length*3;i++)
				{
					polyshades[i]=1.0;//set the currently adjustable face to all white
				}
				
				glBufferSubData(GL_ARRAY_BUFFER,p->vstart*3*sizeof(GLfloat),p->length*3*sizeof(GLfloat),polyshades);
			

			delete polyshades;

		case VK_S:
			myshade+=0.01;
			myuvshadearray[adjust_vert]+=0.01;
			if (myuvshadearray[adjust_vert]>1.0)
				myuvshadearray[adjust_vert]=1.0;

			filterwidth*=1.1;
			break;
		case VK_F:
			vmin--;
			if (vmin<0) vmin=0;
			break;
		case VK_G:
			vmin++;
			break;
		case VK_LEFT:
			fMoveX++;
			break;
		case VK_RIGHT:
			fMoveX--;
			break;
		case VK_UP:
			fMoveY++;
			break;
		case VK_DOWN:
			fMoveY--;
			break;
		case VK_OEM_COMMA:
			nearplane--;
			break;
		case VK_OEM_PERIOD:
			nearplane++;
			break;
		case VK_ADD:
			fMoveZ+=1;
			break;
		case VK_SUBTRACT:
			fMoveZ -=1;
			break;
		case VK_1:
			vang=3.14159/4;
			break;
		case VK_2:
			vang=3.14159/2;
			break;
		case VK_3:
			vang=3*3.14159/4;
			break;
		case VK_4:
			vang=3.14159;
			break;
		case VK_0:
				vang=0;
				break;
		case VK_Q:
			switch (selected)
			{
			case 0:
				ambient.r += 0.1;
				ambient.g += 0.1;
				ambient.b += 0.1;
				break;
			case 1:
				ambient.r += 0.1;
				break;
			case 2:
				ambient.g += 0.1;
				break;
			case 3:
				ambient.b += 0.1;
				break;
			}
			break;
		case VK_W:
			switch (selected)
			{
			case 0:
				ambient.r -= 0.1;
				ambient.g -= 0.1;
				ambient.b -= 0.1;
				break;
			case 1:
				ambient.r -= 0.1;
				break;
			case 2:
				ambient.g -= 0.1;
				break;
			case 3:
				ambient.b -= 0.1;
				break;
			}
			break;
		case VK_E:
			switch (selected)
			{
			case 0:
				direct.r += 0.1;
				direct.g += 0.1;
				direct.b += 0.1;
				break;
			case 1:
				direct.r += 0.1;
				break;
			case 2:
				direct.g += 0.1;
				break;
			case 3:
				direct.b += 0.1;
				break;
			}
			break;
		case VK_R:
			switch (selected)
			{
			case 0:
				direct.r -= 0.1;
				direct.g -= 0.1;
				direct.b -= 0.1;
				break;
			case 1:
				direct.r -= 0.1;
				break;
			case 2:
				direct.g -= 0.1;
				break;
			case 3:
				direct.b -= 0.1;
				break;
			}
			break;
		case VK_T:
			selected = 1;
			break;
		case VK_Y:
			selected = 2;
			break;
		case VK_U:
			selected = 3;
			break;
		case VK_I:
			selected = 0;
			break;

		}
		break;
	case WM_LBUTTONDOWN:
		oldMousePos.x  = currentMousePos.x = LOWORD ( lParam );
		oldMousePos.y = currentMousePos.y = HIWORD ( lParam );
		isMouseActive = true;
		break;
	case WM_LBUTTONUP:
		isMouseActive = false;
		break;
	case WM_MOUSEMOVE:
		currentMousePos.x = LOWORD ( lParam );
		currentMousePos.y = HIWORD ( lParam );

		if(isMouseActive)
		{
			xRotation -= ( currentMousePos.x - oldMousePos.x )/2;
			yRotation -= ( currentMousePos.y - oldMousePos.y )/2;
		}

		oldMousePos.x = currentMousePos.x;
		oldMousePos.y = currentMousePos.y;
		break;
	case WM_MOUSEWHEEL:
		 angleofview -= 0.01;
		break;
	default:	// Always have a default in case
		break;
	}

	// Pass all of the unhandled messages to DefWindowProc
	return ( DefWindowProc( hwnd, message, wParam, lParam ) );
}

//-----------------------------------------------------------------------------
// Name:	WinMain
// Parms:	hInstance		= application instance
//			hPrevInstance	= previous application instance
//			lpCmdLine		= 
//			nShowCmd		= 
// Returns:	0 for failure or wParam
// Desc:	Message handling function
//-----------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	MSG msg;                // Message variable
	WNDCLASSEX windowClass; // Window class
	HWND hwnd;              // Window handle
	bool isFinished;        // Used to check if the program is done or not

	// Window class
	windowClass.cbSize			= sizeof( WNDCLASSEX );					// Size of the WNDCLASSEX structure
	windowClass.style			= CS_HREDRAW | CS_VREDRAW;				// Style of the window
	windowClass.lpfnWndProc		= WndProc;								// Address to the windows procedure
	windowClass.cbClsExtra		= 0;									// Extra class information
	windowClass.cbWndExtra		= 0;									// Extra window information
	windowClass.hInstance		= hInstance;							// Handle of application Instance
	windowClass.hIcon			= LoadIcon( NULL, IDI_APPLICATION );	// Handle of application Icon
	windowClass.hCursor			= LoadCursor( NULL, IDC_ARROW );		// Mouse cursor
	windowClass.hbrBackground	= NULL;									// Background color
	windowClass.lpszMenuName	= NULL;									// Name of the main menu
	windowClass.lpszClassName	= "OpenGL";								// Window class name
	windowClass.hIconSm			= LoadIcon( NULL, IDI_APPLICATION );	// Icon when minimized

	// Register you class with Windows.
	if(!RegisterClassEx(&windowClass))
		return 0;

	DWORD dwWindowStyle;

	if ( WINDOWBORDER )
		dwWindowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_POPUP | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	else
		dwWindowStyle = WS_VISIBLE | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	// Create the window.
	hwnd = CreateWindowEx( NULL,								// The extended window style
		"OpenGL",							// Window Class name
		"OpenGL with GLSL: Communications Demonstration",// Window name
		dwWindowStyle,						// Window style
		WINPOSX, WINPOSY,					// Window x, y coordinate
		WINDOWWIDTH, WINDOWHEIGHT,			// Window width and height
		NULL,								// Handle to parent window
		NULL,								// Menu
		hInstance,							// Handle to app instance
		NULL );								// Pointer to window creation data

	// If there was an error with creating the window, close the program
	if(!hwnd)
		return 0;
	//		GLGETERROR("winmainpre");

	ShowWindow( hwnd, SW_SHOW ); // Show the window
	//GLGETERROR("winmainpre2");
	UpdateWindow( hwnd );        // forces a paint message
	//GLGETERROR("winmainpre3");

	isFinished = false;        // False = running, True = not running


	if (!hRC) 
	{
		MessageBox( NULL,"OpenGL 3.2 RC was not created!","No Context", MB_OK);
		return 1; 
	}
	ReadFile("box.dat");
	polyarrays=new float*[3];
	polyarraysizes=set_vertex_arrays(polyarrays,m_iNumOfPolys,polylist);
	if (!bInitialiseGL( ))	
	{
		MessageBox( NULL,"OpenGL 3.2 RC was not created!","Extensions failed", MB_OK);
		return 1; 
	}
	if (!bInitialiseGLTextures())
	{
		MessageBox( NULL,"OpenGL 3.2 RC was not created!","Textures failed", MB_OK);
		return 1; 
	}






	// Messsage loop
	while( !isFinished )
	{
		GLGETERROR("winmainloop");
		if( PeekMessage( &msg, NULL, NULL, NULL, PM_REMOVE ) )
		{
			// If a quit message is received then stop rendering and quit the app
			if( msg.message == WM_QUIT )
				isFinished = true;

			TranslateMessage( &msg );  // Translate any messages
			DispatchMessage( &msg );   // Dispatch any messages
		}
		else
			RenderScene( );          // Render a frame
	}

	//Shutdown( );//new version of this routine not sorted yet - so removed

	return (int)msg.wParam;
}

//-----------------------------------------------------------------------------
// Name:	Shutdown
// Parms:	None
// Returns:	None
// Desc:	Shutdown OpenGL and GLSL
//-----------------------------------------------------------------------------
//void Shutdown()
//{
//	Routine needs re-instating with new 3.2 versions of routines
//
//}
//-----------------------------------------------------------------------------
// Name: displayReadInfo
// Desc: Routine to display useful info after file read, shows light vector
//		 as an example, modify and use for debugging as required
//-----------------------------------------------------------------------------
void displayReadInfo( )
{
	char sDispString[50];
	sprintf( sDispString, "%d polygons read", m_iNumOfPolys );
	MessageBox( NULL, m_sFilename,sDispString , MB_OK );		
  
	sprintf( sDispString , "Light Vector %f  %f  %f", m_vLight.x, m_vLight.y, m_vLight.z );
	MessageBox( NULL,"Start Values:", sDispString  , MB_OK );	
}


//-----------------------------------------------------------------------------
// Name: LoadPolys
// Desc: Read polygon info from file
//-----------------------------------------------------------------------------
int LoadPolys( FILE *infile )
{
	char cInString[1000];
	char filename[200];
	int iNumPolys = 0;
	int read_code;
	float fLength;
	FILE *texfile;
	float fR, fG, fB;		// red, green, blue values
	int vertcount=0;
	fgets( cInString, 1000, infile);						 // Read first/next line of file
	
	while (strncmp(cInString,"end of texture",14)!=0)
	{
		
		sscanf(cInString,"%s%d",filename,&read_code);
		//   for (i=0;i<80;i++)if (filename[i]=='\n')filename[i]=0;
		fgets( cInString, 1000, infile);						 // Read first/next line of file

		texfile=fopen(filename,"rb");
		if(!texfile) 
		{
			return 0;//bail out if file not found
		}
		else
		used_textures=mip_map_texture_file_read(texfile,cInString,used_textures);
		fgets( cInString, 1000, infile);						 // Read first/next line of file

	}
	do
	{
		fgets( cInString, 1000, infile);						 // Read first/next line of file
		sscanf( cInString, "%d%d%d%d", &polylist[iNumPolys].nv,&polylist[iNumPolys].texindex, &polylist[iNumPolys].specindex, &polylist[iNumPolys].normalindex);   // Get number of vertices and texture index
		polylist[iNumPolys].texsize.u=global_textures[polylist[iNumPolys].texindex].sx;
		polylist[iNumPolys].texsize.v=global_textures[polylist[iNumPolys].texindex].sy;

//		fprintf( debugfile, "number of vertices: %d\n", polylist[iNumPolys].nv);   // print number of vertices to debug file
		if  (polylist[iNumPolys].nv == 0)
			break;	// Get out if terminating zero found

		// Only allocate the memory we need - ALWAYS remember to delete on shutdown

		for (int i = 0; i < polylist[iNumPolys].nv; i++)
		{
			// Read next line of file
			fgets(cInString, 1000, infile);
			//Get Coordinates
			sscanf(cInString, "%f%f%f%f%f", &( polylist[iNumPolys].vert[i].x ),
										&( polylist[iNumPolys].vert[i].y ),
										&( polylist[iNumPolys].vert[i].z ) ,&( polylist[iNumPolys].texPos[i].u) ,&( polylist[iNumPolys].texPos[i].v));
			if (i==0 && iNumPolys==0)
				centre=polylist[i].vert[i];
			else
				centre=VectorSum(centre,polylist[iNumPolys].vert[i]);
			vertcount++;
		}
	
		polylist[iNumPolys].normal = Cross( VectorDiff( polylist[iNumPolys].vert[0], polylist[iNumPolys].vert[1] ),
									 VectorDiff( polylist[iNumPolys].vert[0],polylist[iNumPolys].vert[2] ) );
		fLength = (float)sqrt( Dot( polylist[iNumPolys].normal, polylist[iNumPolys].normal ) ); // Calculate length of vector

	    polylist[iNumPolys].normal.x /= fLength;	// Normalise
	    polylist[iNumPolys].normal.y /= fLength;	// each
	    polylist[iNumPolys].normal.z /= fLength;	// component
	    fgets(cInString, 1000, infile);		// Read  next line of file
	    sscanf( cInString, "%f%f%f", &fR, &fG, &fB );	// Get Colour, texture
		if (fR>1.0 ||fG>1.0||fB>1.0)//cope with either Open Gll 0-1.0 colours or old style 0-255 colours
		{
			polylist[iNumPolys].colour.r = fR/255.0;
			polylist[iNumPolys].colour.g = fG/255.0;
			polylist[iNumPolys].colour.b = fB/255.0;	
		}
		else
		{
			polylist[iNumPolys].colour.r = fR;
			polylist[iNumPolys].colour.g = fG;
			polylist[iNumPolys].colour.b = fB;	
		}
		iNumPolys++;
	} while( 1 );
	centre=VScale(1.0/vertcount,centre);
	return iNumPolys;  //Return number of polygons read
}


//-----------------------------------------------------------------------------
// Name: ReadFile
// Desc: Read polygon info from file
//-----------------------------------------------------------------------------
void ReadFile(char *filename)
{
	FILE *flInFile;
	flInFile = fopen( filename, "r" );
	m_iNumOfPolys = LoadPolys( flInFile );
	displayReadInfo();
	fclose(flInFile);	
}
int set_vertex_arrays(float **arrays,int n_polys, POLYGON *plist)
{
	int pind,vcount=0,tcount=0;
	const int pointspervertex=3;
	const int vertspertriangle=3;
	const int tcoordspervertex=2;
	//calculate how many actual floating point numbers will be used (3 per vertex in this case)
	//This will need modifying if 1,2, or 4 component attributes are to be used
	//Each attribute will need its own separate count if the sizes are different
	for (pind=0;pind<n_polys;pind++)
	{
		vcount+=pointspervertex*(vertspertriangle*plist[pind].nv-2*vertspertriangle);
		tcount+=tcoordspervertex*(vertspertriangle*plist[pind].nv-2*vertspertriangle);
	}
	arrays[0]=new float[vcount];//verts
	arrays[1]=new float[vcount];//shades
	arrays[2]=new float[vcount];//vectors
	arrays[3]=new float[tcount];//tcoords
	vcount=0;
	tcount=0;
	int elecount=0;
	for (pind=0;pind<n_polys;pind++)
	{
		plist[pind].vstart=elecount;
		for (int i=1;i<plist[pind].nv-1;i++)
		{
				arrays[0][vcount]=plist[pind].vert[0].x;
				arrays[1][vcount]=plist[pind].colour.r;
				arrays[2][vcount++]=plist[pind].normal.x;
				arrays[0][vcount]=plist[pind].vert[0].y;
				arrays[1][vcount]=plist[pind].colour.g;
				arrays[2][vcount++]=plist[pind].normal.y;
				arrays[0][vcount]=plist[pind].vert[0].z;
				arrays[1][vcount]=plist[pind].colour.b;
				arrays[2][vcount++]=plist[pind].normal.z;
				arrays[3][tcount++]=plist[pind].texPos[0].u;
				arrays[3][tcount++]=plist[pind].texPos[0].v;
				elecount++;
			for (int j=i;j<i+2;j++)
			{
				arrays[0][vcount]=plist[pind].vert[j].x;
				arrays[1][vcount]=plist[pind].colour.r;
				arrays[2][vcount++]=plist[pind].normal.x;
				arrays[0][vcount]=plist[pind].vert[j].y;
				arrays[1][vcount]=plist[pind].colour.g;
				arrays[2][vcount++]=plist[pind].normal.y;
				arrays[0][vcount]=plist[pind].vert[j].z;
				arrays[1][vcount]=plist[pind].colour.b;
				arrays[2][vcount++]=plist[pind].normal.z;
				arrays[3][tcount++]=plist[pind].texPos[j].u;
				arrays[3][tcount++]=plist[pind].texPos[j].v;
				elecount++;

			}
		}
		plist[pind].length=elecount-plist[pind].vstart;
		plist[pind].index=pind;
	}
	return elecount;

}
void DrawPolygon(POLYGON *p)

{
	if (myshade_index>=0)
		glUniform1f(myshade_index,myuvshadearray[p->index]);// set that "global shade"
    GLGETERROR( "Drawpoly" );
	glUniform1i(mytexture_index,/*GL_TEXTURE0+*/p->texindex);
	glUniform1i(myspecmap_index,/*GL_TEXTURE0+*/p->specindex);
	glUniform1i(mynormalmap_index,/*GL_TEXTURE0+*/p->normalindex);
	if (p->specindex != 0)
	{
		glUniform1i(myhasspec_index,1);
	}
	else
	{
		glUniform1i(myhasspec_index, 0);
	}
	if (p->normalindex != 0)
	{
		glUniform1i(myhasnormal_index, 1);
	}
	else
	{
		glUniform1i(myhasnormal_index, 0);
	}
//	glUniform2fv(mytexturesize_index,1,(float *)(&(p->texsize)));
    GLGETERROR( "Drawpoly2" );

	glDrawArrays(GL_TRIANGLES, p->vstart, p->length);
		    GLGETERROR( "Drawpoly2" );

}