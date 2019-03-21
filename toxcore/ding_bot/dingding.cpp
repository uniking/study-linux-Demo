#include "dingding.hpp"
#include <tox/tox.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctime>
#include <locale>
#include <time.h>

#include <string>
using namespace std;

extern void file_send(Tox *tox, uint32_t friend_number, const char *filename);

Tox *ding_tox;
uint32_t ding_friend_number;

void exec_cmd(const char* cmd)
{
	printf("exec_cmd:%s\n", cmd);
	FILE* process_file  = popen(cmd, "r");
	if(process_file == NULL)
	{
		return;
	}

	char buf[256];
	string msg;
	while(fgets(buf, 255, process_file) != NULL)
	{
		//printf("wget info: %s\n", buf);
		msg += buf;
	}

	pclose(process_file);

}

void power_butten()
{//adb shell input keyevent 26
	exec_cmd("adb shell input keyevent 26");
	//screenshot();
}

void start_dingding()
{//adb shell am start -n com.alibaba.android.rimet/com.alibaba.android.rimet.biz.SplashActivity
	exec_cmd("adb shell am start -n com.alibaba.android.rimet/com.alibaba.android.rimet.biz.SplashActivity");
	//screenshot();
}

void click_suninfo(LOC_X_Y* l)
{//adb shell input tap 524 1645
	exec_cmd("adb shell input tap 524 1645");
	l->x=524;
	l->y=1645;
	//screenshot();
}

void click_daka(LOC_X_Y* l)
{//adb shell input tap 395 1356
	exec_cmd("adb shell input tap 400 921");
	l->x=400;
	l->y=921;
	//screenshot();
}

void click_daka2(LOC_X_Y* l)
{//adb shell input tap 395 1356
	exec_cmd("adb shell input tap 400 1260");
	l->x=400;
	l->y=1260;
	//screenshot();
}

void click_ram(int x, int y)
{//adb shell input tap 395 1356
	char cmd[128];
	sprintf(cmd, "adb shell input tap %d %d", x, y);
	exec_cmd(cmd);
	//screenshot();
}

void move_down_screen()
{//adb shell input swipe 520 1207 535 562
	exec_cmd("adb shell input swipe 535 562 520 1207");
	//screenshot();
}

void move_up_screen()
{//adb shell input swipe 520 1207 535 562
	exec_cmd("adb shell input swipe 520 1207 535 562");
	//screenshot();
}

//用于探测x, y的真实位置
void move_right_screen(int x, int y)
{//adb shell input swipe 520 1207 535 562
	char cmd[128];
	sprintf(cmd, "adb shell input swipe %d %d %d %d", x, y, x+200, y);
	exec_cmd(cmd);
	//screenshot();
}

void in_work(LOC_X_Y* l)
{//adb shell input tap $(($rx+568)) 704
	exec_cmd("adb shell input tap 568 704");
	l->x=568;
	l->y=704;
}

void out_work(LOC_X_Y* l)
{//adb shell input tap $(($rx+507)) 1154
	exec_cmd("adb shell input tap 507 1154");
	l->x=507;
	l->y=1154;
}

void click_retern()
{
	exec_cmd("adb shell input keyevent 4");
	//screenshot();
}

void new_image_name(char* name, int length)
{
	struct tm* t;
	time_t timer1;

	timer1 = time(NULL);
	t = localtime(&timer1);
	snprintf(name, length, "screan.%02d.%02d.%02d.%02d.png", t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
}

void screenshot()
{
	exec_cmd("adb shell /system/bin/screencap -p /sdcard/screenshot.png");
	exec_cmd("rm ./screenshot.png");
	sleep(1);
	exec_cmd("adb pull /sdcard/screenshot.png ./screenshot.png");
	sleep(1);

	char imagename[64];
	new_image_name(imagename, 64);

	char cmd[128];
	snprintf(cmd, 128, "convert -sample 25%%x25%% ./screenshot.png ./%s", imagename);
	exec_cmd(cmd);

	char path[64];
	snprintf(path, 64, "./%s", imagename);
	file_send(ding_tox, ding_friend_number, path);
}
