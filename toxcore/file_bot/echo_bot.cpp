#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sodium/utils.h>
#include <tox/tox.h>

#include <string>
using namespace std;

typedef struct DHT_node {
    const char *ip;
    uint16_t port;
    const char key_hex[TOX_PUBLIC_KEY_SIZE*2 + 1];
    unsigned char key_bin[TOX_PUBLIC_KEY_SIZE];
} DHT_node;

const char *savedata_filename = "savedata.tox";
const char *savedata_tmp_filename = "savedata.tox.tmp";

extern void printQrCode(const char* str);

#define NUM_FILE_SENDERS 256
typedef struct {
    FILE *file;
    uint32_t friendnum;
    uint32_t filenumber;
} File_t;
File_t file_senders[NUM_FILE_SENDERS];
File_t file_recv[NUM_FILE_SENDERS];
uint8_t numfilesenders;

extern void wget(Tox *tox, uint32_t friend_number, char* url);
void cmd(Tox *tox, uint32_t friend_number, const char* cmd);

void getpathcontent(const char* path, char* content)
{
	char cmd[1024];
	snprintf(cmd, 1024, "/usr/bin/ls %s", path);
	printf("get file list:%s\n", cmd);
	FILE* process_file  = popen(cmd, "r");
	//* process_file  = popen("/usr/bin/top -n 1 | wc", "r");
	if(process_file == NULL)
	{
		return;
	}

	char buf[256];
	string msg;
	while(fgets(buf, 255, process_file) != NULL)
	{
		msg += buf;
	}

	strncpy(content, msg.c_str(), 2048);

	pclose(process_file);
}

int get_file_size(const char* filename)  
{  
    struct stat statbuf;  
    stat(filename,&statbuf);  
    int size=statbuf.st_size;  
  
    return size;  
}  

uint32_t add_filesender(Tox *m, uint16_t friendnum, char *filename)
{
    FILE *tempfile = fopen(filename, "rb");
    char fn[256];

    char* pfn = strrchr(filename, '/');
    if(pfn == NULL)
    {
        printf("%s not find '/'\n", filename);
	return 0;
    }
    pfn++;

    strncpy(fn, pfn, 256);

    if (tempfile == 0)
        return -1;

    fseek(tempfile, 0, SEEK_END);
    uint64_t filesize = ftell(tempfile);
    fseek(tempfile, 0, SEEK_SET);
    uint32_t filenum = tox_file_send(m, friendnum, TOX_FILE_KIND_DATA, filesize, 0, (uint8_t *)fn,
                                     strlen(fn), 0);

    if (filenum == -1)
        return -1;

    file_senders[numfilesenders].file = tempfile;
    file_senders[numfilesenders].friendnum = friendnum;
    file_senders[numfilesenders].filenumber = filenum;
    ++numfilesenders;
    return filenum;
}

void file_send(Tox *tox, uint32_t friend_number, char *filename)
{
    int file_size = get_file_size((const char*)filename);
    printf("file send: filename:%s filename size:%d file_size:%d\n", (const char*)filename, strlen((const char*)filename), file_size);
    add_filesender(tox, friend_number, filename);
}

void tox_file_chunk_request(Tox *tox, uint32_t friend_number, uint32_t file_number, uint64_t position, size_t length,
                            void *user_data)
{
    unsigned int i;
    //printf("tox_file_chunk_request position:%d length:%d\n", position, length);
    for (i = 0; i < NUM_FILE_SENDERS; ++i) {
        /* This is slow */
        if (file_senders[i].file && file_senders[i].friendnum == friend_number && file_senders[i].filenumber == file_number) {
            if (length == 0) {
                fclose(file_senders[i].file);
                file_senders[i].file = 0;
                printf("[t] %u file transfer: %u completed\n", file_senders[i].friendnum, file_senders[i].filenumber);
                break;
            }

            fseek(file_senders[i].file, position, SEEK_SET);
            uint8_t data[length];
            int len = fread(data, 1, length, file_senders[i].file);
            tox_file_send_chunk(tox, friend_number, file_number, position, data, len, 0);
            break;
        }
    }
}



Tox *create_tox()
{
    Tox *tox;

    struct Tox_Options options;

    tox_options_default(&options);

    FILE *f = fopen(savedata_filename, "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char *savedata = (char*)malloc(fsize);

        fread(savedata, fsize, 1, f);
        fclose(f);

        options.savedata_type = TOX_SAVEDATA_TYPE_TOX_SAVE;
        options.savedata_data = (const uint8_t*)savedata;
        options.savedata_length = fsize;

        tox = tox_new(&options, NULL);

        free(savedata);
    } else {
        tox = tox_new(&options, NULL);
    }

    return tox;
}

void update_savedata_file(const Tox *tox)
{
    size_t size = tox_get_savedata_size(tox);
    char *savedata = (char*)malloc(size);
    tox_get_savedata(tox, (uint8_t*)savedata);

    FILE *f = fopen(savedata_tmp_filename, "wb");
    fwrite(savedata, size, 1, f);
    fclose(f);

    rename(savedata_tmp_filename, savedata_filename);

    free(savedata);
}

void bootstrap(Tox *tox)
{
    DHT_node nodes[] =
    {
	{"111.231.81.34",		33445, "D365C98AAEBE77238535E2C0E7C667A5760B3FD929157BF85CADE05C342BB072", {0}},
        {"178.62.250.138",             33445, "788236D34978D1D5BD822F0A5BEBD2C53C64CC31CD3149350EE27D4D9A2F9B6B", {0}},
        {"2a03:b0c0:2:d0::16:1",       33445, "788236D34978D1D5BD822F0A5BEBD2C53C64CC31CD3149350EE27D4D9A2F9B6B", {0}},
        {"tox.zodiaclabs.org",         33445, "A09162D68618E742FFBCA1C2C70385E6679604B2D80EA6E84AD0996A1AC8A074", {0}},
        {"163.172.136.118",            33445, "2C289F9F37C20D09DA83565588BF496FAB3764853FA38141817A72E3F18ACA0B", {0}},
        {"2001:bc8:4400:2100::1c:50f", 33445, "2C289F9F37C20D09DA83565588BF496FAB3764853FA38141817A72E3F18ACA0B", {0}},
        {"128.199.199.197",            33445, "B05C8869DBB4EDDD308F43C1A974A20A725A36EACCA123862FDE9945BF9D3E09", {0}},
        {"2400:6180:0:d0::17a:a001",   33445, "B05C8869DBB4EDDD308F43C1A974A20A725A36EACCA123862FDE9945BF9D3E09", {0}},
        {"node.tox.biribiri.org",      33445, "F404ABAA1C99A9D37D61AB54898F56793E1DEF8BD46B1038B9D822E8460FAB67", {0}}
    };

    for (size_t i = 0; i < sizeof(nodes)/sizeof(DHT_node); i ++) {
        sodium_hex2bin(nodes[i].key_bin, sizeof(nodes[i].key_bin),
                       nodes[i].key_hex, sizeof(nodes[i].key_hex)-1, NULL, NULL, NULL);
        tox_bootstrap(tox, nodes[i].ip, nodes[i].port, nodes[i].key_bin, NULL);
    }
}

void print_tox_id(Tox *tox)
{
    uint8_t tox_id_bin[TOX_ADDRESS_SIZE];
    tox_self_get_address(tox, tox_id_bin);

    char tox_id_hex[TOX_ADDRESS_SIZE*2 + 1];
    sodium_bin2hex(tox_id_hex, sizeof(tox_id_hex), tox_id_bin, sizeof(tox_id_bin));

    for (size_t i = 0; i < sizeof(tox_id_hex)-1; i ++) {
        tox_id_hex[i] = toupper(tox_id_hex[i]);
    }

    printf("Tox ID: %s\n", tox_id_hex);
    printQrCode(tox_id_hex);
}

void friend_request_cb(Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length,
                                   void *user_data)
{
    printf("add friend %s\n", (const char*)public_key);
    tox_friend_add_norequest(tox, public_key, NULL);

    update_savedata_file(tox);
}

void friend_message_cb(Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message,
                                   size_t length, void *user_data)
{
    printf("get msg: %s\n", (const char*)message);
    if(0 == strncmp((const char*)message, "help", 4))
    {
        char help[1024];
        sprintf(help, "1, pwd\n2, ls dir\n3, get filepath\n4, put filepath\n5, wget url\n");
        tox_friend_send_message(tox, friend_number, type, (const uint8_t*)help, strlen(help), NULL);
        return;
    }
    else if(0 == memcmp(message, "ls", 2))
    {
/*
        char path[1024]={0};
	char content[2048]={0};

	char* pSpace = strchr((char*)message, ' ');
	if(pSpace == NULL)
        {
            tox_friend_send_message(tox, friend_number, type, (const uint8_t*)"ls /", 4, NULL);
            return;
        }

	pSpace++;
	strncpy(path, pSpace, 1023);
	
        getpathcontent(path, content);
        tox_friend_send_message(tox, friend_number, type, (const uint8_t*)content, strlen(content), NULL);
*/
	cmd(tox, friend_number, (const char*)message);
	return;
    }
    else if(0 == memcmp(message, "get", 3))
    {
        char path[1024]={0};

	char* pSpace = strchr((char*)message, ' ');
	if(pSpace == NULL)
        {
            tox_friend_send_message(tox, friend_number, type, (const uint8_t*)"get /home/wzl/abc.txt", 21, NULL);
            return;
        }

	pSpace++;
	strncpy(path, pSpace, 1023);

        file_send(tox, friend_number, path);
        return;
    }
    else if(0 == memcmp(message, "pwd", 3))
    {

    }
    else if(0 == memcmp(message, "wget", 4))
    {

        char url[1024]={0};

	char* pSpace = strchr((char*)message, ' ');
	if(pSpace == NULL)
        {
            tox_friend_send_message(tox, friend_number, type, (const uint8_t*)"url http://www.abc.com/abc.pdf", strlen("url http://www.abc.com/abc.pdf"), NULL);
            return;
        }

	pSpace++;
	strncpy(url, pSpace, 1023);
        wget(tox, friend_number, url);
        return;
    }

    printf("%s\n", (const char*)message);


    tox_friend_send_message(tox, friend_number, type, message, length, NULL);
}

void self_connection_status_cb(Tox *tox, TOX_CONNECTION connection_status, void *user_data)
{
    switch (connection_status) {
        case TOX_CONNECTION_NONE:
            printf("Offline\n");
            break;
        case TOX_CONNECTION_TCP:
            printf("Online, using TCP\n");
            break;
        case TOX_CONNECTION_UDP:
            printf("Online, using UDP\n");
            break;
    }
}

int main()
{
    Tox *tox = create_tox();

    const char *name = "File Bot";
    tox_self_set_name(tox, (const uint8_t*)name, strlen(name), NULL);

    const char *status_message = "Echoing your messages";
    tox_self_set_status_message(tox, (const uint8_t*)status_message, strlen(status_message), NULL);

    bootstrap(tox);

    print_tox_id(tox);

    tox_callback_friend_request(tox, friend_request_cb);
    tox_callback_friend_message(tox, friend_message_cb);
    tox_callback_file_chunk_request(tox, tox_file_chunk_request);

    tox_callback_self_connection_status(tox, self_connection_status_cb);

    update_savedata_file(tox);

    while (1) {
        tox_iterate(tox, NULL);
        usleep(tox_iteration_interval(tox) * 1000);
    }

    tox_kill(tox);

    return 0;
}
