#pragma once

#include <vector>
#include <string>
using namespace std;

typedef std::vector<char> ByteVector;

class BufferSonic
{
public:
	ByteVector				mData;
    mutable int             mReadPos;
public:
	BufferSonic();
	virtual ~BufferSonic();
	void					SetData(char* thePtr, int theCount);
	char*					GetDataPtr();
	int						GetDataLen() const;	
	void					Clear();

	char					ReadByte() const;
	bool					ReadBoolean() const;
	short					ReadShort() const;
	long					ReadLong() const;
	std::string				ReadString() const;	
	void                    ReadData(char *des, int length);
    vector<int>             ReadIntVector() const;
	vector<string>          ReadStringVector() const;
	vector<bool>            ReadBoolVector() const;

	void					WriteByte(char b);
	void					WriteBoolean(bool b);
	void					WriteShort(short s) ;
	void					WriteLong(long l) ;
	void				    WriteString(std::string str);	
    void                    WriteData(char* pData, int length);
	void                    WriteIntVector(vector<int>& vec);
	void                    WriteStringVector(vector<string>& vec);
	void                    WriteBoolVector(vector<bool>& vec);
};
