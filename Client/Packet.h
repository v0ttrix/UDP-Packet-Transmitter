#pragma once
#include <memory>
#include <iostream>
#include <fstream>

const int EmptyPktSize = 6;					//number of data bytes in a packet with no data field

class Packet
{
	struct Header
	{
		unsigned char Source : 4;				//source ID
		unsigned char LineNumber : 4;			//line number of the input file being transmitted
		unsigned char Length;					//number of characters in the line
	} Head;
	char *Data;							//the data bytes
	unsigned int CRC;					//cyclic Redundancy Check

	char *TxBuffer;

public:
	Packet() : Data(nullptr), TxBuffer(nullptr) { memset(&Head, 0, sizeof(Head));  Head.Source = 2; };		//Default Constructor - Safe State
	void SetLineNumber(int value) { Head.LineNumber = value; };		//sets the line number within the object
	void Display(std::ostream& os)
	{
		os << std::dec;
		os << "Source:  " << (int)Head.Source << std::endl;
		os << "LineNum: " << (int)Head.LineNumber << std::endl;
		os << "Length:  " << (int)Head.Length << std::endl;
		os << "Msg:     " << Data << std::endl;
		os << "CRC:     " << std::hex << (unsigned int)CRC << std::endl;
	}
	
	Packet(char* src)
	{
		//Initialize pointers to safe state
		Data = nullptr;
		TxBuffer = nullptr;
		
		//Step 1: parse header (first 2 bytes)
		memcpy(&Head, src, sizeof(Head));
		
		//Step 2: allocate and copy Data (length bytes)
		if (Head.Length > 0)
		{
			Data = new char[Head.Length + 1];  // +1 for null terminator
			memcpy(Data, src + sizeof(Head), Head.Length);
			Data[Head.Length] = '\0';  // null terminate
		}
		
		//Step 3: parse CRC (last 4 bytes)
		memcpy(&CRC, src + sizeof(Head) + Head.Length, sizeof(CRC));
	}

	void SetData(char* srcData, int Size)
	{
		//Step 1: allocate memory for data (Size + 1 for null terminator)
		Data = new char[Size + 1];
		
		//Step 2: copy data from source to internal buffer
		memcpy(Data, srcData, Size);
		
		//Step 3: Add null terminator for safe string operations
		Data[Size] = '\0';
		
		//Step 4: update header with data length
		Head.Length = Size;
	};

	char* SerializeData(int &TotalSize)
	{
		//Step 1: calculate CRC first
		CalculateCRC();
		
		// Step 2: calculate total packet size = Header(2) + Data(Length) + CRC(4)
		TotalSize = sizeof(Head) + Head.Length + sizeof(CRC);
		
		//Step 3: allocate transmission buffer
		TxBuffer = new char[TotalSize];
		
		//Step 4: copy Header (2 bytes)
		memcpy(TxBuffer, &Head, sizeof(Head));
		
		//Step 5: copy Data (Length bytes)
		memcpy(TxBuffer + sizeof(Head), Data, Head.Length);
		
		//Step 6: copy CRC (4 bytes) at the end
		memcpy(TxBuffer + sizeof(Head) + Head.Length, &CRC, sizeof(CRC));
		
		//Step 7: return the serialized buffer
		return TxBuffer;
	};

	unsigned int CalculateCRC()
	{
		CRC = 0xFF00FF00;	//set CRC to specified value
		return CRC;
	}
};
