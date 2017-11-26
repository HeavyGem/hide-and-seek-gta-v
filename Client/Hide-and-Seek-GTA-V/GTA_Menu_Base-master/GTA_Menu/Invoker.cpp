#include "Utils.h"
#include "Invoker.h"

unsigned int FindNativeTableAddress()
{
	unsigned int  l_uiNativeTableAddress = 0;
	unsigned int l_uiStartAddress = 0x390000;
	unsigned int l_uiAddress = 0;
	for (unsigned int i = 0; i < 0x10000; i++) {
		if (*(unsigned int*)(l_uiStartAddress + i) == 0x3C6072BD && *(unsigned int*)(l_uiStartAddress + i + 8) == 0x6063E002) {
			l_uiAddress = *(unsigned int*)(l_uiStartAddress + i + 0x10);
			l_uiAddress &= 0xFFFFFF;
			l_uiAddress += (l_uiStartAddress + i + 0x10) - 1;
			break;
		}
	}
	l_uiNativeTableAddress = (*(unsigned int*)(l_uiAddress + 0x24) << 16) + (*(unsigned int*)(l_uiAddress + 0x2C) & 0xFFFF);
	l_uiNativeTableAddress -= 0x10000;
	return l_uiNativeTableAddress;
}
int NativeAddress(int Native, bool PatchInJump)
{
	int Hash = Native & 0xFF;
	int Table = *(int*)(FindNativeTableAddress() + (Hash * 4));
	while (Table) {
		int NativeCount = *(int*)(Table + 0x20);
		for (int i = 0; i < NativeCount; i++) {
			if (*(int*)((Table + 0x24) + (i * 4)) == Native) {
				if (PatchInJump) {
					int NativeLocation = *(int*)(*(int*)((Table + 4) + (i * 4)));
					for (int i = 0; i < 50; ++i) {
						short CurrentPlace = *(short*)(NativeLocation + (i * 4));
						if (CurrentPlace == 0x4AE6 || CurrentPlace == 0x4AA8 || CurrentPlace == 0x4AE4 || CurrentPlace == 0x4AE5) {
							return (((*(int*)(NativeLocation + (i * 4))) - 0x48000001) + (NativeLocation + (i * 4))) - 0x4000000;
						}
					}
				}
				else return *(int*)((Table + 4) + (i * 4));
			}
		}
		Table = *(int*)(Table);
	}
}
void ResetArgs()
{
	NativeArg->ArgCount = 0;
	NativeArg->ReadCount = 0;
	NativeArg->ArgValues = (int*)0x10050000;
	NativeArg->ReturnValue = NativeArg->ArgValues;
}
void CallHash(unsigned int hash, NativeArg_s* pArg)
{
	Native_s* _Natives = g_Natives[hash & 0xFF];
	while (_Natives) {
		for (unsigned int i = 0; i < _Natives->NativeCount; i++) {
			if (_Natives->NativeHashes[i] == hash) {
				((void(*)(NativeArg_s*))_Natives->NativeFunctions[i])(pArg);
				return;
			}
		}
		_Natives = _Natives->LastNativesTable;
	}
}
void PushArg(Vector3 Value) {
	PushArg(Value.x);
	PushArg(Value.y);
	PushArg(Value.z);
}