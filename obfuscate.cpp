#include<iostream>
#include<Windows.h>

#define key 0x55

constexpr uint32_t modulus() {
	return 0x7fffffff;
}
//pseudo-random number generator (PRNG)
constexpr uint32_t prng(const uint32_t input) {
	return (input * 48271) % modulus();
}


template<size_t N>
constexpr uint32_t seed(const char(&entropy)[N], const uint32_t init_val = 0) {
	auto value{ init_val };
	for (size_t i{ 0 }; i < N; i++) {
		value = (value & ((~0) << 8)) | ((value & 0xFF) ^ entropy[i]);
		value = value << 8 | value >> ((sizeof(value) * 8) - 8);
	}

	while (value > modulus()) {
		value = value >> 1;
	}

	return value << 1 | 1;
}

template <typename T, size_t N>
struct obfuscate {
	int seed;
	T data[N];
};

template<size_t N>
constexpr auto crypt(const char(&input)[N], const uint32_t seed = 0) {
	obfuscate<char, N>dark{};
	dark.seed = seed;
	for (uint32_t index{ 0 }, stream{ seed }; index < N; index++) {
		dark.data[index] = input[index] ^ stream;
		stream = prng(stream);
	}

	return dark;
}

int main() {
	
	constexpr auto obfuscated_str = crypt("Hello", key);

	
	std::string pre_obfuscated;
	for (const auto& c : obfuscated_str.data) {
		pre_obfuscated.push_back(c);
	}
	std::cout << pre_obfuscated;

	return 0;

}
