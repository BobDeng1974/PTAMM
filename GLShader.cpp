#include "GLShader.h"

namespace PTAMM {

#define printOpenGLError() printOglError(__FILE__, __LINE__)

Shader::Shader()
{
	m_VertexShader = NULL;
	m_FragmentShader = NULL;
	m_Program = NULL;
	m_Frag = NULL;
	m_Vert = NULL;
}

Shader::~Shader()
{
	CleanUp();
}
void Shader::CleanUp()
{
	if(NULL != m_VertexShader)
	{
		delete[] m_VertexShader; 
		m_VertexShader = NULL;
	}
	if(NULL != m_FragmentShader)
	{
		delete[] m_FragmentShader;
		m_FragmentShader = NULL;
	}

	//���shader��program�İ󶨹�ϵ
	glDetachShader(m_Program,m_Vert);
	glDetachShader(m_Program,m_Frag);

	//ɾ��shader��program
	glDeleteShader(m_Vert);
	glDeleteShader(m_Frag);
	glDeleteProgram(m_Program);	
	m_Vert = NULL;	
	m_Frag = NULL;
	m_Program = NULL;
}

bool Shader::TextFileRead(char *_fn, GLchar *&_shader)
{
	FILE *fp;
	int count = 0;

	fp = fopen(_fn,"rt");
	if(NULL == fp)
		return false;

	// ���ļ�ָ��ָ���ļ���ĩβ
	fseek(fp,0,SEEK_END);
	// �����ļ�β����ļ�ͷ��ƫ�������ļ����ȣ�
	count = ftell(fp);
	// ���ļ�ָ������ָ���ļ���ͷ��
	rewind(fp);

	if(count<=0)
		return false;

	_shader = new GLchar[count + 8];	//+8��ֹԽ��
	count = fread(_shader,sizeof(GLchar),count,fp);
	_shader[count] = '\0';
	fclose(fp);
	return true;
}

bool Shader::ReadVertextShader(char *_fn)
{
	if(TextFileRead(_fn,m_VertexShader))
		return true;
	else
		return false;
}

bool Shader::ReadFragmentShader(char *_fn)
{
	if(TextFileRead(_fn,m_FragmentShader))
		return true;
	else
		return false;
}

bool Shader::SetShaderFile(char* sVSFileName, char* sFSFileName)
{
	if (glIsProgram(m_Program))
	{
		CleanUp();
	}
	if (!ReadVertextShader(sVSFileName) || !ReadFragmentShader(sFSFileName))
	{
		return false;
	}
	GLint vertCompiled, fragCompiled;			//״ֵ̬
	GLint linked;
	//����shader����
	m_Vert = glCreateShader(GL_VERTEX_SHADER);
	m_Frag = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar *vv = m_VertexShader;
	const GLchar *ff = m_FragmentShader;
	//���shader
	glShaderSource(m_Vert,1,&vv,NULL);
	glShaderSource(m_Frag,1,&ff,NULL);

	//����shader
	glCompileShader(m_Vert);
	printOpenGLError(); //���OpenGL����
	//glGetObjectParameterivARB(m_Vert, GL_OBJECT_COMPILE_STATUS_ARB, &vertCompiled);
	glGetShaderiv(m_Vert, GL_COMPILE_STATUS, &vertCompiled);
	printShaderInfoLog(m_Vert);
	glCompileShader(m_Frag);
	printOpenGLError(); //���OpenGL����
	//glGetObjectParameterivARB(m_Frag, GL_OBJECT_COMPILE_STATUS_ARB, &fragCompiled);
	glGetShaderiv(m_Frag, GL_COMPILE_STATUS, &fragCompiled);
	printShaderInfoLog(m_Frag); 

	if (!vertCompiled || !fragCompiled)
		return false;

	//�����������
	m_Program = glCreateProgram();

	//��shader���������
	glAttachShader(m_Program,m_Vert);
	glAttachShader(m_Program,m_Frag);	

	//���ӳ���
	glLinkProgram(m_Program);
	printOpenGLError(); //���OpenGL����
	//glGetObjectParameterivARB(m_Program, GL_OBJECT_COMPILE_STATUS_ARB, &linked);
	glGetProgramiv(m_Program, GL_LINK_STATUS, &linked);  
	printProgramInfoLog(m_Program);

	if (!linked)
		return false; 
	UseShader(true);				
	return true;
}

void Shader::UseShader(bool bOn)
{
	if (!bOn)
	{
		glUseProgram(0);
	}
	else
	{
		glUseProgram(m_Program);		
	}
}

GLint Shader::getUniLoc(GLuint program, const GLchar *name)
{
	GLint loc;
	loc = glGetUniformLocation(program, name);
	if (loc == -1)
		printf("uniform variates %s undefined! \n", name);
	//printOpenGLError(); // Check for OpenGL errors
	return loc;
} 
void Shader::SetUniVar(char* sVarName, float fValue0, float fValue1, float fValue2)
{
	//���ó�ʼһ�±���ֵ
	glUniform3f(getUniLoc(m_Program, sVarName), fValue0, fValue1, fValue2); 

}
void Shader::SetUniVar(char* sVarName, float fValue0, float fValue1)
{
	//���ó�ʼһ�±���ֵ
	glUniform2f(getUniLoc(m_Program, sVarName), fValue0, fValue1);
}
void Shader::SetUniVar(char* sVarName, float fValue0)
{
	//���ó�ʼһ�±���ֵ
	glUniform1f(getUniLoc(m_Program, sVarName), fValue0);
}
void Shader::SetUniVar(char* sVarName, int nValue0)
{
	//���ó�ʼһ�±���ֵ
	glUniform1i(getUniLoc(m_Program, sVarName), nValue0);
}
void Shader::SetSampler(char* sVarName, int tex)
{
	GLint texSampler;
	texSampler = glGetUniformLocation(m_Program, sVarName);
	glUniform1i(texSampler, tex);
}
//---------------------------------------------------------------------------
/**
* @brief ��ӡOpenGL������Ϣ
* @param  file �������ڵ��ļ�
* @param  line �������ڵ���
* @return 1 OpenGL error
* @return 0 other  error
**/
int Shader::printOglError(char *file, int line)
{
	GLenum glErr;
	int retCode = 0;
	glErr = glGetError();///��ȡ����
	while (glErr != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
		retCode = 1;
		glErr = glGetError();///��ȡ��һ������
	}
	return retCode;
}

/*** @brief ��ӡ��־ ***/
void Shader::printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		free(infoLog);
	}
}

/*** @brief ��ӡ��־ ***/
void Shader::printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		free(infoLog);
	}
}

}
