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

	// uniform parameter
	GLuint Mario_uniform;
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
	bool haveGround(float x, float y);
	bool haveRoof(float x, float y);
	unsigned int loadTexture(std::string path, int imageType);
private:
};


