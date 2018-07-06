#include "stdafx.h"
#include "JpegProcessor.h"

#include <iostream>
#include <vector>
#include <memory>

#include "jpeglib.h"

CJpegProcessor::CJpegProcessor(const std::string & inFile, const std::string & outFile) :
	m_inFile(inFile),
	m_outFile(outFile)
{
}


CJpegProcessor::~CJpegProcessor()
{
	for (auto &line : m_lines)
		free(line);
}

int CJpegProcessor::read_JPEG_file()
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr pub;

	cinfo.err = jpeg_std_error(&pub);

	FILE * infile;
	fopen_s(&infile, m_inFile.c_str(), "rb");
	if (!infile)
		return -1;

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	(void)jpeg_read_header(&cinfo, TRUE);
	(void)jpeg_start_decompress(&cinfo);

	m_width = cinfo.output_width;
	m_height = cinfo.output_height;

	int row_stride = m_width * cinfo.output_components;

	JSAMPROW buffer = (JSAMPROW)malloc(sizeof(JSAMPLE) * row_stride);
	while (cinfo.output_scanline < cinfo.output_height)
	{
		(void)jpeg_read_scanlines(&cinfo, &buffer, 1);

		unsigned char *line = new unsigned char[sizeof(JSAMPLE) * row_stride];
		memcpy(line, buffer, sizeof(JSAMPLE) * row_stride);

		m_lines.push_back(line);
	}
	free(buffer);

	(void)jpeg_finish_decompress(&cinfo);

	fclose(infile);

	return 0;

}

int CJpegProcessor::write_JPEG_file(int quality)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	FILE * outfile;
	fopen_s(&outfile, m_outFile.c_str(), "wb");
	if (!outfile)
		return -1;

	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = m_width;
	cinfo.image_height = m_height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);

	jpeg_start_compress(&cinfo, TRUE);

	for (auto & line : m_lines)
		(void)jpeg_write_scanlines(&cinfo, &line, 1);

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	fclose(outfile);

	return 0;
}