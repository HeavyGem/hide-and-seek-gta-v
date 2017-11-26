#include "Sockets.h"

int WriteinConsole(const char * s)
{
	uint32_t len;
	system_call_4(403, 0, (uint64_t)s, std::strlen(s), (uint64_t)&len);
	return_to_user_prog(int);
}

void SleepMM(usecond_t time)
{
	sys_timer_usleep(time * 1000);
}

char* SocketRequest(char* URL, char* Key, char* Path = "")
{
	Host = gethostbyname(URL);
	SocketAddress.sin_addr.s_addr = *((unsigned long*)Host->h_addr);
	SocketAddress.sin_family = AF_INET;
	SocketAddress.sin_port = SERVER_PORT;
	Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(Socket, (struct sockaddr *)&SocketAddress, sizeof(SocketAddress)) != 0) {
		return "CONNECTION ERROR";
	}
	strcpy(RequestBuffer, "GET /");
	if (strlen(Path) > 0){
		strcat(RequestBuffer, Path);
	}
	strcat(RequestBuffer, Key);
	strcat(RequestBuffer, " HTTP/1.0\r\nHOST: ");
	strcat(RequestBuffer, URL);
	strcat(RequestBuffer, "\r\n\r\n");

	send(Socket, RequestBuffer, strlen(RequestBuffer), 0);

	while (recv(Socket, bufferReturn, 10000, 0) > 0)
	{
		return bufferReturn;
		SleepMM(1);
	}
	socketclose(Socket);
}

void encryptDecrypt(char *input, char *output) {
	char key[] = { 'K', 'C', 'Q' }; //Can be any chars, and any size array


	for (int i = 0; i < strlen(input); i++) {
		output[i] = input[i] ^ key[i % (sizeof(key) / sizeof(char))];
	}
}

/*
char baseStr[] = "kylewbanks.com";

char encrypted[strlen(baseStr)];
encryptDecrypt(baseStr, encrypted);
printf("Encrypted:%s\n", encrypted);

char decrypted[strlen(baseStr)];
encryptDecrypt(encrypted, decrypted);
printf("Decrypted:%s\n", decrypted);

*/

bool IsRequest(char* Key)
{
	char* penis = SocketRequest("mathiis.pw", Key, "gtalicensething.php?key=");
	char* s = strstr(penis, "Key is valid");
	if (s != NULL)
	{

		return true;
	}
	else
	{
		return false;
	}
}
/*
bool IsPrem(char* Key)
{
	char* penis = SocketRequest("mathiis.xyz", Key, "premlicense.php?key=");
	char* s = strstr(penis, "Key is premi");
	if (s != NULL)
	{

		return true;
	}
	else
	{
		return false;
	}
}*/

int ExitThisShit()
{
	system_call_1(41, 0);
	return_to_user_prog(int);
}

char* GetKey()
{
	int fd;
	int ret;
	uint64_t pos;
	uint64_t nread;

	cellMsgDialogProgressBarInc(0, 1);
	cellMsgDialogProgressBarSetMsg(0, "Loading Key...");
	ret = cellFsOpen("/dev_hdd0/tmp/key.txt", 0, &fd, NULL, 0);
	if (!ret)
	{
		cellFsLseek(fd, 0, CELL_FS_SEEK_SET, &pos);
		ret = cellFsRead(fd, Key, sizeof(Key), &nread);
		if (!ret)
		{
			cellFsClose(fd);
		}
		else
		{
			cellMsgDialogClose(5.0);
			SleepMM(500);
			Dialog::msgdialog_mode = 2;
			Dialog::Show("Key failed to Read!");
			ExitThisShit();
		}
	}
	else
	{
		cellMsgDialogClose(5.0);
		SleepMM(500);
		Dialog::msgdialog_mode = 2;
		Dialog::Show("Key failed to load!");
		ExitThisShit();
	}
	return;
}

bool isNumericChar1(char x)
{
 return (x >= '0' && x <= '9') ? true : false;
}

int myAtoi(char *str)
{
 if (*str == NULL)
  return 0;

 int res = 0;  // Initialize result
 int sign = 1;  // Initialize sign as positive
 int i = 0;  // Initialize index of first digit

 // If number is negative, then update sign
 if (str[0] == '-')
 {
  sign = -1;
  i++;  // Also update index of first digit
 }

 // Iterate through all digits of input string and update result
 for (; str[i] != '\0'; ++i)
 {
  if (isNumericChar1(str[i]) == false)
   return 0; // You may add some lines to write error message
  // to error stream
  res = res * 10 + str[i] - '0';
 }

 // Return result with sign
 return sign*res;
}

bool isRequesthook(char* Key)
{
	char* add = SocketRequest("mathiis.pw", Key, "Lel.php?key=");
	char* add2 = strstr(add, "29");
	HookAddress = myAtoi(add2);
	char *begin = bufferReturn;
	char *end = begin + sizeof(bufferReturn);
	std::fill(begin, end, 0);
}

bool isRequesthook1(char* Key)
{
	char* add = SocketRequest("mathiis.pw", Key, "Lel2.php?key=");
	char* add2 = strstr(add, "40");
	HookAddress1 = myAtoi(add2);
	char *begin = bufferReturn;
	char *end = begin + sizeof(bufferReturn);
	std::fill(begin, end, 0);
}
/*
char* plateName;
bool IsRequestPlate(char* Key)
{
	char* penis = SocketRequest("mathiis.xyz", Key, "plate.php?key=");
	plateName = strstr(penis, "Test");
	char *begin = bufferReturn;
	char *end = begin + sizeof(bufferReturn);
	std::fill(begin, end, 0);
}*/