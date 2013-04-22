#include "BufferSonic.h"

BufferSonic::BufferSonic()
:mReadPos(0)
{	
}

BufferSonic::~BufferSonic()
{
}

void BufferSonic::SetData(char* thePtr, int theCount)
{
	mData.clear();
	mData.reserve(theCount);
	mData.insert(mData.begin(), thePtr, thePtr + theCount);
}

char* BufferSonic::GetDataPtr()
{
	if (mData.size() == 0)
		return NULL;
	return &mData[0];
}

int BufferSonic::GetDataLen() const
{
    return mData.size();
}

void BufferSonic::Clear()
{
	mReadPos = 0;

	mData.clear();
}

char BufferSonic::ReadByte() const
{
	if (mReadPos >= mData.size())
	{
		return 0; //Underflow
	}

	char b = mData[mReadPos];
    
	mReadPos++;

	return b;
}

bool BufferSonic::ReadBoolean() const
{
	return ReadByte() != 0;
}

short BufferSonic::ReadShort() const
{
	char b1 = ReadByte();
	char b2 = ReadByte();

	short aShort;

	char *byt = (char*)&aShort;

	byt[0] = b1;
	byt[1] = b2;

	return aShort;	
}

long BufferSonic::ReadLong() const
{

	char b1 = ReadByte();
	char b2 = ReadByte();
    char b3 = ReadByte();
	char b4 = ReadByte();

	long aLong;
    
	char *byt = (char*)&aLong;

	byt[0] = b1;
    byt[1] = b2;
	byt[2] = b3;
	byt[3] = b4;

	return aLong;
}

std::string	BufferSonic::ReadString() const
{
	std::string aString;
	int aLen = ReadShort();

	for (int i = 0; i < aLen; i++)
		aString += (char) ReadByte();

	return aString;
}

void BufferSonic::ReadData( char *des, int length )
{
    for (int i = 0; i < length; i++)
    {
		des[i] = ReadByte();
    }
}

vector<int> BufferSonic::ReadIntVector() const
{
    vector<int> vecRes;

	int num = ReadLong();

	for (int i = 0; i < num; i++)
	{
		vecRes.push_back(ReadLong());
	}

	return vecRes;
}

vector<string> BufferSonic::ReadStringVector() const
{
	vector<string> vecRes;

	int num = ReadLong();

	for (int i = 0; i < num; i++)
	{
		vecRes.push_back(ReadString());
	}

	return vecRes;
}

vector<bool> BufferSonic::ReadBoolVector() const
{
	vector<bool> vecRes;

	int num = ReadLong();

	for (int i = 0; i < num; i++)
	{
		vecRes.push_back(ReadBoolean());
	}

	return vecRes;
}

void BufferSonic::WriteByte( char b )
{
   mData.push_back(b);
}

void BufferSonic::WriteBoolean( bool b )
{
   WriteByte(b);
}

void BufferSonic::WriteShort( short s )
{
	char *byt = (char*)&s;

	WriteByte(byt[0]);
	WriteByte(byt[1]);

}

void BufferSonic::WriteLong( long l )
{
    char *byt = (char*)&l;

	WriteByte(byt[0]);
    WriteByte(byt[1]);
	WriteByte(byt[2]);
	WriteByte(byt[3]);
}

void BufferSonic::WriteString( std::string str )
{
	std::string aString;
	int aLen = str.length();
    
	WriteShort(aLen);
	for (int i = 0; i < aLen; i++)
		WriteByte(str[i]);
}

void BufferSonic::WriteData( char* pData, int length )
{
   for (int i = 0; i < length; i++)
   {
	   WriteByte(pData[i]);
   }
}

void BufferSonic::WriteIntVector( vector<int>& vec )
{
   WriteLong(vec.size());

   for (int i = 0; i < vec.size(); i++)
   {
	   WriteLong(vec[i]);
   }
}

void BufferSonic::WriteStringVector( vector<string>& vec )
{
	WriteLong(vec.size());

	for (int i = 0; i < vec.size(); i++)
	{
		WriteString(vec[i]);
	}
}

void BufferSonic::WriteBoolVector( vector<bool>& vec )
{
	WriteLong(vec.size());

	for (int i = 0; i < vec.size(); i++)
	{
		WriteBoolean(vec[i]);
	}
}

