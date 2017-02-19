#include "Net.h"
#include <sstream>

Net::Net(int port, int packetSize)
{
	_socket = SDLNet_UDP_Open(port);
	if (!_socket)
	{
		std::cout << "Failed to open socket on port " << port << std::endl;
	}
	_packet = SDLNet_AllocPacket(packetSize);
}

//void Net::SetPacketAddress()
//{
//	_packet->address.host = _ipAddress.host;
//	_packet->address.port = _ipAddress.port;
//}

void Net::WriteInt(int value) 
{
	_packet->data[_packet->len++] = (value);		
	_packet->data[_packet->len++] = (value >> 0x08);
	_packet->data[_packet->len++] = (value >> 0x10);
	_packet->data[_packet->len++] = (value >> 0x18);
}

int Net::ReadInt(int & byteOffset)
{
	int value = _packet->data[byteOffset++];
	value |= _packet->data[byteOffset++] << 0x08;
	value |= _packet->data[byteOffset++] << 0x10;
	value |= _packet->data[byteOffset++] << 0x18;
	return value;
}

void Net::WriteFloat(float valueF)
{
	//endianess issues? IEEE 754?
	Uint8* value = reinterpret_cast<Uint8*>(&valueF);
	for (int i = 0; i < sizeof(float); i++)
	{
		_packet->data[_packet->len++] = value[i];
	}
}

float Net::ReadFloat(int & byteOffset)
{
	float value;
	memcpy(&value, _packet->data + byteOffset, sizeof(float));
	byteOffset += sizeof(float);
	return value;
}

void Net::WriteString(std::string & s)
{
	int length = s.length() + 1;
	WriteInt(length); //this adds 4 more bytes? instead, try reading until nul is reached but depends on encoding
	memcpy(_packet->data + _packet->len, s.c_str(), length);
	_packet->len += length;
	_packet->len++;
}

std::string Net::ReadString(int & byteOffset)
{ 
	int length = ReadInt(byteOffset);
	std::stringstream ss;
	for (int i = 0; i < length; i++)
	{
		ss << _packet->data[byteOffset + i];
	}
	byteOffset += length + 1;
	return ss.str();
}
