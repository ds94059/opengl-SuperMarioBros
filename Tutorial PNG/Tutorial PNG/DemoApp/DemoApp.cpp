#include <windows.h>
#include "DemoApp.h"
#include "TextureApp.h"
#include "LoadShaders.h"

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

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "Mario.vs" },//vertex shader
		{ GL_FRAGMENT_SHADER, "Mario.fs" },//fragment shader
		{ GL_NONE, NULL } };
	program = LoadShaders(shaders);//讀取shader
	glUseProgram(program);//uniform參數數值前必須先use shader

}

void DempApp::Finalize()
{
	glDeleteTextures(1, &m_Coin);
}

float walkingDistanceX = 0, walkingDistanceY = 0, farestPos = 0, screenmiddle = 0, jumpGroundHeight=0;
int walkingState = 0, right = 1, state = STAND, pressTime = 0, speed = 0, risingSpeed = 5, falltimer = 0, timer = 0, inair = 0, floattimer = 0,endingstep=0,endtimer=0;
bool controlAble = 1,//1可控制 0不可
		endGame=0;
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
	if (endGame == 0)
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



		if ((haveGround(walkingDistanceX, walkingDistanceY) == 1) && inair != RISING)
		{
			inair = STANDING;
			jumpGroundHeight = walkingDistanceY;
		}
		else if (inair != RISING)
		{
			inair = FALLING;
		}

		if (inair == RISING)
		{
			if (UpButtonDown)
			{
				pressTime++;
				if (pressTime % 5 == 0)
					risingSpeed--;
				if (risingSpeed <= 2)
					risingSpeed = 2;

				walkingDistanceY += 0.02*risingSpeed;
			}

			if (walkingDistanceY > 0.6 + jumpGroundHeight)
			{
				inair = FLOATING;
			}
		}
		else if (inair == FLOATING)
		{
			floattimer++;
			if (floattimer > 5)
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
			/*if (walkingDistanceY < 0)
			{
				walkingDistanceY = 0;
				//inair = STANDING;
				risingSpeed = 5;
				falltimer = 0;
			}*/
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
			if (endtimer > 60 && endtimer<=90)
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
	if (controlAble)
	{

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
	if (key == KEY_ENTER)
	{
		//TextureApp::ScreenShot(std::string("ScreenShot"));
	}
	
}

void DempApp::KeyUp(int key)
{
	InitGlutInput::KeyUp(key);
	if (controlAble)
	{
		if (key == KEY_RIGHT)
		{
<<<<<<< HEAD
			RightButtonDown = false;
			state = STAND;
			//speed = 0;
			pressTime = 0;
		}		
	}
	if (key == KEY_LEFT)
	{
		if (right == 0)
=======
			if (right)
			{
				RightButtonDown = false;
				state = STAND;
				//speed = 0;
				pressTime = 0;
			}
		}
		if (key == KEY_LEFT)
>>>>>>> 72d9ed27cc74a69f162f4e130aadaa8c02ba51b7
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
			pressTime = 0;
<<<<<<< HEAD
		}		
	}
	if (key == KEY_UP)
	{
		UpButtonDown = false;
		pressTime = 0;
		if (inair == RISING)
			inair = FLOATING;
=======
			if (inair == RISING)
				inair = FLOATING;
		}
>>>>>>> 72d9ed27cc74a69f162f4e130aadaa8c02ba51b7
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
		glTexCoord2d(0.458333, 0.91098); glVertex2d(-0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
		glTexCoord2d(0.497395, 0.91098); glVertex2d(0.05 + walkingDistanceX, -0.05 + walkingDistanceY);
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



bool haveGround(float x, float y)
{
	// 第一大地板上的水管
	if (y<=0.41 && x>= 1.102 && x<= 1.35)
	{
		walkingDistanceY = 0.41;
		risingSpeed = 1;
		falltimer = 0;
		return 1;
	}
	// 第一大地板
	else if (y <= 0 && x >= -0.667&&x <= 2.123)
	{
		walkingDistanceY = 0;
		risingSpeed = 1;
		falltimer = 0;
		return 1;
	}
	// 第二大地板
	else if (y <= 0 && x >= 2.255&&x <= 6.149)
	{
		walkingDistanceY = 0;
		risingSpeed = 1;
		falltimer = 0;
		return 1;
	}
	// 第三大地板
	else if (y <= 0 && x >= 6.463&&x <= 12.3651)
	{
		walkingDistanceY = 0;
		risingSpeed = 1;
		falltimer = 0;
		return 1;
	}
	// 第四大地板
	else if (y <= 0 && x >= 12.4981 &&x <= 14.5501)
	{
		walkingDistanceY = 0;
		risingSpeed = 1;
		falltimer = 0;
		return 1;
	}
	// 第五大地板
	else if (y <= 0 && x >= 14.7761&&x <= 15.0721)
	{
		walkingDistanceY = 0;
		risingSpeed = 1;
		falltimer = 0;
		return 1;
	}
	// 第六大地板
	else if (y <= 0 && x >= 15.2181&&x <= 15.8001)
	{
		walkingDistanceY = 0;
		risingSpeed = 1;
		falltimer = 0;
		return 1;
	}
	// 最終大地板
	else if (y <= 0 && x >= 16.0841)
	{
		walkingDistanceY = 0;
		risingSpeed = 1;
		falltimer = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}