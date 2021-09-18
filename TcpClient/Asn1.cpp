#include "Asn1.h"

namespace asn1 {
	//checks if 8'th bit is set
	bool isShortForm(const std::uint8_t byte)
	{
		if (byte >> 7 == 0)
			return true;
		else return false;
	}
	//returns number of additional bytes in which length is stored. Can be value from 0 (in short form) to 127
	std::uint8_t getLengthBytesNum(const std::uint8_t byte)
	{
		if (isShortForm(byte))
			return 0;
		else return byte & 0b01111111;
	}

	//functions for encoding length in ASN1 format, returns length header + bytes sequence in big endian (if needed)
	std::vector<char> encodeLength(const std::size_t length)
	{
		std::vector<char> lengthBytes;
		if (length < 128) //until 8'th bit is needed
		{
			//short form
			lengthBytes.push_back(static_cast<char>(length));
		}
		else
		{
			//long form
			auto bitsCount = std::ceil(std::log2(length));
			std::size_t bytesCount = (std::size_t)std::ceil(bitsCount / 8); //number of bytes in which size will be stored

			//casting is safe since we know bytesCount < sizeof(std::uint64_t or std::unit_32t)
			lengthBytes.push_back(bytesCount | (1 << 7)); //setting 8'th bit

			//assume that computer has Little-Endian encoding
			union {
				std::size_t big;
				char byte[sizeof(big)];
			} lengthUnion;
			lengthUnion.big = length;
			//pushing back bytesCount bytes of length in Big-Endian format
			for (std::size_t i = 0; i < bytesCount; i++)
			{
				lengthBytes.push_back(lengthUnion.byte[bytesCount - i - 1]);
			}
		}
		return lengthBytes;
	}
}