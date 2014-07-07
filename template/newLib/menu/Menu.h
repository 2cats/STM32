#ifndef MENU_h
#define MENU_h
#include <stm32f10x.h>
#include <string>
#include <vector>
using namespace std;
#define KEY_NULL		0
#define KEY_UP 			1
#define KEY_DOWN		2
class Drawer
{
public :
		Drawer();
		void gotoXY(int p_x,int p_y);
		void printString(char const*str);
		void move2NextLine();
//		void CEOL();
		void printStringCEOL(char const *str);	
//		void printChar(char c);
//		void highlightY(int p_y);
};
class Menu
{
public:
	Menu();
	vector<Menu>subMenus;
	Menu * parent;
	Menu * currMenu;

	string label;
	u8 id;
	Menu* addMenu(Menu*parent,string label,void(*cb)(void)=0);
};
class MenuWidget
{
public:
	MenuWidget();
	u8 topId;
	Drawer drawer;
	void exec();
	Menu* setRoot(Menu *_root);
	int latestedKey();
	void setKeyDevice(int (*keyscan)(void));
private:
	void draw(Menu *top);
	int p_x,p_y,width,height;
	Menu *root;
	Menu *currMenu;
};


#endif


