#include <windows.h>
#include "DemoApp.h"
#include "TextureApp.h"
#include "LoadShaders.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STAND 0
#define WALKING 1
#define JUMPING 2
#define CROUCH 3

#define MAX_SPEED 10

#define STANDING 0
#define RISING 1
#define FALLING 2
#define FLOATING 3

bool RightButtonDown = false;
bool LeftButtonDown = false;
bool UpButtonDown = false;

GLuint program;
unsigned int quadVAO, quadVBO;
unsigned int framebuffer;
unsigned int textureColorbuffer;

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
	//m_Mario = DempApp::loadTexture("Media\\Texture\\Mario2.png",GL_RGBA);
	m_Mario = TextureApp::GenTexture("Media\\Texture\\Mario2.png");
	m_Start = TextureApp::GenTexture("Media\\Texture\\startpage.jpg");
	m_End = TextureApp::GenTexture("Media\\Texture\\gameover.jpg");
	m_3 = TextureApp::GenTexture("Media\\Texture\\3.png");
	m_2 = TextureApp::GenTexture("Media\\Texture\\2.png");
	m_1 = TextureApp::GenTexture("Media\\Texture\\1.png");
	m_X = TextureApp::GenTexture("Media\\Texture\\X.png");

	m_FlowerOpen = TextureApp::GenTexture("Media\\Texture\\flowerOpen.png");
	m_FlowerClose = TextureApp::GenTexture("Media\\Texture\\flowerClose.png");

	m_question1 = TextureApp::GenTexture("Media\\Texture\\questionblock.png");
	m_question2 = TextureApp::GenTexture("Media\\Texture\\questionblock2.png");
	m_Pipe = TextureApp::GenTexture("Media\\Texture\\Pipe.png");

	//ShaderInfo shaders[] = {
	//	{ GL_VERTEX_SHADER, "Mario.vs" },//vertex shader
	//	{ GL_FRAGMENT_SHADER, "Mario.fs" },//fragment shader
	//	{ GL_NONE, NULL } };
	//program = LoadShaders(shaders);//讀取shader
	//glUseProgram(program);//uniform參數數值前必須先use shader

	//m_Mario = TextureApp::GenTexture("Media\\Texture\\Mario2.png");
	//glBindTexture(GL_TEXTURE_2D, m_Mario);
	////glActiveTexture(GL_TEXTURE0);
	////glDrawArrays(GL_TRIANGLE_STRIP,0,4);

}

void DempApp::Finalize()
{
	glDeleteTextures(1, &m_Coin);
}

float walkingDistanceX = 0, walkingDistanceY = 0, farestPos = 0, screenmiddle = 0, jumpGroundHeight = 0, startrange = 0.7;
int walkingState = 0, right = 1, state = STAND, pressTime = 0, pressTimeUp = 0, speed = 0, risingSpeed = 5, falltimer = 0, timer = 0, inair = 0, floattimer = 0, endingstep = 0, endtimer = 0, startGame = 0, die = 0, dietimer = 0, starttimer = 0, lives = 3, flowerMove = 0;
bool controlAble = 0,//1可控制 0不可
endGame = 0;
bool firstlowquestion = 0, firsthighquestion=0;
void DempApp::Display(bool auto_redraw)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	if (startGame == 0)
	{
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, m_Start);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex2d(-1, -1);
		glTexCoord2d(1, 0); glVertex2d(1, -1);
		glTexCoord2d(1, 1); glVertex2d(1, 1);
		glTexCoord2d(0, 1); glVertex2d(-1, 1);
		glEnd();
		glPopMatrix();
	}
	else if (startGame == 2)
	{
		glPushMatrix();
		glTranslated(-0.305, -0.7, 0);
		glBindTexture(GL_TEXTURE_2D, m_Mario);
		glBegin(GL_QUADS);
		glTexCoord2d(0.458333, 0.91098); glVertex2d(-0.05, -0.05 + startrange);
		glTexCoord2d(0.497395, 0.91098); glVertex2d(0.05, -0.05 + startrange);
		glTexCoord2d(0.497395, 0.9385); glVertex2d(0.05, 0.1 + startrange);
		glTexCoord2d(0.458333, 0.9385); glVertex2d(-0.05, 0.1 + startrange);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-0.305, -0.7, 0);
		glBindTexture(GL_TEXTURE_2D, m_X);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex2d(0.275, 0.675);
		glTexCoord2d(1, 0); glVertex2d(0.325, 0.675);
		glTexCoord2d(1, 1); glVertex2d(0.325, 0.775);
		glTexCoord2d(0, 1); glVertex2d(0.275, 0.775);
		glEnd();
		glPopMatrix();
		switch (lives)
		{
		case(3):
			glPushMatrix();
			glTranslated(-0.305, -0.7, 0);
			glBindTexture(GL_TEXTURE_2D, m_3);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2d(0.55, 0.65);
			glTexCoord2d(1, 0); glVertex2d(0.65, 0.65);
			glTexCoord2d(1, 1); glVertex2d(0.65, 0.8);
			glTexCoord2d(0, 1); glVertex2d(0.55, 0.8);
			glEnd();
			glPopMatrix();
			break;
		case(2):
			glPushMatrix();
			glTranslated(-0.305, -0.7, 0);
			glBindTexture(GL_TEXTURE_2D, m_2);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2d(0.55, 0.65);
			glTexCoord2d(1, 0); glVertex2d(0.65, 0.65);
			glTexCoord2d(1, 1); glVertex2d(0.65, 0.8);
			glTexCoord2d(0, 1); glVertex2d(0.55, 0.8);
			glEnd();
			glPopMatrix();
			break;
		case(1):
			glPushMatrix();
			glTranslated(-0.305, -0.7, 0);
			glBindTexture(GL_TEXTURE_2D, m_1);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2d(0.55, 0.65);
			glTexCoord2d(1, 0); glVertex2d(0.65, 0.65);
			glTexCoord2d(1, 1); glVertex2d(0.65, 0.8);
			glTexCoord2d(0, 1); glVertex2d(0.55, 0.8);
			glEnd();
			glPopMatrix();
			break;
		default:
			break;
		}

		if (starttimer == 0)
		{
			timer = 0;
			walkingDistanceX = 0;
			walkingDistanceY = 0;
			farestPos = 0;
			jumpGroundHeight = 0;
			walkingState = 0;
			right = 1;
			state = STAND;
			speed = 0;
			die = 0;
			firstlowquestion = 0;
			firsthighquestion = 0;
			state = STANDING;
			LeftButtonDown = FALSE;
			RightButtonDown = FALSE;
		}
		starttimer++;

		if (starttimer > 60)
		{
			startrange -= 0.01;
			if (startrange <= 0)
			{
				startrange = 0.7;
				starttimer = 0;
				startGame = 1;
				controlAble = 1;
			}

		}
	}
	else if (startGame == 3)
	{
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, m_End);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex2d(-1, -1);
		glTexCoord2d(1, 0); glVertex2d(1, -1);
		glTexCoord2d(1, 1); glVertex2d(1, 1);
		glTexCoord2d(0, 1); glVertex2d(-1, 1);
		glEnd();
		glPopMatrix();
	}
	else if (startGame == 1)
	{
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, m_Background);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex2d(-1, -1);
		glTexCoord2d(1, 0); glVertex2d(20, -1);
		glTexCoord2d(1, 1); glVertex2d(20, 1);
		glTexCoord2d(0, 1); glVertex2d(-1, 1);
		glEnd();
		glPopMatrix();

		// 第一朵花
		if (flowerMove < 10)
		{
			glPushMatrix();
			glTranslated(-0.305, -0.7, 0);
			glBindTexture(GL_TEXTURE_2D, m_FlowerOpen);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2d(13.607, 0.22);
			glTexCoord2d(1, 0); glVertex2d(13.707, 0.22);
			glTexCoord2d(1, 1); glVertex2d(13.707, 0.47);
			glTexCoord2d(0, 1); glVertex2d(13.607, 0.47);
			glEnd();
			glPopMatrix();
			flowerMove++;
		}
		else
		{
			glPushMatrix();
			glTranslated(-0.305, -0.7, 0);
			glBindTexture(GL_TEXTURE_2D, m_FlowerClose);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2d(13.607, 0.22);
			glTexCoord2d(1, 0); glVertex2d(13.707, 0.22);
			glTexCoord2d(1, 1); glVertex2d(13.707, 0.47);
			glTexCoord2d(0, 1); glVertex2d(13.607, 0.47);
			glEnd();
			glPopMatrix();
			flowerMove++;
			if (flowerMove > 20)
				flowerMove = 0;
		}
		// 第一個問號 低
		if (!firstlowquestion)
		{
			glPushMatrix();
			glTranslated(-0.305, -0.7, 0);
			glBindTexture(GL_TEXTURE_2D, m_question1);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2d(-0.05 + 1.539, -0.05 + 0.4);
			glTexCoord2d(1, 0); glVertex2d(0.05 + 1.539, -0.05 + 0.4);
			glTexCoord2d(1, 1); glVertex2d(0.05 + 1.539, 0.1 + 0.4);
			glTexCoord2d(0, 1); glVertex2d(-0.05 + 1.539, 0.1 + 0.4);
			glEnd();
			glPopMatrix();
		}
		else
		{
			glPushMatrix();
			glTranslated(-0.305, -0.7, 0);
			glBindTexture(GL_TEXTURE_2D, m_question2);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2d(-0.05 + 1.539, -0.05 + 0.4);
			glTexCoord2d(1, 0); glVertex2d(0.05 + 1.539, -0.05 + 0.4);
			glTexCoord2d(1, 1); glVertex2d(0.05 + 1.539, 0.1 + 0.4);
			glTexCoord2d(0, 1); glVertex2d(-0.05 + 1.539, 0.1 + 0.4);
			glEnd();
			glPopMatrix();
		}

		// 第一個問號 高
		if (!firsthighquestion)
		{
			glPushMatrix();
			glTranslated(-0.305, -0.7, 0);
			glBindTexture(GL_TEXTURE_2D, m_question1);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2d(-0.05 + 1.539, -0.05 + 0.95);
			glTexCoord2d(1, 0); glVertex2d(0.05 + 1.539, -0.05 + 0.95);
			glTexCoord2d(1, 1); glVertex2d(0.05 + 1.539, 0.1 + 0.95);
			glTexCoord2d(0, 1); glVertex2d(-0.05 + 1.539, 0.1 + 0.95);
			glEnd();
			glPopMatrix();
		}
		else
		{
			glPushMatrix();
			glTranslated(-0.305, -0.7, 0);
			glBindTexture(GL_TEXTURE_2D, m_question2);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2d(-0.05 + 1.539, -0.05 + 0.95);
			glTexCoord2d(1, 0); glVertex2d(0.05 + 1.539, -0.05 + 0.95);
			glTexCoord2d(1, 1); glVertex2d(0.05 + 1.539, 0.1 + 0.95);
			glTexCoord2d(0, 1); glVertex2d(-0.05 + 1.539, 0.1 + 0.95);
			glEnd();
			glPopMatrix();
		}

		//// 水管
		//glPushMatrix();
		//glScaled(0.2, 0.2, 0);
		//glBindTexture(GL_TEXTURE_2D, m_Pipe);
		//glBegin(GL_QUADS);
		//glTexCoord2d(0, 0); glVertex2d(0.32, 0);
		//glTexCoord2d(1, 0); glVertex2d(0.64, 0);
		//glTexCoord2d(1, 1); glVertex2d(0.64, 0.64);
		//glTexCoord2d(0, 1); glVertex2d(0.32, 0.64);
		//glEnd();
		//glPopMatrix();

		timer++;

		walkingDistanceX += 0.002*speed;
		// 最遠距離
		if (farestPos < walkingDistanceX)
		{
			farestPos = walkingDistanceX;
		}
		std::cout << "farest:" << farestPos << std::endl;
		std::cout << "screenmiddle:" << screenmiddle << std::endl;
		std::cout << "now:" << walkingDistanceX << std::endl;
		std::cout << "height" << walkingDistanceY << std::endl;
		std::cout << "risingSpeed: " << risingSpeed << std::endl;
		if (die == 1)
		{
			if (lives > 0)
			{
				dietimer++;
				if (dietimer > 120)
				{
					dietimer = 0;
					startGame = 2;
					gluLookAt(-screenmiddle, 0, 0, -screenmiddle, 0, -1, 0, 1, 0);
					screenmiddle = 0;
				}
			}
			else
			{
				gluLookAt(-screenmiddle, 0, 0, -screenmiddle, 0, -1, 0, 1, 0);
				screenmiddle = 0;
				startGame = 3;
			}
		}
		else if (endGame == 0)
		{
			// 螢幕中心點 起始為0
			if (farestPos > (0.222 + screenmiddle))
			{
				if (screenmiddle < 18.8)
				{
					gluLookAt((farestPos - (0.222 + screenmiddle)), 0, 0, (farestPos - (0.222 + screenmiddle)), 0, -1, 0, 1, 0);
					screenmiddle = farestPos - 0.222;
				}
			}
			walls();
			


			// 下墜 天花板 判定
			if ((haveGround(walkingDistanceX, walkingDistanceY) == 1) && inair != RISING)
			{
				inair = STANDING;
				jumpGroundHeight = walkingDistanceY;
			}
			else if (inair != RISING && inair != FALLING && inair != FLOATING)
			{
				risingSpeed = 2;
				inair = FALLING;
			}
			else if((haveRoof(walkingDistanceX, walkingDistanceY) == 1) && inair == RISING)
			{
				inair = FALLING;
			}

			if (inair == RISING)
			{
				if (UpButtonDown)
				{
					pressTimeUp++;
					if (pressTimeUp % 1 == 0)
						risingSpeed--;
					if (risingSpeed <= 2)
						risingSpeed = 2;

					walkingDistanceY += 0.02*risingSpeed;
				}

				if (walkingDistanceY > 0.65 + jumpGroundHeight)
				{
					inair = FLOATING;
				}
			}
			else if (inair == FLOATING)
			{
				floattimer++;
				if (floattimer > 1)
				{
					floattimer = 0;
					inair = FALLING;
				}
			}
			else if (inair == FALLING)
			{
				falltimer++;
				if (falltimer % 5 == 0)
					risingSpeed++;
				if (risingSpeed >= 5)
					risingSpeed = 5;
				walkingDistanceY -= 0.01*risingSpeed;
			}
			if (walkingDistanceY < -0.41)
			{
				die = 1;
				lives--;
				controlAble = 0;
			}
		}


		// 拉到旗子
		if (endGame == 1)
		{
			if (endingstep == 0)
			{
				renderFlag(1);
				walkingDistanceX = 18.9981;
				walkingDistanceY -= 0.005;
				if (walkingDistanceY <= 0.15)
				{
					endingstep = 1;
				}
			}
			else if (endingstep == 1)
			{
				endtimer++;
				if (endtimer > 60 && endtimer <= 90)
				{
					walkingDistanceX = 19.0821;
					renderFlag(-1);
				}
				else if (endtimer > 90)
				{
					endingstep = 2;
					endtimer = 0;
				}
				else
				{
					renderFlag(1);
				}
			}
			else if (endingstep == 2)
			{
				walkingDistanceY = 0;
				walkingDistanceX += 0.005;
				renderWalk(1);
				walkingState++;
				if (walkingState == 9)
					walkingState = 0;
				if (walkingDistanceX >= 19.51)
					endingstep = 3;
			}
			else if (endingstep == 3)
			{
				endtimer++;
				if (endtimer > 120)
				{
					endingstep = 4;
					startGame = 3;
					gluLookAt(-screenmiddle, 0, 0, -screenmiddle, 0, -1, 0, 1, 0);
				}
			}
		}
		else if (right)	//向右
		{
			if (inair != STANDING)
			{
				renderJump(1);
			}
			else if (state == WALKING)
			{
				if (speed >= 0) //正常跑步
				{
					renderWalk(1);
				}
				else //滑行
				{
					renderSlide(1);
				}
			}
			else if (state == STAND)
			{
				renderStand(1);
			}
		}
		else //向左
		{
			if (inair != STANDING)
			{
				renderJump(-1);
			}
			else if (state == WALKING)
			{
				if (speed <= 0) //正常跑步
				{
					renderWalk(-1);
				}
				else //滑行
				{
					renderSlide(-1);
				}
			}
			else if (state == STAND)
			{
				renderStand(-1);
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
	InitGlutInput::KeyPress(key);
	if (controlAble)
	{
		if (key == 'i')
		{
			walkingDistanceY += 0.01;
		}
		if (key == 'k')
		{
			walkingDistanceY -= 0.01;
		}
	}
}

void DempApp::KeyDown(int key)
{
	InitGlutInput::KeyDown(key);
	if (controlAble)
	{
		if (key == 'q')
		{
			walkingDistanceY += 0.01;
		}
		if (key == 'a')
		{
			walkingDistanceY -= 0.01;
		}
		if (key == KEY_RIGHT)
		{
			state = WALKING;
			RightButtonDown = true;
			LeftButtonDown = false;
			right = 1;
		}
		if (key == KEY_LEFT)
		{
			state = WALKING;
			LeftButtonDown = true;
			RightButtonDown = false;
			right = 0;
		}
		if (key == KEY_UP)
		{
			if (inair == STANDING)
			{
				inair = RISING;
				UpButtonDown = true;
			}
		}
	}
	if (startGame == 0)
	{
		if (key == KEY_ENTER)
		{
			startGame = 2;
			//TextureApp::ScreenShot(std::string("ScreenShot"));
		}
	}


}

void DempApp::KeyUp(int key)
{
	InitGlutInput::KeyUp(key);
	if (controlAble)
	{
		if (key == KEY_RIGHT)
		{
			if (right)
			{
				RightButtonDown = false;
				state = STAND;
				//speed = 0;
				pressTime = 0;
			}
		}
		if (key == KEY_LEFT)
		{
			if (right == 0)
			{
				LeftButtonDown = false;
				state = STAND;
				//speed = 0;
				pressTime = 0;
			}
		}
		if (key == KEY_UP)
		{
			UpButtonDown = false;
			pressTimeUp = 0;
			if (inair == RISING)
				inair = FLOATING;
		}
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

void DempApp::renderStand(int dir)
{
	if (dir > 0)
	{
		glPushMatrix();
		glTranslated(-0.305, -0.7, 0);
		glBindTexture(GL_TEXTURE_2D, m_Mario);
		glBegin(GL_QUADS);
		glTexCoord2d(0.458333, 0.91098);glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
		glTexCoord2d(0.497395, 0.91098);glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
		glTexCoord2d(0.497395, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
		glTexCoord2d(0.458333, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
		glEnd();
		glPopMatrix();
	}
	else
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

void DempApp::renderWalk(int dir)
{
	if (dir > 0)
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
	else
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

}

void DempApp::renderSlide(int dir)
{
	if (dir > 0)
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
	else
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

void DempApp::renderJump(int dir)
{
	if (dir > 0)
	{
		glPushMatrix();
		glTranslated(-0.305, -0.7, 0);
		glBindTexture(GL_TEXTURE_2D, m_Mario);
		glBegin(GL_QUADS);
		glTexCoord2d(0.375, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
		glTexCoord2d(0.41406, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
		glTexCoord2d(0.41406, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
		glTexCoord2d(0.375, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
		glEnd();
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
		glTranslated(-0.305, -0.7, 0);
		glBindTexture(GL_TEXTURE_2D, m_Mario);
		glBegin(GL_QUADS);
		glTexCoord2d(0.41406, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
		glTexCoord2d(0.375, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
		glTexCoord2d(0.375, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
		glTexCoord2d(0.41406, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
		glEnd();
		glPopMatrix();
	}
}

void DempApp::renderFlag(int dir)
{
	if (dir > 0)
	{
		glPushMatrix();
		glTranslated(-0.305, -0.7, 0);
		glBindTexture(GL_TEXTURE_2D, m_Mario);
		glBegin(GL_QUADS);
		glTexCoord2d(0.54166, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
		glTexCoord2d(0.58072, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
		glTexCoord2d(0.58072, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
		glTexCoord2d(0.54166, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
		glEnd();
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
		glTranslated(-0.305, -0.7, 0);
		glBindTexture(GL_TEXTURE_2D, m_Mario);
		glBegin(GL_QUADS);
		glTexCoord2d(0.58072, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
		glTexCoord2d(0.54166, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
		glTexCoord2d(0.54166, 0.9385); glVertex2d(0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
		glTexCoord2d(0.58072, 0.9385); glVertex2d(-0.05 + walkingDistanceX, 0.1 + walkingDistanceY);
		glEnd();
		glPopMatrix();
	}

}



bool DempApp::haveGround(float x, float y)
{
	// 第一大地板上的水管
	if (y < 0.4100001 && x >= 1.102 && x <= 1.35)
	{
		walkingDistanceY = 0.41;
		risingSpeed = 5;
		falltimer = 0;
		return 1;
	}
	// 第一個問號 低
	else	if (y>0.4 && y < 0.5500001 && x >= 1.445 && x <= 1.635)
		{
			walkingDistanceY = 0.55;
			risingSpeed = 5;
			falltimer = 0;
			return 1;
		}
	// 第一個問號 高
	else	if (y > 0.95 && y < 1.100001 && x >= 1.445 && x <= 1.635)
	{
		walkingDistanceY = 1.1;
		risingSpeed = 5;
		falltimer = 0;
		return 1;
	}
	// 第一大地板
	else if (y <= 0 && x >= -0.667&&x <= 2.123)
	{
		walkingDistanceY = 0;
		risingSpeed = 5;
		falltimer = 0;
		return 1;
	}
	// 第二大地板
	else if (y <= 0 && x >= 2.255&&x <= 6.149)
	{
		walkingDistanceY = 0;
		risingSpeed = 5;
		falltimer = 0;
		return 1;
	}
	// 第三大地板上的撞柱
	else if (y < 0.4100001 && x >= 8.27 && x <= 8.445)
	{
		walkingDistanceY = 0.41;
		risingSpeed = 5;
		falltimer = 0;
		return 1;
	}
	// 第三大地板上的水管
	else if (y < 0.5400001 && x >= 9.41198 && x <= 9.66597)
	{
		walkingDistanceY = 0;
		walkingDistanceY = 0.54;
		risingSpeed = 5;
		falltimer = 0;
		return 1;
	}
	// 第三大地板上的水管2
	else if (y < 0.5400001 && x >= 10.814 && x <= 11.064)
	{
		walkingDistanceY = 0;
		walkingDistanceY = 0.54;
		risingSpeed = 5;
		falltimer = 0;
		return 1;
	}
	// 第三大地板
	else if (y <= 0 && x >= 6.463&&x <= 12.3651)
	{
		walkingDistanceY = 0;
		risingSpeed = 5;
		falltimer = 0;
		return 1;
	}
	// 第四大地板
	else if (y <= 0 && x >= 12.4981 &&x <= 14.5501)
	{
		walkingDistanceY = 0;
		risingSpeed = 5;
		falltimer = 0;
		return 1;
	}
	//第四大地板 小水管
	else if (y < 0.2700001 && x >= 13.528 && x <= 13.778)
	{
		walkingDistanceY = 0;
		walkingDistanceY = 0.27;
		risingSpeed = 5;
		falltimer = 0;
		return 1;
	}
	// 第五大地板
	else if (y <= 0 && x >= 14.7761&&x <= 15.0721)
	{
		walkingDistanceY = 0;
		risingSpeed = 5;
		falltimer = 0;
		return 1;
	}
	// 第六大地板
	else if (y <= 0 && x >= 15.2181&&x <= 15.81)
	{
		walkingDistanceY = 0;
		risingSpeed = 5;
		falltimer = 0;
		return 1;
	}
	// 第六大地板上的撞柱
	else if (y < 0.4100001 && x >= 15.8001 && x <= 15.9501)
	{
		walkingDistanceY = 0.41;
		risingSpeed = 5;
		falltimer = 0;
		return 1;
	}
	// 最終大地板
	else if (y <= 0 && x >= 16.0841)
	{
		walkingDistanceY = 0;
		risingSpeed = 5;
		falltimer = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}
bool DempApp::haveRoof(float x, float y)
{
	// 第一個問號 低
	if (y > 0.25 && y < 0.400001 && x >= 1.445 && x <= 1.635)
		{
		firstlowquestion = 1;
		risingSpeed = 2;
		falltimer = 0;
		return 1;
		}
	else if (y > 0.8 && y < 0.9500001 && x >= 1.445 && x <= 1.635)
	{
		firsthighquestion = 1;
		risingSpeed = 2;
		falltimer = 0;
		return 1;
	}
	return false;
}

void DempApp::walls()
{
	// 螢幕左邊的牆壁 與螢幕中心點相差0.648
	if (walkingDistanceX < -0.648 + screenmiddle)
	{
		walkingDistanceX = -0.647 + screenmiddle;
		speed = 0;
		state = STAND;
	}

	// 第一大地板上的水管 左邊
	else if (walkingDistanceX > 1.102 && walkingDistanceX < 1.122 && walkingDistanceY >= 0 && walkingDistanceY < 0.4)
	{
		walkingDistanceX = 1.101;
		speed = 0;
		state = STAND;
	}

	// 第一大地板上的水管 右邊
	else if (walkingDistanceX < 1.35 && walkingDistanceX> 1.33 &&  walkingDistanceY >= 0 && walkingDistanceY < 0.4)
	{
		walkingDistanceX = 1.351;
		speed = 0;
		state = STAND;
	}

	// 第一個問號 低 左邊
	else if (walkingDistanceX > 1.445 && walkingDistanceX < 1.465 && walkingDistanceY >= 0.25 && walkingDistanceY < 0.54)
	{
		walkingDistanceX = 1.444;
		speed = 0;
		state = STAND;
	}

	// 第一個問號 低 右邊
	else if (walkingDistanceX < 1.635 && walkingDistanceX> 1.615 &&  walkingDistanceY >= 0.25 && walkingDistanceY < 0.54)
	{
		walkingDistanceX = 1.636;
		speed = 0;
		state = STAND;
	}

	// 第一個問號 高 左邊
	else if (walkingDistanceX > 1.445 && walkingDistanceX < 1.465 && walkingDistanceY >= 0.8 && walkingDistanceY < 1)
	{
		walkingDistanceX = 1.444;
		speed = 0;
		state = STAND;
	}

	// 第一個問號 高 右邊
	else if (walkingDistanceX < 1.635 && walkingDistanceX> 1.615 &&  walkingDistanceY >= 0.8 && walkingDistanceY < 1)
	{
		walkingDistanceX = 1.636;
		speed = 0;
		state = STAND;
	}
	// 第一大地板 右邊
	else if (walkingDistanceX < 2.123 && walkingDistanceX>2.103 &&  walkingDistanceY < 0)
	{
		walkingDistanceX = 2.1231;
		speed = 0;
		state = STAND;
	}

	// 第二大地板 左邊
	else if (walkingDistanceX > 2.255 && walkingDistanceX < 2.275 && walkingDistanceY < 0)
	{
		walkingDistanceX = 2.254;
		speed = 0;
		state = STAND;
	}

	// 第二大地板 右邊
	else if (walkingDistanceX < 6.149 && walkingDistanceX>6.129 &&  walkingDistanceY < 0)
	{
		walkingDistanceX = 6.1491;
		speed = 0;
		state = STAND;
	}

	// 第三大地板 左邊
	else if (walkingDistanceX > 6.463 && walkingDistanceX < 6.483 && walkingDistanceY < 0)
	{
		walkingDistanceX = 6.462;
		speed = 0;
		state = STAND;
	}

	// 第三大地板上的撞柱 左邊
	else if (walkingDistanceX > 8.27 && walkingDistanceX < 8.29 && walkingDistanceY >= 0 && walkingDistanceY < 0.4)
	{
		walkingDistanceX = 8.269;
		speed = 0;
		state = STAND;
	}

	// 第三大地板上的撞柱 右邊
	else if (walkingDistanceX < 8.445 && walkingDistanceX> 8.425 &&  walkingDistanceY >= 0 && walkingDistanceY < 0.4)
	{
		walkingDistanceX = 8.4451;
		speed = 0;
		state = STAND;
	}

	// 第三大地板上的水管 左邊
	else if (walkingDistanceX > 9.41198 && walkingDistanceX < 9.43198 && walkingDistanceY >= 0 && walkingDistanceY < 0.53)
	{
		walkingDistanceX = 9.4119;
		speed = 0;
		state = STAND;
	}

	// 第三大地板上的水管 右邊
	else if (walkingDistanceX < 9.66597 && walkingDistanceX> 9.64597 &&  walkingDistanceY >= 0 && walkingDistanceY < 0.53)
	{
		walkingDistanceX = 9.666;
		speed = 0;
		state = STAND;
	}

	// 第三大地板上的水管2 左邊
	else if (walkingDistanceX > 10.814 && walkingDistanceX < 10.834 && walkingDistanceY >= 0 && walkingDistanceY < 0.53)
	{
		walkingDistanceX = 10.813;
		speed = 0;
		state = STAND;
	}

	// 第三大地板上的水管2 右邊
	else if (walkingDistanceX < 11.064 && walkingDistanceX> 11.044 &&  walkingDistanceY >= 0 && walkingDistanceY < 0.53)
	{
		walkingDistanceX = 11.0641;
		speed = 0;
		state = STAND;
	}

	// 第三大地板 右邊
	else if (walkingDistanceX < 12.3651 && walkingDistanceX> 12.3451 &&  walkingDistanceY < 0)
	{
		walkingDistanceX = 12.3652;
		speed = 0;
		state = STAND;
	}

	// 第四大地板 左邊
	else if (walkingDistanceX > 12.4981 && walkingDistanceX < 12.5181 && walkingDistanceY < 0)
	{
		walkingDistanceX = 12.498;
		speed = 0;
		state = STAND;
	}

	// 第四大地板 小水管 左邊
	else if (walkingDistanceX > 13.528 && walkingDistanceX < 13.548 && walkingDistanceY >= 0 && walkingDistanceY < 0.26)
	{
		walkingDistanceX = 13.527;
		speed = 0;
		state = STAND;
	}

	// 第四大地板 小水管 右邊
	else if (walkingDistanceX < 13.778 && walkingDistanceX> 13.758 &&  walkingDistanceY >= 0 && walkingDistanceY < 0.26)
	{
		walkingDistanceX = 13.779;
		speed = 0;
		state = STAND;
	}

	// 第四大地板 右邊
	else if (walkingDistanceX < 14.5501 && walkingDistanceX> 14.5301 &&  walkingDistanceY < 0)
	{
		walkingDistanceX = 14.5502;
		speed = 0;
		state = STAND;
	}

	// 第五大地板 左邊
	else if (walkingDistanceX > 14.7761 && walkingDistanceX < 14.7961 && walkingDistanceY < 0)
	{
		walkingDistanceX = 14.776;
		speed = 0;
		state = STAND;
	}

	// 第五大地板 右邊
	else if (walkingDistanceX < 15.0721 && walkingDistanceX> 15.0521 &&  walkingDistanceY < 0)
	{
		walkingDistanceX = 15.0722;
		speed = 0;
		state = STAND;
	}

	// 第六大地板 左邊
	else if (walkingDistanceX > 15.2181 && walkingDistanceX < 15.2381 && walkingDistanceY < 0)
	{
		walkingDistanceX = 15.218;
		speed = 0;
		state = STAND;
	}

	// 第六大地板 右邊
	else if (walkingDistanceX < 15.8001 && walkingDistanceX> 15.7801 &&  walkingDistanceY < 0)
	{
		walkingDistanceX = 15.8002;
		speed = 0;
		state = STAND;
	}
	// 第六大地板上的撞柱 左邊
	else if (walkingDistanceX > 15.8001 && walkingDistanceX < 15.8201 && walkingDistanceY >= 0 && walkingDistanceY < 0.4)
	{
		walkingDistanceX = 15.8;
		speed = 0;
		state = STAND;
	}

	// 第六大地板上的撞柱 右邊
	else if (walkingDistanceX < 15.9501 && walkingDistanceX> 15.9301 && walkingDistanceY < 0.4)
	{
		walkingDistanceX = 15.9511;
		speed = 0;
		state = STAND;
	}

	// 最終大地板 左邊
	else if (walkingDistanceX > 16.0841 && walkingDistanceX < 16.1041 && walkingDistanceY < 0)
	{
		walkingDistanceX = 16.084;
		speed = 0;
		state = STAND;
	}
	else if (LeftButtonDown == true || RightButtonDown == true)
	{
		state = WALKING;
	}
	//旗子
	if (walkingDistanceX >= 18.9462 && walkingDistanceX < 18.9662)
	{
		if (walkingDistanceY < 0.15)	//旗子下的小方塊
		{
			walkingDistanceX = 18.9461;
			speed = 0;
			state = STAND;
		}
		else
		{
			endGame = 1;
			LeftButtonDown = false;
			RightButtonDown = false;
			state = STAND;
			speed = 0;
			controlAble = 0;
		}

	}



	// 最右邊的牆壁
	if (walkingDistanceX > 20.072)
	{
		walkingDistanceX = 20.071;
		speed = 0;
		state = STAND;
	}
}


unsigned int DempApp::loadTexture(std::string path, int imageType)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, imageType, width, height, 0, imageType, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	return textureID;
}