#include "menu/Menu.h"

void MenuWidget::draw(Menu *top)
{
	vector<Menu>::iterator it;
	int i;
	drawer.gotoXY(p_x,p_y);
	for(i=0,it=top->subMenus.begin();it!=top->subMenus.end();it++,i++)
	{
			drawer.printStringCEOL((*it).label.data());
			drawer.move2NextLine();
	}
}
void MenuWidget::exec()
{
	int key=latestedKey();
	if(key!=KEY_NULL)
	{
		switch(key)
		{
			case KEY_UP:
				
				break;
			case KEY_DOWN:
				break;
		}
	}else
	{
		return ;
	}
}
