#include <iostream>
#include <time.h>
using uint8_t = unsigned char;
const uint8_t SIZE = 27;
const char alph[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

size_t maxLength(char** begin, char** end) {
	size_t result = 0;
	char** currentElem = begin;
	while (currentElem != end) {
		size_t length = 0;
		const char* sign = *currentElem;
		while (*sign) {
			++length;
			++sign;
		}
		if (length > result) {
			result = length;
		}
		++currentElem;
	}
	return result;
}

void printStringArray(char** str, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		std::cout << str[i] << ' ';
		std::cout << i << ' ';
	}
	std::cout << '\n';
}

void radixSort(char** begin, char** end, size_t radix, size_t maxStrLen) {
	if (radix == maxStrLen) {
		return;
	}
	size_t indices[SIZE]{};
	{
		size_t chars[SIZE]{};
		char** currentElem = begin;
		while (currentElem != end + 1) {
			uint8_t buff = std::tolower(*(*currentElem + radix));
			if (buff >= 'a' && buff <= 'z') {
				chars[buff - 'a' + 1] += 1;
			}
			else if (buff == '\0') {
				++chars[0];
			}
			else {
				throw std::logic_error("Incorret char!");
			}
			++currentElem;
		}
		indices[0] = chars[0];

		for (uint8_t i = 1; i < SIZE; ++i) {
			if (chars[i] == 0) {
				indices[i] = indices[i - 1];
			}
			else {
				indices[i] = chars[i] + indices[i - 1];
			}
		}

		char** buffArray = new char* [end - begin + 1] {};
		currentElem = begin;
		while (currentElem != end) {
			uint8_t letter = std::tolower(*(*currentElem + radix));
			if (letter) {
				size_t position = letter - 'a' + 1;
				buffArray[indices[position] - chars[position]] = *currentElem;
				--chars[position];
			}
			else {
				buffArray[indices[0] - chars[0]] = *currentElem;
				--chars[0];
			}
			++currentElem;
		}
		uint8_t letter = std::tolower(*(*currentElem + radix));
		if (letter) {
			size_t position = letter - 'a' + 1;
			buffArray[indices[position] - chars[position]] = *currentElem;
			--chars[position];
		}
		else {
			buffArray[indices[0] - chars[0]] = *currentElem;
			--chars[0];
		}

		currentElem = begin;
		while (currentElem != end) {
			*currentElem = buffArray[currentElem - begin];
			++currentElem;		
		}
		*currentElem = buffArray[currentElem - begin];

		delete[] buffArray;
	}

	for (uint8_t i = 1; i < SIZE; ++i) {
		if ((indices[i - 1] != indices[i] - 1) && (indices[i - 1] != indices[i])) {
			radixSort(begin + indices[i - 1], begin + indices[i] - 1, radix + 1, maxStrLen);
		}
	}
}

void radixSort(char** begin, char** end, size_t maxStrLen) {
	if (begin == end) {
		return;
	}
	try {
		radixSort(begin, end, 0, maxStrLen);
	}
	catch(const std::exception& err) {
		throw;
	}
}

bool isGreaterString(char* str1, char* str2) {
	bool isGreater = false;
	bool isEqual = true;
	const char* ptr1 = str1;
	const char* ptr2 = str2;
	while (*ptr1 && *ptr2 && isEqual) {
		if (std::tolower(*ptr1) > std::tolower(*ptr2)) {
			isGreater = true;
			isEqual = false;
		}
		else if (std::tolower(*ptr1) < std::tolower(*ptr2)) {
			isGreater = false;
			isEqual = false;
		}
		else {
			++ptr1;
			++ptr2;
		}
	}
	if (isEqual && (*ptr1 && !*ptr2)) {
		isGreater = true;
	}
	return isGreater;
}

bool isSorted(char** str, size_t size) {
	bool isSorted = true;
	for (size_t i = 1; i < size && isSorted; ++i) {
		isSorted = !isGreaterString(str[i - 1], str[i]);
	}
	return isSorted;
}

void checkFunction(size_t size, size_t maxArraySize, size_t step, size_t expectedMaxStrLen) {
	srand(time(NULL));
	for (;size <= maxArraySize; size += step) {
		if (size == 0) {
			continue;
		}
		std::cout << "Size of array: " << size << '\n';
		char** str = new char* [size];
		uint8_t maxStrSize = 0;
		for (size_t i = 0; i < size; ++i) {
			uint8_t strSize = rand() % (expectedMaxStrLen+1) + 1;
			if (strSize > maxStrSize) {
				maxStrSize = strSize;
			}
			str[i] = new char[strSize];
			str[i][strSize - 1] = '\0';
			for (size_t j = 0; j < strSize - 1; ++j) {
				uint8_t newChar = rand() % 52;
				str[i][j] = alph[newChar];
			}
		}
		std::cout << "Maximum size of string: " << maxStrSize - 1 << '\n';
		try {
			double time_spent = 0.0;
			clock_t begin = clock();
			radixSort(str, str + size - 1, maxStrSize - 1);
			clock_t end = clock();
			time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
			std::cout << "Array is sorted: " << isSorted(str, size) << '\n';
			std::cout << "Function was working for: " << time_spent << '\n';
		}
		catch (...) {
			std::cerr << "Error during sorting!\n";
		}
		for (size_t j = 0; j < size; ++j) {
			delete[] str[j];
		}
		delete[] str;
		std::cout << '\n';
	}
}

void checkWorst(size_t size, size_t maxArraySize, size_t step, size_t maxStrLen) {
	srand(time(NULL));
	for (; size <= maxArraySize; size += step) {
		if (size == 0) {
			continue;
		}
		std::cout << "Size of array: " << size << '\n';
		char** str = new char* [size];
		for (size_t i = 0; i < size; ++i) {
			str[i] = new char[maxStrLen + 1];
			str[i][maxStrLen] = '\0';
			for (size_t j = 0; j < maxStrLen; ++j) {
				str[i][j] = alph[0];
			}
		}
		std::cout << "Maximum size of string: " << maxStrLen << '\n';
		double time_spent = 0.0;
		clock_t begin = clock();
		try {
			radixSort(str, str + size - 1, maxStrLen);
			clock_t end = clock();
			time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
			std::cout << "Array is sorted: " << isSorted(str, size) << '\n';
			std::cout << "Function was working for: " << time_spent << '\n';
		}
		catch (...) {
			std::cerr << "Error during sorting!\n";
		}
		for (size_t j = 0; j < size; ++j) {
			delete[] str[j];
		}
		delete[] str;
		std::cout << '\n';
	}
}

int main() {
	std::cout << "Average cases:\n" << "--------------------------------------------------\n";
	checkFunction(0, 100000, 10000, 100);

	std::cout << "Best cases:\n" << "--------------------------------------------------\n";
	checkFunction(0, 100000, 10000, 1);

	std::cout << "Worst cases:\n" << "--------------------------------------------------\n";
	checkWorst(0, 100000, 10000, 100);
	return 0;
}
