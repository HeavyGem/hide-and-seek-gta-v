#include "Utils.h"

string::operator char*() {
	return _string;
}
string::operator const char*() {
	return _string;
}
string::string(char* a) {
	strcpy(_string, a);
}
string::string(const char* a) {
	strcpy(_string, a);
}
string::string(int a) {
	itoa(_string, a);
}
string::string(float a) {
	fntos(_string, a);
}
string::string(double a) {
	fntos(_string, a);
}
string::string(bool a) {
	strcpy(_string, a ? "true" : "false");
}

int string::length() {
	return strlen(_string);
}
string string::operator +(const string str2) {
	char out[sizeof(_string) + sizeof(str2._string)];
	strcpy(out, _string);
	strcat(out, str2._string);
	strcpy(_string, out);
	return *this;
}
string string::operator +=(const string str2) {
	return *this + str2;
}
string string::operator =(const string str2) {
	strcpy(_string, str2._string);
	return *this;
}
bool string::operator ==(const string str2) {
	return !strcmp(_string, str2._string);
}
bool string::operator ==(const char* str2) {
	return !strcmp(_string, str2);
}
bool string::contains(const string str2) {
	return strstr(_string, str2._string);
}
char string::operator [](const int x) {
	return _string[x];
}