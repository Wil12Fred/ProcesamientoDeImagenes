#include <png.h>

bool load_file(string file_name){
	png_byte header[8];
	FILE *fp=fopen(file_name.c_str(),"rb");
	if(!fp){
		perror(file_name.c_str());
		return 0;
	}
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8)){
		fprintf(stderr, "error: %s is not a PNG.\n", file_name.c_str());
		fclose(fp);
		return 0;
	}
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr){
		fprintf(stderr, "error: png_create_read_struct returned 0.\n");
		fclose(fp);
		return 0;
	}
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr){
		fprintf(stderr, "error: png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return 0;
	}
	png_infop end_info = png_create_info_struct(png_ptr);
	if(!end_info){
		fprintf(stderr, "error: png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return 0;
	}
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);
	png_uint_32 temp_width, temp_height;
	png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
			NULL, NULL, NULL);
	if (width){ *width = temp_width; }
	if (height){ *height = temp_height; }
	if (bit_depth != 8){
		fprintf(stderr, "%s: Unsupported bit depth %d.  Must be 8.\n", file_name.c_str(), bit_depth);
		return 0;
	}
	int format;
	switch(color_type){
		case PNG_COLOR_TYPE_RGB:
			format = PNG_COLOR_TYPE_RGB;
			size_pixel=3;
			//printf("PNG_COLOR_TYPE_RGB\n");
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			format = PNG_COLOR_TYPE_RGB_ALPHA;
			size_pixel=4;
			//printf("PNG_COLOR_TYPE_RGB_ALPHA\n");
			break;
		default:
			fprintf(stderr, "%s: Unknown libpng color type %d.\n", file_name.c_str(), color_type);
			return 0;
	}
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	image_rowbytes=rowbytes;
	rowbytes += 3 - ((rowbytes-1) % 4);
	png_byte * image_data = (png_byte *)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
	png_byte * image_data2 = (png_byte *)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
	if (image_data==NULL){
		fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 0;
	}
	row_pointers = (png_byte **)malloc(temp_height * sizeof(png_byte *));
	write_rp = (png_byte **)malloc(temp_height * sizeof(png_byte *));
	if (row_pointers == NULL) {
		fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		free(image_data);
		fclose(fp);
		return 0;
	}
	for (unsigned int i = 0; i < temp_height; i++) {
		row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
		write_rp[temp_height - 1 - i] = image_data2 + i*rowbytes;
	}
	png_read_image(png_ptr, row_pointers);
	for(int y = 0; y < *height; y++) {
		png_bytep row = row_pointers[y];
		png_bytep row2 = write_rp[y];
		for(int x = 0; x < *width; x++) {
			png_bytep px = &(row[x * size_pixel]);
			png_bytep px2 = &(row2[x * size_pixel]);
			px2[0]=px[0];
			px2[1]=px[1];
			px2[2]=px[2];
			if(size_pixel==4){
				px2[3]=px[3];
			}
		}
	}
	return 1;
}

void write_png_file(string new_file) {
	const char *filename=new_file.c_str();
	int y;
	FILE *fp = fopen(filename, "wb");
	if(!fp) abort();
	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) abort();
	png_infop info = png_create_info_struct(png);
	if (!info) abort();
	if (setjmp(png_jmpbuf(png))) abort();
	png_init_io(png, fp);
	// Output is 8bit depth, RGBA format.
	png_set_IHDR(
		png,
		info,
		*width, *height,
		8,
		color_type,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);
	png_write_info(png, info);
	// To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
	// Use png_set_filler().
	//png_set_filler(png, 0, PNG_FILLER_AFTER);
	png_write_image(png, write_rp);
	png_write_end(png, NULL);
	/*for(int y = 0; y < *height; y++) {
	free(row_pointers[y]);
	}
	free(row_pointers);*/
	fclose(fp);
	png_destroy_write_struct(&png, &info);
}
