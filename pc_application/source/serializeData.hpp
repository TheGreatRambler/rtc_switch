#pragma once

#include <cstring>
#include <memory>
#include <zpp.h>

struct ProtcolFormat {
	// Flag in a uint8_t
	uint8_t flag;
}

namespace SerializeProtocol {
	// clang-format on
	// Will be converted to bytes
	std::vector<unsigned char> serializingData;

	std::shared_ptr<ProtcolFormat> binaryToFrame(uint8_t * data, uint16_t size) {
		// Load with YAS
		std::shared_ptr<ProtcolFormat> inputData = std::make_shared<ProtcolFormat>();

		// Create the archive
		zpp::serializer::memory_input_archive in(serializingData);

		// This simple
		in(inputData);

		serializingData.clear();
		return inputData;
	}

	void frameToBinary(std::shared_ptr<ProtcolFormat> inputData, uint8_t * data, uint16_t * size) {
		// Create the archive
		zpp::serializer::memory_output_archive out(serializingData);

		out(inputData);

		// Copy to pointer
		// An unsigned char is one byte, so this is it
		*size = (uint16_t)serializingData.size();
		memcpy(data, serializingData.data(), *size);
		serializingData.clear();
	}
}