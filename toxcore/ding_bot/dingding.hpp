#include <tox/tox.h>
#include <unistd.h>

typedef struct loc_x_y {
	int x;
	int y;
} LOC_X_Y;

void power_butten();
void start_dingding();
void click_suninfo(LOC_X_Y* l);
void click_daka(LOC_X_Y* l);
void click_daka2(LOC_X_Y* l);//出现 "待我审批"， “出勤天数”，“请假”的条目时
void move_down_screen();
void move_up_screen();
void move_right_screen(int x, int y);
void screenshot();
void click_retern();
void click_ram(int x, int y);

void in_work(LOC_X_Y* l);
void out_work(LOC_X_Y* l);
