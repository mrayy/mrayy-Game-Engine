
#include "stdafx.h"
#include "BitConverter.h"

namespace mray
{
namespace core
{

int BitConverter::GetBytes(bool val,byte* buff,int maxLen)
{
	return GetBytesT(val,buff,maxLen);
}
int BitConverter::GetBytes(char val,byte* buff,int maxLen)
{
	return GetBytesT(val,buff,maxLen);
}
int BitConverter::GetBytes(short val,byte* buff,int maxLen)
{
	return GetBytesT(val,buff,maxLen);
}
int BitConverter::GetBytes(ushort val,byte* buff,int maxLen)
{
	return GetBytesT(val,buff,maxLen);
}
int BitConverter::GetBytes(int val,byte* buff,int maxLen)
{
	return GetBytesT(val,buff,maxLen);
}
int BitConverter::GetBytes(uint val,byte* buff,int maxLen)
{
	return GetBytesT(val,buff,maxLen);
}
int BitConverter::GetBytes(long val,byte* buff,int maxLen)
{
	return GetBytesT(val,buff,maxLen);
}
int BitConverter::GetBytes(ulong val,byte* buff,int maxLen)
{
	return GetBytesT(val,buff,maxLen);
}
int BitConverter::GetBytes(float val,byte* buff,int maxLen)
{
	return GetBytesT(val,buff,maxLen);
}
int BitConverter::GetBytes(double val,byte* buff,int maxLen)
{
	return GetBytesT(val,buff,maxLen);
}

bool BitConverter::ToBool(const byte* value)
{
	return GetValueT<bool>(value);
}
char BitConverter::ToChar(const byte* value)
{
	return GetValueT<char>(value);
}
short BitConverter::ToInt16(const byte* value)
{
	return GetValueT<short>(value);
}
ushort BitConverter::ToUInt16(const byte* value)
{
	return GetValueT<ushort>(value);
}
int BitConverter::ToInt32(const byte* value)
{
	return GetValueT<int>(value);
}
uint BitConverter::ToUInt32(const byte* value)
{
	return GetValueT<uint>(value);
}
long BitConverter::ToInt64(const byte* value)
{
	return GetValueT<long>(value);
}
ulong BitConverter::ToUInt64(const byte* value)
{
	return GetValueT<ulong>(value);
}
float BitConverter::ToFloat(const byte* value)
{
	return GetValueT<float>(value);
}
double BitConverter::ToDouble(const byte* value)
{
	return GetValueT<double>(value);
}


}
}
