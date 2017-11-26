#ifndef UTILS_H
#define UTILS_H
#include "Includes.h"

#define DEX // Change before compilation
#define TOC  (0x1C85330)
#define FLT_MAX (3.40282347E+38F)
#define PI (3.14159265359)

void sleep(usecond_t time = 100);
int console_write(const char* s);
#ifdef DEX
int32_t write_offset(uint64_t ea, const void * data, uint32_t size);
#else
void write_offset(unsigned int location, void *data, int size);
#endif
void PatchInJump(int Address, int Destination);
float fmod_(float a, float b);
void itoa(char* str, int num);
void fntos(char* buffer, float input);
void null();

typedef void* Void;
typedef int Any;
typedef int netScene;
typedef unsigned int uint;
typedef int Hash;
typedef int BOOL;
typedef int Entity;
typedef int Player;
typedef int FireId;
typedef int Ped;
typedef int Vehicle;
typedef int Cam;
typedef int CarGenerator;
typedef int Group;
typedef int Train;
typedef int Pickup;
typedef int Object;
typedef int Weapon;
typedef int Interior;
typedef int Blip;
typedef int Texture;
typedef int TextureDict;
typedef int CoverPoint;
typedef int Camera;
typedef int TaskSequence;
typedef int ColourIndex;
typedef int Sphere;
typedef int ScrHandle;

struct opd_s { int32_t sub; int32_t toc; };

#define RUN_MODULE(name, start) \
		SYS_MODULE_INFO(name, 0, 1, 1); \
		SYS_MODULE_START(start);

class Debug {
public:
	void Write(char* input);
	void WriteLine(char* input = "");
	void operator <<(char* input);
};
sys_ppu_thread_t new_thread(void(*method)(uint64_t));

void wrap_num(float* x, float min, float max);
template<typename T>
void wrap_num(T* ptr, T min, T max) {
	T range = max - min + 1;
	if (*ptr < min) *ptr += range * ((min - *ptr) / range + 1);
		*ptr = min + (*ptr - min) % range;
}

typedef class string {
private:
	char _string[0x25];
public:
	string(char* a = "");
	string(const char* a = "");
	string(int a);
	string(float a);
	string(double a);
	string(bool a);
	int length();
	operator char*();
	operator const char*();
	string operator +(const string str2);
	string operator +=(const string str2);
	string operator =(const string str2);
	bool operator ==(const string str2);
	bool operator ==(const char* str2);
	bool contains(const string str2);
	char operator [](const int x);
} string_t;

typedef struct
{
	float x;
	float y;
	float z;
} Vector3;

struct RGB { int R, G, B; };

#endif // !UTILS_H