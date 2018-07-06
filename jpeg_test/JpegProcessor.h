#pragma once

#include <string>
#include <vector>

class CJpegProcessor
{
public:
	CJpegProcessor(const std::string & inFile, const std::string & outFile);
	~CJpegProcessor();

	int read_JPEG_file();
	int write_JPEG_file(int quality);

private:
	std::vector <unsigned char *> m_lines;
	long m_width;
	long m_height;

	std::string m_inFile;
	std::string m_outFile;
};

