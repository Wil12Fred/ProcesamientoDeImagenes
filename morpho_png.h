#include <iostream>

void dilate(int* Mask, int maskh, int maskw, int oy=-1, int ox=-1){
	if(ox==-1){
		oy=maskh/2;
		ox=maskw/2;
	}
	for (int y=0; y< *height; y++){
		png_bytep row = row_pointers[y];
		png_bytep row2 = write_rp[y];
		for (int x = 0; x<*width; x++){
			png_bytep px = &(row[x * size_pixel]);
			png_bytep px2 = &(row2[x*size_pixel]);
			px[0]=px[1]=px[2]=px2[0];
			px2[0]=px2[1]=px2[2]=255;
		}
	}
	for (int y=-maskh; y< *height+maskh; y++){
		if(y+oy>=0 && y+oy<*height){
			png_bytep row2 = write_rp[y+oy];
			for (int x = -maskw; x<*width+maskw; x++){
				if (x+ox>=0 && x+ox<*width){
					png_bytep px2 = &(row2[(x+ox) * size_pixel]);
					bool isAllPainted=false;
					for (int i=0;i<maskh;i++){
						int yy=y+i;//-ox;
						if(yy<*height){
							png_bytep row = row_pointers[yy];
							for (int j=0; j<maskw; j++){
								int xx=x+j;//-oy;
								if(xx<*width){
									png_bytep px = &(row[xx * size_pixel]);
									if(yy>=0 && yy<*height && xx>=0 && xx<*width){
										if(px[0]==0){
											isAllPainted=true;
										}
									} else {
										//isAllPainted=false;
									}
								}
							}
						}
					}
					if(isAllPainted){
						px2[0]=px2[1]=px2[2]=0;
					}
					//px2[0]=px2[1]=px2[2]=px[0];
				}
			}
		}
	}
}

void erosion(int* Mask, int maskh, int maskw, int oy=-1, int ox=-1){
	if(ox==-1){
		ox=maskh/2;
		oy=maskw/2;
	}
	for (int y=0; y< *height; y++){
		png_bytep row = row_pointers[y];
		png_bytep row2 = write_rp[y];
		for (int x = 0; x<*width; x++){
			png_bytep px = &(row[x * size_pixel]);
			png_bytep px2 = &(row2[x*size_pixel]);
			px[0]=px[1]=px[2]=px2[0];
			px2[0]=px2[1]=px2[2]=255;
		}
	}
	for (int y=0; y< *height; y++){
		if(y+oy<*height){
			png_bytep row2 = write_rp[y+oy];
			for (int x = 0; x<*width; x++){
				if (x+ox<*width){
					png_bytep px2 = &(row2[(x+ox) * size_pixel]);
					bool isAllPainted=true;
					for (int i=0;i<maskh;i++){
						int yy=y+i;//-ox;
						png_bytep row = row_pointers[yy];
						for (int j=0; j<maskw; j++){
							int xx=x+j;//-oy;
							png_bytep px = &(row[xx * size_pixel]);
							if(yy>=0 && yy<*height && xx>=0 && xx<*width){
								if(px[0]==255){
									isAllPainted=false;
								}
							} else {
								isAllPainted=false;
							}
						}
					}
					if(isAllPainted){
						px2[0]=px2[1]=px2[2]=0;
					}
					//px2[0]=px2[1]=px2[2]=px[0];
				}
			}
		}
	}
}

void opening(int* Mask, int maskh, int maskw, int oy=-1, int ox=-1){
	erosion(Mask, maskh, maskw, oy, ox);
	dilate(Mask, maskh, maskw, oy, ox);
}

void closing(int * Mask, int maskh, int maskw, int oy=-1, int ox=-1){
	dilate(Mask, maskh, maskw, oy, ox);
	erosion(Mask, maskh, maskw, oy, ox);
}

void dilateMinusErosion(int* Mask, int maskh, int maskw, int oy=-1, int ox=-1){
	if(ox==-1){
		ox=maskh/2;
		oy=maskw/2;
	}
	int dilation[*height][*width];
	int erosion[*height][*width];
	for (int y=0; y< *height; y++){
		png_bytep row = row_pointers[y];
		png_bytep row2 = write_rp[y];
		for (int x = 0; x<*width; x++){
			png_bytep px = &(row[x * size_pixel]);
			png_bytep px2 = &(row2[x*size_pixel]);
			px[0]=px[1]=px[2]=px2[0];
			dilation[y][x]=255;
		}
	}
	for (int y=-maskh; y< *height+maskh; y++){
		if(y+oy>=0 && y+oy<*height){
			for (int x = -maskw; x<*width+maskw; x++){
				if (x+ox>=0 && x+ox<*width){
					bool isAllPainted=false;
					for (int i=0;i<maskh;i++){
						int yy=y+i;//-ox;
						png_bytep row = row_pointers[yy];
						for (int j=0; j<maskw; j++){
							int xx=x+j;//-oy;
							png_bytep px = &(row[xx * size_pixel]);
							if(yy>=0 && yy<*height && xx>=0 && xx<*width){
								if(px[0]==0){
									isAllPainted=true;
								}
							} else {
								//isAllPainted=false;
							}
						}
					}
					if(isAllPainted){
						dilation[y+oy][x+ox]=0;
					}
					//px2[0]=px2[1]=px2[2]=px[0];
				}
			}
		}
	}
	for (int y=0; y< *height; y++){
		for (int x = 0; x<*width; x++){
			erosion[y][x]=255;
		}
	}
	for (int y=0; y< *height; y++){
		if(y+oy<*height){
			for (int x = 0; x<*width; x++){
				if (x+ox<*width){
					bool isAllPainted=true;
					for (int i=0;i<maskh;i++){
						int yy=y+i;//-ox;
						png_bytep row = row_pointers[yy];
						for (int j=0; j<maskw; j++){
							int xx=x+j;//-oy;
							png_bytep px = &(row[xx * size_pixel]);
							if(yy>=0 && yy<*height && xx>=0 && xx<*width){
								if(px[0]==255){
									isAllPainted=false;
								}
							} else {
								isAllPainted=false;
							}
						}
					}
					if(isAllPainted){
						erosion[y][x]=0;
					}
					//px2[0]=px2[1]=px2[2]=px[0];
				}
			}
		}
	}
	for (int y=0; y< *height; y++){
		png_bytep row = write_rp[y];
		for (int x = 0; x<*width; x++){
			png_bytep px = &(row[x * size_pixel]);
			if(dilation[y][x]==erosion[y][x]){
				px[0]=px[1]=px[2]=0;
			} else {
				px[0]=px[1]=px[2]=255;
			}
			int value=dilation[y][x]-erosion[y][x];
			//value=max(value,0);
			//value=min(value,255);
			px[0]=px[1]=px[2]=abs(value);
			//px[0]=px[1]=px[2]=dilation[y][x];
		}
	}
}
