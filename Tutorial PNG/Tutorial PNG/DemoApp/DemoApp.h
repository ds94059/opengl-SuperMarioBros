#pragma once
#include "InitOpenGLApp.h"
#include "InitGlutInput.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
private:
};


