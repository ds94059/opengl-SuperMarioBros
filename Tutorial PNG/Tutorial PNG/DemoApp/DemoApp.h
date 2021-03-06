#pragma once
#include "InitOpenGLApp.h"
#include "InitGlutInput.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include<cmath>
using namespace glm;



class DempApp :
	public InitOpenGLApp,
	public InitGlutInput
{
private:
	double m_Angle;
	double m_CoinRotation;
	struct 
	{
		float x, y;
	} question;
	mat4 coinMatrix,fireMatrix,endMatrix;

	// textureID
	GLuint m_Coin;
	GLuint m_Background;
	GLuint m_Mario;
	GLuint m_Mario_shader;
	GLuint m_Start;
	GLuint m_End;
	GLuint m_3;
	GLuint m_2;
	GLuint m_1;
	GLuint m_X;
	GLuint m_FlowerOpen;
	GLuint m_FlowerClose;
	GLuint m_Pipe;
	GLuint m_question1;
	GLuint m_question2;
	GLuint m_Noise;
	GLuint m_goomba1;
	GLuint m_goomba2;
	GLuint m_goombadead;
	GLuint m_shroom;
	GLuint m_end1;
	GLuint m_end2;
	GLuint m_end3;
	typedef const CHAR* JUMP;
	// uniform parameter
	GLfloat GL_coinTranslateX,GL_coinTranslateY,GL_fireTranslateX,GL_fireTranslateY;
	GLuint GL_coinTimer,GL_fireTimer, GL_right;
	GLuint GL_coinMatrix;
	GLuint GL_fireMatrix;
	GLuint GL_endTimer, GL_endMatrix,GL_endTranslateX, GL_endTranslateY;
	// VAO, VBO, EBO
	GLuint coinVAO, coinVBO, coinEBO;
	GLuint fireVAO, fireVBO, fireEBO;
	GLuint endVAO, endVBO, endEBO;
	float vertices[32] = { // (0,0)�b���W
		// positions           // colors           // texture coords
		-0.05f, -0.05f, 0.0f,  0.0f, 0.0f, 1.0f,   0, 1, // bottom left
		 0.05f, -0.05f, 0.0f,  0.0f, 1.0f, 0.0f,   1, 1, // bottom right
		 0.05f,  0.1f, 0.0f,   1.0f, 0.0f, 0.0f,   1, 0,  // top right
		-0.05f,  0.1f, 0.0f,   0.0f, 0.0f, 1.0f,   0, 0   // top left
	};
	float fireVertices[32] = {
		// positions         // colors           // texture coords
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0, 1, // bottom left
		 1.0f, -1.0f, 0.0f,	 0.0f, 1.0f, 0.0f,   1, 1, // bottom right
		 1.0f,  1.0f, 0.0f,	 1.0f, 0.0f, 0.0f,   1, 0,  // top right
		-1.0f,  1.0f, 0.0f,	 0.0f, 0.0f, 1.0f,   0, 0   // top left
	};
	float endVertices[32] = {
		// positions         // colors           // texture coords
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0, 1, // bottom left
		 1.0f, -1.0f, 0.0f,	 0.0f, 1.0f, 0.0f,   1, 1, // bottom right
		 1.0f,  1.0f, 0.0f,	 1.0f, 0.0f, 0.0f,   1, 0,  // top right
		-1.0f,  1.0f, 0.0f,	 0.0f, 0.0f, 1.0f,   0, 0   // top left
	};
	unsigned int indices[6] = {
		0,1,2, // first triangle
		2,3,0  // second triangle
	};

public:
	DempApp(void);
	~DempApp(void);

	void Initialize();
protected:
	void Update();
	void KeyDown(int key);
	void KeyPress(int key);
	void KeyUp(int key);
	void Display(bool auto_redraw = true);
	void Finalize();
	void move();
	void renderStand(int dir);
	void renderWalk(int dir);
	void renderSlide(int dir);
	void renderJump(int dir);
	void renderFlag(int dir);
	void renderBlock(float x, float y, bool used);
	bool blockRoof(float x, float y, float xl, float x2, float y1, float y2);
	bool haveGround(float x, float y);
	bool haveRoof(float x, float y);
	void walls();
	unsigned int loadTexture(std::string path, int imageType);
	void renderFire(int right);
	void initFire();
	void renderGetCoin();
	void initGetCoin();
private:
};


