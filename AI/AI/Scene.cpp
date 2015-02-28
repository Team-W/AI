#include "Scene.h"

Scene::Scene(void)
{
	view_matrix = glm::lookAt(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
	memset(&KeyStates, 0, sizeof(KeyStates));
	// Init Player
	this->player = new Player(this, 0.0f, 0.0f);
	this->debug = false;
	this->game = true;
	this->score = 0;

	// Init Objects	
	AddObstacle(new Obstacle(this, 15.0, 15.0, 7.00));
	AddObstacle(new Obstacle(this, -12.0, -17.0, 9.00));
	AddObstacle(new Obstacle(this, 12.0, -6.0, 5.00));
	AddObstacle(new Obstacle(this, -16.0, 15.0, 6.00));

	for (unsigned int i = 0; i < 10; ++i) AddZombie(new Zombie(this));

}

Scene::~Scene(void)
{
	if (player != 0) delete player;
	//if (test_zombie != 0) delete test_zombie;
	for (int i = 0; i < objects.size(); i++){
		if (objects[i] != 0) delete objects[i];
	}

}

void Scene::PrintResult()
{
	cout << "---------- RESULT ----------" << endl;
	cout << "Score: " << player->score << endl << endl;
	cout << "Type [R] to restart game" << endl << endl;
}

void RenderBitmapString(float x, float y, char *string)
{
	char *c;
	glColor3f(1, 0, 0);
	glRasterPos3f(x, y, -0.1);
	for(c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

float GlutPrint(float x, float y, char *txt, float scale)
{
	int i;
	int len=strlen(txt);
	
	for(i=0; i<len; i++)
	{
		glRasterPos2f(x, y);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, txt[i]);
		x += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, txt[i])*scale;
	}

	return 0;
}

void PrintText(char* string, int x, int y) {

	if(string && strlen(string))
	{
		glPushMatrix();
		glLoadIdentity();
		glScalef(0.003f, 0.003f,0.003f);
		glColor3f(1, 0, 0);
		glRasterPos3d(x, y, 0.1);
		while(*string)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string);
			string++;
		}
		glPopMatrix();
	}

}

void Scene::Update(double delta_time)
{
	if (this->game){
		CheckVictoryCondition();
		for (unsigned int i = 0; i < objects.size(); i++)
		{
			objects[i]->Update(delta_time);
		}

		if (KeyStates['w'] || KeyStates['W']) player->Move(glm::vec2(0, 0.3), delta_time);
		if (KeyStates['s'] || KeyStates['S']) player->Move(glm::vec2(0, -0.3), delta_time);
		if (KeyStates['a'] || KeyStates['A']) player->Move(glm::vec2(-0.3, 0), delta_time);
		if (KeyStates['d'] || KeyStates['D']) player->Move(glm::vec2(0.3, 0), delta_time);

		player->Update(delta_time);
	}
	if ((KeyStates['r'] || KeyStates['R']) && !game) Restart();
	if (KeyStates[27]) exit(0); //ESC key quits the game
}

bool Scene::CheckVictoryCondition(void)
{
	for (unsigned int i = 0; i<zombies.size(); ++i)
	{
		if (GetDistance(player->GetObjectPosition(), zombies[i]->GetObjectPosition()) < 2)
		{
			this->game = false;
			PrintResult();
		}
	}

	return game;
}

void Scene::Restart(void){
	this->game = true;
	this->debug = true;
	for (unsigned int i = 0; i < zombies.size(); i++)
	{
		zombies[i]->RandomPosition();	
		zombies[i]->aggressive = false;
	}
	this->debug = false;
	player->Reset();
}

void Scene::Draw(void)
{
	player->Draw();
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->Draw();
	}
}

void Scene::PlayerRotate(glm::vec2 heading)
{
	player->Rotate(heading);
	
}

void Scene::PlayerShoot(glm::vec2 aim)
{
	player->Shoot(aim);
}

void Scene::KeyState(unsigned char key, bool tf)
{
	KeyStates[key] = tf;
}

void Scene::AddObject(GameEntity *entity)
{
	objects.push_back(entity);
}
void Scene::AddZombie(Zombie *entity)
{
	zombies.push_back(entity);
	objects.push_back(entity);
}
void Scene::AddObstacle(Obstacle *entity)
{
	obstacles.push_back(entity);
	objects.push_back(entity);
}

ostream& operator<<(ostream &o, const Scene &gw)
{
	o << "---------------- SCENE ---------------" << "\n";

	o << "--------------------------------------" << endl;
	return o;
}
