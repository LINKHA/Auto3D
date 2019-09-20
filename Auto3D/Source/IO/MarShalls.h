#pragma once

namespace Auto3D {

union MarshallFloat 
{

	unsigned int _uInt; ///< int
	float _float; ///< float
};

union MarshallDouble 
{

	unsigned long long _ulonglong; ///< long long
	double _double; ///< double
};

static inline unsigned int EncodeUInt16(unsigned short value, unsigned char* arr) {

	for (int i = 0; i < 2; i++) {

		*arr = value & 0xFF;
		arr++;
		value >>= 8;
	}

	return sizeof(unsigned short);
}

static inline unsigned int EncodeUInt32(unsigned int value, unsigned char* arr) {

	for (int i = 0; i < 4; i++) {

		*arr = value & 0xFF;
		arr++;
		value >>= 8;
	}

	return sizeof(unsigned int);
}

static inline unsigned int EncodeFloat(float value, unsigned char* arr) {

	MarshallFloat marshallFloat;
	marshallFloat._float = value;
	EncodeUInt32(marshallFloat._uInt, arr);

	return sizeof(unsigned int);
}

static inline unsigned int EncodeUInt64(unsigned long long value, unsigned char* arr) {

	for (int i = 0; i < 8; i++) {

		*arr = value & 0xFF;
		arr++;
		value >>= 8;
	}

	return sizeof(unsigned long long);
}

static inline unsigned int EncodeDouble(double p_double, unsigned char* arr) {

	MarshallDouble md;
	md._double = p_double;
	EncodeUInt64(md._ulonglong, arr);

	return sizeof(unsigned long long);
}

static inline int EncodeCString(const char* p_string, unsigned char* p_data) {

	int len = 0;

	while (*p_string) {

		if (p_data) {

			*p_data = (unsigned char)* p_string;
			p_data++;
		}
		p_string++;
		len++;
	};

	if (p_data)* p_data = 0;
	return len + 1;
}

static inline unsigned short DecodeUInt16(const unsigned char* arr) {

	unsigned short u = 0;

	for (int i = 0; i < 2; i++) {

		unsigned short b = *arr;
		b <<= (i * 8);
		u |= b;
		arr++;
	}

	return u;
}

static inline unsigned int DecodeUInt32(const unsigned char* arr) {

	unsigned int u = 0;

	for (int i = 0; i < 4; i++) {

		unsigned int b = *arr;
		b <<= (i * 8);
		u |= b;
		arr++;
	}

	return u;
}

static inline float DecodeFloat(const unsigned char* arr) {

	MarshallFloat marshallFloat;
	marshallFloat._uInt = DecodeUInt32(arr);
	return marshallFloat._float;
}

static inline unsigned long long DecodeUInt64(const unsigned char* arr) {

	unsigned long long u = 0;

	for (int i = 0; i < 8; i++) {

		unsigned long long b = (*arr) & 0xFF;
		b <<= (i * 8);
		u |= b;
		arr++;
	}

	return u;
}

static inline double DecodeDouble(const unsigned char* arr) {

	MarshallDouble marshallDouble;
	marshallDouble._ulonglong = DecodeUInt64(arr);
	return marshallDouble._double;
}

}