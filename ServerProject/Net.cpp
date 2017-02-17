#include "Net.h"

Net::Net(const std::string & targetHost, int packetSize, int serverPort, int clientPort)
{
	IPaddress ipAddress;
	SDLNet_ResolveHost(&ipAddress, targetHost.c_str(), serverPort);
	_socket = SDLNet_UDP_Open(clientPort);

	_packet = SDLNet_AllocPacket(packetSize);
	_packet->address.host = ipAddress.host;
	_packet->address.port = ipAddress.port;
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

void Net::WriteString(std::string & s)
{
	int l = s.length() + 1;
	memcpy(_packet->data + _packet->len, s.c_str(), l);
	_packet->len += l;
	_packet->len++;
}

std::string Net::ReadString(int & byteOffset)
{
	return std::string((char*)_packet->data + byteOffset);
}
