#pragma once
#include "InitOpenGLApp.h"
#include "InitGlutInput.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
using namespace glm;



class DempApp :
	public InitOpenGLApp,
	public InitGlutInput
{
private:
	double m_Angle;
	double m_CoinRotation;
	GLuint m_Coin;
	GLuint m_Background;
	GLuint m_Mario;
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
	// uniform parameter
	GLuint Mario_uniform;

	GLuint marioVAO, marioVBO, marioEBO;

	float vertices[32] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f  // top left
	};
	unsigned int indices[6] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
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
	void drawMarioTexture();
	void initMarioTexture();
private:
};


