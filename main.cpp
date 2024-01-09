//-static-libgcc -lSDL2 -lSDL2_image
/*************************************************
 * 
 * Project:Touhou-like Danmaku Game
 *  maker:qytlix
 *  begining time: 2023/12
 *  version  time: 2024/1/7
 *  update describe:
 *   [2024/1/7] Add Power mode
 *   [2024/1/7] Change struct to class.(OOP!!!)
 * 
*************************************************/
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define nullptr NULL
using namespace std;
/*vars*/
// window size
#define WINDOW_HEIGHT 960
#define WINDOW_WIDTH  1280

// map edges
#define UPLINE 92
#define DOWNLINE 880
#define LEFTLINE 88
#define RIGHTLINE 708

#define SELFBULLETMAX 50 // bullets from selfPlane max number
#define BULLETMAX 2000
#define POWERSMAX 100
// sdl vars
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *Background = nullptr;
SDL_Texture *bullet_a = nullptr;
SDL_Texture *background = nullptr;
SDL_Texture *plane = nullptr;
SDL_Texture *testEnemypic = nullptr;
SDL_Texture *PowerPic = nullptr;

const Uint8* state;//keyboard event read

unsigned int power;
bool quit = false;//main quit
unsigned int preTick;//which tick is now's tick
struct S_Box{double x,y,a;};// Sqare hitBox
struct point{double x,y;};
struct Box{
	point pos;//centre position for a box
	double a;
};
/*--------*/

/*function declarations*/
bool initSDL();// init SDL including SDL_VEDIO, IMG_PNG, Window and icon
void closeSDL();// release SDL
SDL_Texture * getimage(const char *);// load a image(png)
void putimage(SDL_Texture *, int, int, int = 0, int = 0);// put image to randerer
void putimage(SDL_Texture *, Box);
void KBReflection();// check KB event and update place of SelfPlane
bool checkHit(Box,Box);// check 2 box if cross
void readEnemyBullets(const char *);
void drawBox(Box);
point operator+(point,point);
point operator-(point,point);
void throwPower(double,double);
/*--------*/
class flys{
	protected:
		Box hitbox,imgbox;// hitbox and image's box
		point v;//speed : vector
		SDL_Texture *pic;// Texture
		bool exist;// if exist
	public:
		void init(SDL_Texture *_pic, double x, double y, double hitsize, double imgsize, double dx = 0, double dy = 0) {
			exist = 1;
			pic = _pic;

			hitbox.pos.x = x;
			hitbox.pos.y = y;
			hitbox.a = hitsize;

			imgbox.pos.x = x;
			imgbox.pos.y = y;
			imgbox.a = imgsize;

			v.x = dx;
			v.y = dy;
		}
		void putPic() {
			putimage(this->pic,imgbox);
		}
		void update() {
			hitbox.pos = v + hitbox.pos;
			imgbox.pos = v + imgbox.pos;
		}
		void die() {
			exist = 0;
		}
		bool ifexist() { // check it if exist
			return exist;
		}
		Box getHitbox() {
			return hitbox;
		}
		Box getImgbox() {
			return imgbox;
		}
		bool check() {
			if (hitbox.pos.x <= LEFTLINE - 100 or hitbox.pos.x >= RIGHTLINE + 100) return 0;
			if (hitbox.pos.y <= UPLINE - 100   or hitbox.pos.y >= DOWNLINE + 100)  return 0;
			return 1;
		}
};
class Bullet : public flys{
	public:
		void init(int choosepic, double x, double y, double hitsize, double imgsize, double dx, double dy) {
			exist = 1;
			//set pic
			switch (choosepic) {
				case 1://bullet_a
					pic = bullet_a;
					break;
				default:
					break;
			}

			hitbox.pos.x = x;
			hitbox.pos.y = y;
			hitbox.a = hitsize;

			imgbox.pos.x = x;
			imgbox.pos.y = y;
			imgbox.a = imgsize;

			v.x = dx;
			v.y = dy;
		}
}bullets[SELFBULLETMAX],enemybullets[BULLETMAX];
class Enemys : public flys{
	private:
		int hits;
		unsigned int step;
	public:
		void init(int choosepic, double x, double y, double hitsize, double imgsize, double dx, double dy, int _hits) {
			exist = 1;
			//set pic
			switch (choosepic) {
				case 1:// testpic
					pic = testEnemypic;
					break;
				default:
					break;
			}

			hitbox.pos.x = x;
			hitbox.pos.y = y;
			hitbox.a = hitsize;

			imgbox.pos.x = x;
			imgbox.pos.y = y;
			imgbox.a = imgsize;

			v.x = dx;
			v.y = dy;

			hits = _hits;
		}
		void update(){
			flys::update();
			step++;
		}
		void getHit(){
			hits --;
			if (hits == 0)
			{
				die();
				throwPower(hitbox.pos.x,hitbox.pos.y);
			}
		}
		int getstep(){
			return step;
		}
		void throwBullet_followselfplane(double bspeed);
}testEnemy;
class Drops : public flys{
	private:
		int type;//0=power,1=dian
		point a;//accelerate
	public:
		int getType() {
			return type;
		}
		void init(int type, double x, double y){
			exist = 1;
			v.y = -7;

			switch (type)
			{
				case 0:
					pic = PowerPic;
					break;
				
				default:
					break;
			}

			hitbox.pos.x = x;
			hitbox.pos.y = y;
			hitbox.a = 15;//const size for Drops

			imgbox.pos.x = x;
			imgbox.pos.y = y;
			imgbox.a = 15;
		}
		void update(){
			flys::update();
			if (v.y <= 3)v.y += 0.25;//accele Y
			else v.y = 3;
			if (v.x >= 0)v.x -= 0.25;//accele X
			else v.x = 0;
		}

}powers[POWERSMAX];
struct Timer {
	unsigned int last,tick;
	Timer (int _tick){
		last = 0;
		tick = _tick;
	}
	bool check() { //check and update
		unsigned int now = SDL_GetTicks();
		if (now > last + tick) {
			last = now;
			return 1;
		}
		return 0;
	}
	bool ocheck() {// only check
		unsigned int now = SDL_GetTicks();
		if (now > last + tick) {
			return 1;
		}
		return 0;
	}
	void update() {
		unsigned int now = SDL_GetTicks();
		last = now;
	}
};
//selfPlane move data
double v=8;
double rx,ry,lx,ly;
Box selfBox = {200,200,5};
#undef main
int main(){
	if(!initSDL())return -1;
	SDL_Event event;

	Timer ScreenTimer = Timer(15);
//	Timer SelfPlaneAnime = Timer(700);
	while(!quit) {
		//wheather quit
		if (ScreenTimer.check()) {
			preTick++;
			//update screen
			SDL_RenderPresent(renderer);
			//ready for next tick fresh
			//cls
			SDL_RenderClear(renderer);
			//throw powers
			for (int i = 0; i < POWERSMAX; i ++)
			{
				if (powers[i].ifexist())
				{
					powers[i].update();
					powers[i].putPic();
					if (!powers[i].check())
					{
						powers[i].die();
						continue;
					}
					if (checkHit(powers[i].getHitbox(),selfBox))
					{
						powers[i].die();
						//TODO
						power ++;
						continue;
					}
					drawBox(powers[i].getHitbox());
				}
			}
			//put bullet from selfPlane
			for (int i = 0; i < SELFBULLETMAX; i ++)
			{
				if (bullets[i].ifexist()) {
					bullets[i].update();
					bullets[i].putPic();
					// putimage (bullet_a, bullets[i].getHitbox());
					if (!bullets[i].check())
					{
						bullets[i].die();
						continue;
					}
					//TODO : for testEnemy 
					if (testEnemy.ifexist() and checkHit(bullets[i].getHitbox(),testEnemy.getHitbox())){
						testEnemy.getHit();//be hitted and check if die
						bullets[i].die();// clear the hitting bullet
						continue;
					}
					drawBox(bullets[i].getHitbox());
				}
			}
			
			if (testEnemy.ifexist())
			{
				//move exemy
				testEnemy.update();
				if (!testEnemy.check()){
					testEnemy.die();
				}
				if (testEnemy.ifexist() and !(testEnemy.getstep() % 15))testEnemy.throwBullet_followselfplane(5);
				//put enemy
				if (testEnemy.ifexist()) putimage(testEnemypic, testEnemy.getImgbox());
				drawBox(testEnemy.getHitbox());
			}	
			//put enemy's bullets
			for (int i = 0; i < BULLETMAX; i ++)
			{
				if (enemybullets[i].ifexist()) {
					enemybullets[i].update();
					enemybullets[i].putPic();
					// putimage (bullet_a, enemybullets[i].getImgbox());
					if (!enemybullets[i].check()){
						enemybullets[i].die();
						continue;
					}
					if (enemybullets[i].ifexist() and checkHit(enemybullets[i].getHitbox(),selfBox)){
						cout << "Hit!\n";
						enemybullets[i].die();
						continue;
					}
					drawBox(enemybullets[i].getHitbox());
				}
			}
			
			//put selfplane
			
//			cout <<x <<' ' << y<<endl;
//			cout <<"fps:"<< 1000/(now-lastTime) << endl;
			int nexx = selfBox.pos.x - lx * v + rx * v, nexy = selfBox.pos.y - ly * v + ry * v;
			lx = 0;ly = 0;
			rx = 0; ry = 0;
			if (nexx >= LEFTLINE and nexx <= RIGHTLINE)selfBox.pos.x = nexx;
			else if (nexx <= LEFTLINE)selfBox.pos.x = LEFTLINE;
			else if (nexx >= RIGHTLINE)selfBox.pos.x = RIGHTLINE;
			
			if (nexy >= UPLINE and nexy <= DOWNLINE)selfBox.pos.y = nexy;
			else if (nexy <= UPLINE)selfBox.pos.y = UPLINE;
			else if (nexy >= DOWNLINE)selfBox.pos.y = DOWNLINE;
			putimage(plane,selfBox);
			drawBox(selfBox);
			//put background
			putimage(background, 0, 0);
		}
		while (SDL_PollEvent(&event) > 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
				break;
			}
		}
		KBReflection();
	}
	closeSDL();
}

//Functions
bool initSDL() {
	if (IMG_Init(IMG_INIT_PNG) == -1) {
		printf("[ERROR] Init Image_PNG : %s\n",SDL_GetError());
		return 0;
	}
	//init the window
	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("[ERROR] Init SDL : %s\n",SDL_GetError());
		return 0;
	}
	//create a window
	window = SDL_CreateWindow( "TouHou : World Link Century", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		printf("[ERROR] Create Window : %s\n",SDL_GetError());
		return 0;
	}
	//set icon
	SDL_Surface *_icon= IMG_Load("./image/icon.png");
	if (_icon == nullptr) {
		printf("[ERROR] Image_jpg : %s\n",SDL_GetError());
		return 0;
	}
	SDL_SetWindowIcon(window, _icon);
	SDL_FreeSurface(_icon);
	_icon = nullptr;
	//create a renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		printf("[ERROR] Create Renderer : %s\n",SDL_GetError());
		return 0;
	}

	//Textures Load
	bullet_a = getimage("./image/bullet_a.png");
	background = getimage("./image/background.png");
	plane = getimage("./image/plane.png");
	testEnemypic = getimage("./image/enemy.png");
	PowerPic = getimage("./image/power.png");
	return 1;
}
void closeSDL() {
	SDL_DestroyTexture(plane);
	SDL_DestroyTexture(testEnemypic);
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(bullet_a);
	SDL_DestroyTexture(PowerPic);

	IMG_Quit();
	
	SDL_DestroyWindow( window );
	window = nullptr;
	
	SDL_Quit();
}
SDL_Texture * getimage(const char *path) {
	SDL_Surface *image = IMG_Load(path);
	if (image == NULL) {
		printf("IMG_Load <%s> failed\n" ,path);
		return nullptr;
	}
	SDL_Texture *back = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);
	return back;
}
void putimage(SDL_Texture *texture, int x, int y, int textureW, int textureH) {
	if (textureW == 0 and textureH == 0)SDL_QueryTexture(texture ,NULL ,NULL ,&textureW, &textureH);
	else if (textureW == 0) SDL_QueryTexture(texture ,NULL ,NULL ,&textureW, NULL);
	else if (textureH == 0) SDL_QueryTexture(texture ,NULL ,NULL ,NULL , &textureH);
	SDL_Rect im_place = {x, y, textureW, textureH};
	SDL_RenderCopy(renderer, texture, nullptr, &im_place);
}
void putimage(SDL_Texture *texture, Box box){
	SDL_Rect im_place = {(int)(box.pos.x - box.a), (int)(box.pos.y - box.a), 2*(int)box.a, 2*(int)box.a};
	SDL_RenderCopy(renderer, texture, nullptr, &im_place);
}
int bulAddPoint;
Timer ShootTimer = Timer(15);
Timer switchTimer = Timer(150);
bool drawBoxon;
void KBReflection() {
	state = SDL_GetKeyboardState(NULL);
	if(state[SDL_SCANCODE_X])quit = 1;
	if(state[SDL_SCANCODE_LEFT])lx = 1;
	if(state[SDL_SCANCODE_RIGHT])rx = 1;
	if(state[SDL_SCANCODE_UP])ly = 1;
	if(state[SDL_SCANCODE_DOWN])ry = 1;
	if(state[SDL_SCANCODE_LSHIFT])v = 4;
	else v = 8;
	if(ShootTimer.check()) {
		if(state[SDL_SCANCODE_Z]) {
			int ori = bulAddPoint;// original place
			while(bullets[bulAddPoint].ifexist()){
				bulAddPoint = (bulAddPoint + 1) % SELFBULLETMAX;
				if (ori == bulAddPoint)
				{
					//cout<<"Space G\n";
					return;
				}
			}
			bullets[bulAddPoint].init(1, selfBox.pos.x, selfBox.pos.y + 20, 8, 10, 0, -20);
		}
		if(state[SDL_SCANCODE_A]) {
			testEnemy.init(1,LEFTLINE+1,200,20,20,3,0,50);
		}
	}
	if (switchTimer.ocheck()) {
		if(state[SDL_SCANCODE_S]) {
			drawBoxon = !drawBoxon;
			switchTimer.update();
		}
	}
}
int enemybulletAddPoint;
void Enemys :: throwBullet_followselfplane (double bspeed) {
	double toX = selfBox.pos.x - hitbox.pos.x,toY = selfBox.pos.y - hitbox.pos.y;
	double bLength = sqrt(pow(toX,2) + pow(toY,2));
	toX *= bspeed;
	toY *= bspeed;
	toX /= bLength;
	toY /= bLength;
	
	int ori = enemybulletAddPoint;// original place
	while(enemybullets[enemybulletAddPoint].ifexist()){
		enemybulletAddPoint = (enemybulletAddPoint + 1) % BULLETMAX;
		if (ori == enemybulletAddPoint)
		{
			//cout<<"Space G\n";
			return;
		}
	}
	enemybullets[enemybulletAddPoint].init(1, hitbox.pos.x, hitbox.pos.y,10,20, toX, toY);
}
bool checkHit(Box A,Box B) {
	if (abs(A.pos.x-B.pos.x)>A.a+B.a)return 0;
	if (abs(A.pos.y-B.pos.y)>A.a+B.a)return 0;
	return 1;
}
void drawBox(Box A){
	if (!drawBoxon)return;
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_Rect rect1 = { (int)(A.pos.x - A.a), (int)(A.pos.y - A.a), (int)(2*A.a), (int)(2*A.a)};
	SDL_RenderDrawRect(renderer, &rect1);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}
point operator+(point a,point b){
	point ans;
	ans.x = a.x + b.x;
	ans.y = a.y + b.y;
	return ans;
}
point operator-(point a,point b){
	point ans;
	ans.x = a.x - b.x;
	ans.y = a.y - b.y;
	return ans;
}
int powerAddPoint;
void throwPower(double x,double y){
	int ori = powerAddPoint;// original place
	while(powers[powerAddPoint].ifexist()){
		powerAddPoint = (powerAddPoint + 1) % SELFBULLETMAX;
		if (ori == powerAddPoint)
		{
			//cout<<"Space G\n";
			return;
		}
	}
	powers[powerAddPoint].init(0,x,y);
}