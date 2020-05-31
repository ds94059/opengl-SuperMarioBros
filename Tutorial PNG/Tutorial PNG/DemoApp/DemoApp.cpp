#include <windows.h>
#include "DemoApp.h"
#include "TextureApp.h"

#define STAND 0
#define WALKING 1
#define JUMPING 2
#define CROUCH 3

#define MAX_SPEED 10

bool RightButtonDown = false;
bool LeftButtonDown = false;

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
	m_Mario = TextureApp::GenTexture("Media\\Texture\\Mario2.png");
}

void DempApp::Finalize()
{
	glDeleteTextures(1, &m_Coin);
}

float walkingDistanceX = 0, walkingDistanceY = 0, farestPos = 0, screenmiddle=0;
int walkingState = 0, right = 1, state = STAND, pressTime = 0, speed = 0,timer=0;

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

	timer++;

	walkingDistanceX += 0.002*speed;
	// 
	if (farestPos < walkingDistanceX)
	{
		farestPos = walkingDistanceX;
	}
	std::cout << "farest:" << farestPos << std::endl;
	std::cout << "screenmiddle:" << screenmiddle << std::endl;
	std::cout << "now:" << walkingDistanceX << std::endl;
	if (farestPos > (0.222+screenmiddle))
	{
		gluLookAt((farestPos- (0.222 + screenmiddle)), 0, 0, (farestPos - (0.222 + screenmiddle)), 0, -1, 0, 1, 0);
		screenmiddle = farestPos - 0.222;
	}
	if (right)
	{
		if (state == WALKING)
		{
			if (speed >= 0) //正常跑步
			{
				if (walkingState < 3)
				{
					glPushMatrix();
					glTranslated(-0.305, -0.7, 0);
					glBindTexture(GL_TEXTURE_2D, m_Mario);
					glBegin(GL_QUADS);
					glTexCoord2d(0.20833333333, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
					glTexCoord2d(0.2447916666666667, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
					glTexCoord2d(0.2447916666666667, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
					glTexCoord2d(0.20833333333, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
					glEnd();
					glPopMatrix();
				}
				else if (walkingState < 6)
				{
					glPushMatrix();
					glTranslated(-0.305, -0.7, 0);
					glBindTexture(GL_TEXTURE_2D, m_Mario);
					glBegin(GL_QUADS);
					glTexCoord2d(0.25, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
					glTexCoord2d(0.2890625, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
					glTexCoord2d(0.2890625, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
					glTexCoord2d(0.25, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
					glEnd();
					glPopMatrix();
				}
				else if (walkingState < 9)
				{
					glPushMatrix();
					glTranslated(-0.305, -0.7, 0);
					glBindTexture(GL_TEXTURE_2D, m_Mario);
					glBegin(GL_QUADS);
					glTexCoord2d(0.29167, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
					glTexCoord2d(0.33073, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
					glTexCoord2d(0.33073, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
					glTexCoord2d(0.29167, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
					glEnd();
					glPopMatrix();
				}
			}
			else //滑行
			{
				glPushMatrix();
				glTranslated(-0.305, -0.7, 0);
				glBindTexture(GL_TEXTURE_2D, m_Mario);
				glBegin(GL_QUADS);
				glTexCoord2d(0.33333, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
				glTexCoord2d(0.37239, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
				glTexCoord2d(0.37239, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
				glTexCoord2d(0.33333, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
				glEnd();
				glPopMatrix();
			}
		}
		else if (state == STAND)
		{
			glPushMatrix();
			glTranslated(-0.305, -0.7, 0);
			glBindTexture(GL_TEXTURE_2D, m_Mario);
			glBegin(GL_QUADS);
			glTexCoord2d(0.458333, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
			glTexCoord2d(0.497395, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
			glTexCoord2d(0.497395, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
			glTexCoord2d(0.458333, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
			glEnd();
			glPopMatrix();
		}
	}
	else
	{
		if (state == WALKING)
		{
			if (speed <= 0) //正常跑步
			{
				if (walkingState < 3)
				{
					glPushMatrix();
					glTranslated(-0.305, -0.7, 0);
					glBindTexture(GL_TEXTURE_2D, m_Mario);
					glBegin(GL_QUADS);
					glTexCoord2d(0.2447916666666667, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
					glTexCoord2d(0.20833333333, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
					glTexCoord2d(0.20833333333, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
					glTexCoord2d(0.2447916666666667, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
					glEnd();
					glPopMatrix();
				}
				else if (walkingState < 6)
				{
					glPushMatrix();
					glTranslated(-0.305, -0.7, 0);
					glBindTexture(GL_TEXTURE_2D, m_Mario);
					glBegin(GL_QUADS);
					glTexCoord2d(0.2890625, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
					glTexCoord2d(0.25, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
					glTexCoord2d(0.25, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
					glTexCoord2d(0.2890625, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
					glEnd();
					glPopMatrix();
				}
				else if (walkingState < 9)
				{
					glPushMatrix();
					glTranslated(-0.305, -0.7, 0);
					glBindTexture(GL_TEXTURE_2D, m_Mario);
					glBegin(GL_QUADS);
					glTexCoord2d(0.33073, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
					glTexCoord2d(0.29167, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
					glTexCoord2d(0.29167, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
					glTexCoord2d(0.33073, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
					glEnd();
					glPopMatrix();
				}
			}
			else //滑行
			{
				glPushMatrix();
				glTranslated(-0.305, -0.7, 0);
				glBindTexture(GL_TEXTURE_2D, m_Mario);
				glBegin(GL_QUADS);
				glTexCoord2d(0.37239, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
				glTexCoord2d(0.33333, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
				glTexCoord2d(0.33333, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
				glTexCoord2d(0.37239, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
				glEnd();
				glPopMatrix();
			}
		}
		else if (state == STAND)
		{
			glPushMatrix();
			glTranslated(-0.305, -0.7, 0);
			glBindTexture(GL_TEXTURE_2D, m_Mario);
			glBegin(GL_QUADS);
			glTexCoord2d(0.497395, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
			glTexCoord2d(0.458333, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
			glTexCoord2d(0.458333, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
			glTexCoord2d(0.497395, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
			glEnd();
			glPopMatrix();
		}
	}


	if (state == WALKING)
	{
		move();
	}
	else if (state == STAND)
	{
		if (timer == 3)
		{
			if (speed > 0)
			{
				speed--;
			}
			else if (speed < 0)
			{
				speed++;
			}
		}


	}
	if (walkingState == 9)
		walkingState = 0;
	if (timer >= 3)
	{
		timer = 0;
	}
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
		state = WALKING;
		RightButtonDown = true;
		right = 1;
		//gluLookAt(0.1, 0, 0, 0.1, 0, -1, 0, 1, 0);
	}
	else if (key == KEY_LEFT)
	{
		state = WALKING;
		LeftButtonDown = true;
		right = 0;
		//gluLookAt(-0.1, 0, 0, -0.1, 0, -1, 0, 1, 0);
	}
}

void DempApp::KeyUp(int key)
{
	InitGlutInput::KeyUp(key);
	if (key == KEY_RIGHT)
	{
		RightButtonDown = false;
		state = STAND;
		//speed = 0;
		pressTime = 0;
	}
	else if (key == KEY_LEFT)
	{
		LeftButtonDown = false;
		state = STAND;
		//speed = 0;
		pressTime = 0;
	}
}

void DempApp::move()
{
	pressTime++;
	if (LeftButtonDown)
	{
		if (pressTime % 3 == 0)
			speed--;
		if (speed < -(MAX_SPEED))
			speed = -(MAX_SPEED);
	}
	if (RightButtonDown)
	{
		if (pressTime % 3 == 0)
			speed++;
		if (speed > MAX_SPEED)
			speed = MAX_SPEED;
	}
	walkingState++;
}