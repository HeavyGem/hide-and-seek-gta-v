#include <sys/sys_time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/timer.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netex/net.h>
#include <netex/errno.h>
#include <string.h>
#pragma region Declarations
#include <sys/ppu_thread.h>
#include <string.h>
#include <sys/sys_time.h>
#include <sys/time_util.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/process.h>
#include <sys/memory.h>
#include <sys/timer.h>
#include <sys/return_code.h>
#include <sys/prx.h>
#include <stddef.h>
#include <math.h>
#include <stdarg.h>
#include <cellstatus.h>
#include <typeinfo>
#include <vector>
#include <pthread.h>
#include <locale.h>
#include <cell/error.h>
#include <sys/paths.h>
#include <time.h>
#include <net\if_dl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cell/cell_fs.h>
#include <cell/sysmodule.h>
#include <stdio.h>
#include <string.h>
#include <cell/fs/cell_fs_errno.h>
#include <cell/fs/cell_fs_file_api.h>
#include <ppu_intrinsics.h>
#include <cstdlib>
#include "Dialog.h"
#pragma comment(lib, "net_stub")
#pragma comment(lib, "netctl_stub")

#define SERVER_PORT htons(80)

int Socket;
struct hostent *Host;
struct sockaddr_in SocketAddress;
char bufferReturn[10000];
char RequestBuffer[2000];

char *HookName;
char Key[15];
int HookAddress = 0;
int HookAddress1 = 0;

int WriteinConsole(const char * s);

void SleepMM(usecond_t time);

char* SocketRequest(char* URL, char* Key, char* Path = "");
void encryptDecrypt(char *input, char *output);

bool IsRequest(char* Key);

int ExitThisShit();

char* GetKey();

bool isNumericChar1(char x);

int myAtoi(char *str);

bool isRequesthook(char* Key);

bool isRequesthook1(char* Key);