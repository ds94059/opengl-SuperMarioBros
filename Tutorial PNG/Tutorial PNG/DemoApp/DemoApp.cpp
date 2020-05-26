#include <windows.h>
#include "DemoApp.h"
#include "TextureApp.h"

DempApp::DempApp(void) :
	m_Angle(2),
	m_CoinRotation(0.0)
{
	Initialize();
}


DempApp::~DempApp(void)
{
}

void DempApp::Initialize()
{
	InitOpenGLApp::Initialize("App", 140, 140, 1200, 720);
	InitGlutInput::Initialize();
	m_Coin = TextureApp::GenTexture("Media\\Texture\\coin.png");
	m_Background = TextureApp::GenTexture("Media\\Texture\\ourmap.png");
}

void DempApp::Finalize()
{
	glDeleteTextures(1, &m_Coin);
}

void DempApp::Display(bool auto_redraw)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.02f, 0.3f, 0.55f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, m_Background);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2d(-1, -1);
	glTexCoord2d(1, 0); glVertex2d(20, -1);
	glTexCoord2d(1, 1); glVertex2d(20, 1);
	glTexCoord2d(0, 1); glVertex2d(-1, 1);
	glEnd();
	glPopMatrix();


	/*glPushMatrix();
	glTranslated(-0.305,0.1,0);
	glScaled(0.5,0.5,0);
	glRotatef(m_CoinRotation,0,1,0);
	glTranslated(-0.15,0,0);
	glBindTexture(GL_TEXTURE_2D,m_Coin);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);glVertex2d(0  ,0);
	glTexCoord2d(1,0);glVertex2d(0.3,0);
	glTexCoord2d(1,1);glVertex2d(0.3,0.5);
	glTexCoord2d(0,1);glVertex2d(0  ,0.5);
	glEnd();
	glPopMatrix();*/



	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	InitOpenGLApp::Display(false);
}

void DempApp::Update()
{
	m_CoinRotation += m_Angle;
	if (abs(m_CoinRotation) >= 90)
	{
		m_Angle *= -1;
	}
	glutPostRedisplay();
}

void DempApp::KeyPress(int key)
{
	InitGlutInput::KeyPress(key);
	if (key == KEY_RIGHT)
	{
		gluLookAt(0.1, 0, 0, 0.1, 0, -1, 0, 1, 0);
	}
	else if (key == KEY_LEFT)
	{
		gluLookAt(-0.1, 0, 0, -0.1, 0, -1, 0, 1, 0);
	}

}

void DempApp::KeyDown(int key)
{
	InitGlutInput::KeyDown(key);

	if (key == KEY_ENTER)
	{
		TextureApp::ScreenShot(std::string("ScreenShot"));
	}
	else if (key == KEY_RIGHT)
	{
		gluLookAt(0.1, 0, 0, 0.1, 0, -1, 0, 1, 0);
	}
	else if (key == KEY_LEFT)
	{
		gluLookAt(-0.1, 0, 0, -0.1, 0, -1, 0, 1, 0);
	}
}
