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

void click_suninfo()
{//adb shell input tap 524 1645
	exec_cmd("adb shell input tap 524 1645");
	//screenshot();
}

void click_daka()
{//adb shell input tap 395 1356
	exec_cmd("adb shell input tap 395 1356");
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

void in_work()
{//adb shell input tap $(($rx+568)) 704
	exec_cmd("adb shell input tap 568 704");
}

void out_work()
{//adb shell input tap $(($rx+507)) 1154
	exec_cmd("adb shell input tap 507 1154");
}

void click_retern()
{
	exec_cmd("adb shell input keyevent 4");
	//screenshot();
}

void screenshot()
{
	exec_cmd("adb shell /system/bin/screencap -p /sdcard/screenshot.png");
	sleep(2);
	exec_cmd("adb pull /sdcard/screenshot.png ./screenshot.png");
	sleep(2);

	file_send(ding_tox, ding_friend_number, "./screenshot.png");
}
