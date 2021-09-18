#include <cstdint>
#include <cstddef>
#include <vector>
#include <cmath>

namespace asn1 {
	//checks if 8'th bit is set
	bool isShortForm(const std::uint8_t byte);

	//returns number of additional bytes in which length is stored. Can be value from 0 (in short form) to 127
	std::uint8_t getLengthBytesNum(const std::uint8_t byte);

	//functions for encoding length in ASN1 format, returns length header + bytes sequence in big endian (if needed)
	std::vector<char> encodeLength(const std::size_t length);

	//decoding ASN1 length:
	//return type: std::size_t length
	//template parameters: function F for getting data and Args for that function.
	//F must have the last parameter of std::size_t type for the number of bytes to get.
	//return type of F must be vector of chars.
	template <typename F /*function for gettting bytes*/, typename ... Args /*arguments needed for function*/>
	std::size_t decodeLength(F&& getBytesFunc, Args&& ... funcArgs)
	{
		//get first byte and check if 8'th bit is set
		auto lengthByte = getBytesFunc(funcArgs ..., 1);
		auto lengthBytesNum = getLengthBytesNum(lengthByte[0]);

		std::size_t length = 0;
		//if length can fit in first byte, it is the length
		if (lengthBytesNum == 0)
			length = lengthByte[0];
		else //read lengthBytesNum bytes
		{
			if (lengthBytesNum > sizeof(length))
				throw std::runtime_error("Too big length");

			//length of the ASN1 message is coded in Big-Endian format
			auto lengthBytes = getBytesFunc(funcArgs ..., lengthBytesNum);

			//get length from vector in BigEndian and then convert int to LittleEndian
			std::uint8_t byteNum = 0;
			for (auto it = lengthBytes.rbegin(); it != lengthBytes.rend(); ++it)
			{
				length += static_cast<std::size_t>(*it) << (static_cast<std::size_t>(byteNum++) * 8);
			}
		}
		return length;
	}

}