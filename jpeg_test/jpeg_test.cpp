#include "stdafx.h"

#include "JpegProcessor.h"

#include <iostream>

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		std::cout << "Wrone number of arguments" << std::endl;
		return -1;
	}

	CJpegProcessor processor(argv[1], argv[2]);
	if (processor.read_JPEG_file() < 0)
	{
		std::cout << "Can't find input file" << std::endl;
		return -1;
	}

	if (processor.write_JPEG_file(std::stoi(argv[3])) < 0)
	{
		std::cout << "Can't open output file" << std::endl;
		return -1;
	}



	return 0;
}