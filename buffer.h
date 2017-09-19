#ifndef __BUFFER_H
#define __BUFFER_H

#include <stdint.h>
#include <string>
#include <vector>

class Buffer {
public:
	Buffer (std::size_t size);
	void writeUInt32BE(std::size_t index, uint32_t value);
	void writeUInt32BE(uint32_t value);
	void writeUInt32LE(std::size_t index, uint32_t value);
	void writeUInt32LE(uint32_t value);

	void writeInt32BE(std::size_t index, int32_t value);
	void writeInt32BE(int32_t value);
	void writeInt32LE(std::size_t index, int32_t value);
	void writeInt32LE(int32_t value);

	void writeUInt16BE(std::size_t index, uint16_t value);
	void writeUInt16BE(uint16_t value);
	void writeUInt16LE(std::size_t index, uint16_t value);
	void writeUInt16LE(uint16_t value);

	void writeInt16BE(std::size_t index, int16_t value);
	void writeInt16BE(int16_t value);
	void writeInt16LE(std::size_t index, int16_t value);
	void writeInt16LE(int16_t value);

	uint32_t readUInt32BE(std::size_t index);
	uint32_t readUInt32BE();
	uint32_t readUInt32LE(std::size_t index);
	uint32_t readUInt32LE();

	int32_t readInt32BE(std::size_t index);
	int32_t readInt32BE();
	int32_t readInt32LE(std::size_t index);
	int32_t readInt32LE();

	uint16_t readUInt16BE(std::size_t index);
	uint16_t readUInt16BE();
	uint16_t readUInt16LE(std::size_t index);
	uint16_t readUInt16LE();

	int16_t readInt16BE(std::size_t index);
	int16_t readInt16BE();
	int16_t readInt16LE(std::size_t index);
	int16_t readInt16LE();

	void writeString(std::string str);
	std::string readString();

	std::size_t getReadIndex();
	std::size_t getWriteIndex();

	void setReadIndex(std::size_t index);
	void setWriteIndex(std::size_t index);

	void printInHex();
	std::vector<uint8_t>& getData();

private:

	std::vector<uint8_t> _buffer;
	int writeIndex;
	int readIndex;
};


#endif


#include "buffer.hpp"
#include <stdio.h>

Buffer::Buffer(std::size_t size) {
	_buffer.resize(size);
	writeIndex = 0;
	readIndex  = 0;
}

void Buffer::writeUInt32BE(std::size_t index, uint32_t value) {
	_buffer[index + 3] = value;
	_buffer[index + 2] = value >> 8;
	_buffer[index + 1] = value >> 16;
	_buffer[index]     = value >> 24;
}

void Buffer::writeUInt16BE(std::size_t index, uint16_t value) {
	_buffer[index + 1] = value;
	_buffer[index] = value >> 8;
}

void Buffer::writeUInt32BE(uint32_t value) {
	writeUInt32BE(writeIndex, value);
	writeIndex += 4;
}

void Buffer::writeUInt16BE(uint16_t value) {
	writeUInt16BE(writeIndex, value);
	writeIndex += 2;
}

void Buffer::writeUInt32LE(std::size_t index, uint32_t value) {
	_buffer[index] = value;
	_buffer[index + 1] = value >> 8;
	_buffer[index + 2] = value >> 16;
	_buffer[index + 3] = value >> 24;
}	

void Buffer::writeUInt32LE(uint32_t value) {
	writeUInt32LE(writeIndex, value);
	writeIndex += 4;
}

uint32_t Buffer::readUInt32BE(std::size_t index) {
	// 27 b5 57 01
	// 00 00 00 27
	uint32_t value = _buffer[index + 3];
	// 00 00 b5 00
	value |= _buffer[index + 2] << 8;
	// 00 57 00 00
	value |= _buffer[index + 1] << 16;
	// 01 00 00 00
	value |= _buffer[index] << 24;

	return value;
}

uint32_t Buffer::readUInt32BE() {
	uint32_t value = readUInt32BE(readIndex);
	readIndex += 4;
	return value;
}

uint32_t Buffer::readUInt32LE(std::size_t index) {
	// 27 b5 57 01
	// 00 00 00 27
	uint32_t value = _buffer[index];
	// 00 00 b5 00
	value |= _buffer[index + 1] << 8;
	// 00 57 00 00
	value |= _buffer[index + 2] << 16;
	// 01 00 00 00
	value |= _buffer[index + 3] << 24;

	return value;
}

uint32_t Buffer::readUInt32LE() {
	uint32_t value = readUInt32LE(readIndex);
	readIndex += 4;
	return value;
}

uint16_t Buffer::readUInt16BE(std::size_t index) {
	uint16_t value = _buffer[index + 1];
	value |= _buffer[index];
	return value;
}

uint16_t Buffer::readUInt16BE() {
	uint32_t value = readUInt16BE(readIndex);
	readIndex += 2;
	return value;
}

std::size_t Buffer::getReadIndex() {
	return readIndex;
}

std::size_t Buffer::getWriteIndex() {
	return writeIndex;
}

void Buffer::setReadIndex(std::size_t index) {
	readIndex = index;
}

void Buffer::setWriteIndex(std::size_t index) {
	writeIndex = index;
}

void Buffer::printInHex() {
	for (std::vector<uint8_t>::iterator it = _buffer.begin() ; it != _buffer.end(); ++it) {
		printf("%02x ", *it);
	}
	printf("\n");
}

std::vector<uint8_t>& Buffer::getData() {
	return _buffer;
}

void Buffer::writeString(std::string str) {
	writeUInt32BE(str.length());
	for (int idxString = 0; idxString < str.length(); idxString++) {
		_buffer[writeIndex] = str[idxString];
		writeIndex += 1;
	}
}

std::string Buffer::readString() {
	uint32_t length = readUInt32BE();
	std::string str;
	
	for (uint32_t idxString = 0; idxString < length; idxString++) {
		str += _buffer[readIndex];
		readIndex += 1;
	}
	return str;
}
