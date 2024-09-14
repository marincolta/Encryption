# Code Obfuscator
-------------------

## About:
This project demonstrates a straightforward obfuscation technique that leverages a pseudo-random number generator (PRNG) and bitwise operations. It features functions to:

-Generate a seed from an entropy string.

-Obfuscate data using the generated seed.

---

## Explanation
As an amateur reverse engineer, I have been exploring anti-debugging and obfuscation techniques to deepen my understanding. Today, I will explain these techniques and share my implementations, showcasing what I've learned in the process.

The project consists of 4 functions that guide the obfuscation process. Let’s dive into each function and explore how they contribute to the overall obfuscation technique.

**Code**:
```cpp
constexpr uint32_t modulus() {
    return 0x7fffffff;
}
```
The name of the function is modulus and not because i liked it or something but because the main function of it is to return a constant value that would help us to constrain the output range of the PRNG.

if we go to the next function
**Code**:
```cpp
constexpr uint32_t prng(const uint32_t input) {
	return (input * 48271) % modulus();
}
```
We can see our PRNG function which return an interesting value `return (input * 48271) % modulus();` , we have one argument called `input` by using a LCG method we are able to produce a pseudo-random output wthout exceding the limit, therefore we got a range.
**Code**:

```cpp
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
```
The seed function generates a pseudo-random seed value based on an input entropy string and an optional initial value. This seed value can then be used to initialize a pseudo-random number generator. In other words  tt uses bitwise operations and rotations to mix the bits of the entropy into the seed value. After processing, it ensures the seed is within the desired range by adjusting it with bit shifts and ensuring it is odd. 

The last function:
**Code**:
```cpp
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
```

As thename of the function says it obfuscate/encrypt the value we send as a string.
But how?
->Creates an obfuscate struct named dark to store the obfuscated data and the seed used.
->Loop through Input:
  *For each character in the input array, it XORs the character with the current PRNG value
  *Updates `dark.data[index]` with the obfuscated character
  *Updates the PRNG value  using the prng function to generate the next value in the sequence

What can we say about it in the end??
The obfuscate struct and the crypt function work together to obfuscate data using a simple PRNG-based method. Specifically, the crypt function takes a string and a seed as input, applies a bitwise XOR to each character using the PRNG, and stores the result along with the seed in an obfuscate struct.

Besides that, even tho it obfuscate the data (from input) is still far better.
![image](https://github.com/user-attachments/assets/e6157cef-5ea7-4fcd-8514-6fa337148914)

---
# End:
Obfuscation techniques are quite interesting, but now I am focusing on understanding the process of deobfuscation.




