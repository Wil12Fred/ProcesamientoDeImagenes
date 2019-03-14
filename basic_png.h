#include <iostream>
#include <fstream>
#include "fourier.h"

using namespace fourier;

void to_grayscale(){
	for(int y = 0; y < *height; y++) {
		png_bytep row = row_pointers[y];
		png_bytep row2 = write_rp[y];
		for(int x = 0; x < *width; x++) {
			png_bytep px = &(row[x * size_pixel]);
			png_bytep px2 = &(row2[x * size_pixel]);
			int promedio=(px[0]+px[1]+px[2])/3;
			px[0]=px[1]=px[2]=promedio;
			px2[0]=px2[1]=px2[2]=promedio;
			if(size_pixel==4){
				px2[3]=px[3];
			}
			//printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
			
		}
	}
}

void sum(string st){
	load_file(st);
	for(int y = 0; y < *height; y++) {
		png_bytep row = row_pointers[y];
		png_bytep row2 = write_rp[y];
		for(int x = 0; x < *width; x++) {
			png_bytep px = &(row[x * size_pixel]);
			png_bytep px2 = &(row2[x * size_pixel]);
			int value=px[0]+px2[0];
			if(value<0){
				value=0;
			}
			if(value>255){
				value=255;
			}
			px2[0]=px2[1]=px2[2]=value;
		}
	}
}

void to_white_and_black(int th=128){
	for(int y = 0; y < *height; y++) {
		png_bytep row = write_rp[y];
		for(int x = 0; x < *width; x++) {
			png_bytep px = &(row[x * size_pixel]);
			int value=px[0];
			if(value<th){
				px[0]=px[1]=px[2]=0;
			} else {
				px[0]=px[1]=px[2]=255;
			}
			//px[0]=px[1]=px[2]=(((value)*2)/256)*255;
		}
	}
}

void to_negative(){
	for(int y = 0; y < *height; y++) {
		png_bytep row = write_rp[y];
		for(int x = 0; x < *width; x++) {
			png_bytep px = &(row[x * size_pixel]);
			px[0]=255-px[0];
			px[1]=255-px[1];
			px[2]=255-px[2];
		}
	}
}

void to_log(double b=2, double c=0){
	for(int y = 0; y < *height; y++) {
		png_bytep row = write_rp[y];
		for(int x = 0; x < *width; x++) {
			png_bytep px = &(row[x * size_pixel]);
			int value=px[0];
			px[0]=px[1]=px[2]=max(int((log(double(value))/log(b))*c),0);
		}
	}
}

void to_pow(double e=2, double c=0){
	for(int y = 0; y < *height; y++) {
		png_bytep row = write_rp[y];
		for(int x = 0; x < *width; x++) {
			png_bytep px = &(row[x * size_pixel]);
			int value=px[0];
			px[0]=px[1]=px[2]=min(int(pow(double(value),e)*c),255);
		}
	}
}

void to_cpx(){
	maxn=1;
	while(maxn<max(*height, *width)){
		maxn<<=1;
	}
	write_cpx=new cpx[maxn*maxn];
	for(int y = 0; y < *height; y++) {
		png_bytep row = write_rp[y];
		for(int x = 0; x < *width; x++) {
			png_bytep px = &(row[x * size_pixel]);
			int value=px[0];
			write_cpx[y*maxn+x]=cpx(double(value)/**pow(-1,x+y)*/,0);
		}
		for (int x=*width;x<maxn;x++){
			write_cpx[y*maxn+x]=cpx(0,0);
		}
	}
	for (int y=*height; y<maxn; y++){
		for (int x=0; x<maxn; x++){
			write_cpx[y*maxn+x]=cpx(0,0);
		}
	}
}

void complex_to_real(cpx* Y){
	for(int y = 0; y < *height; y++) {
		png_bytep row = row_pointers[y];
		for(int x = 0; x < *width; x++) {
			png_bytep px = &(row[x * size_pixel]);
			int value= //Y[y*maxn+x].real();
				//int(sqrt(Y[y*maxn+x].real()*Y[y*maxn+x].real()+Y[y*maxn+x].imag()*Y[y*maxn+x].imag()));
				abs(Y[y*maxn+x]);
			value=max(0,value);
			value=min(255,value);
			px[0]=px[1]=px[2]=value;
		}
	}
}

void complex_to_image(cpx* Y){
	for(int y = 0; y < *height; y++) {
		png_bytep row = write_rp[y];
		for(int x = 0; x < *width; x++) {
			png_bytep px = &(row[x * size_pixel]);
			int value= //Y[y*maxn+x].real();
				//int(sqrt(Y[y*maxn+x].real()*Y[y*maxn+x].real()+Y[y*maxn+x].imag()*Y[y*maxn+x].imag()));
				sqrt(abs(Y[y*maxn+x]));
			value=max(0,value);
			value=min(255,value);
			px[0]=px[1]=px[2]=value;
		}
	}
}

void to_fourier(/*cpx*& y*/){
	to_cpx();
	tfourier=new cpx[maxn*maxn];
	fft2d(write_cpx,tfourier,maxn,1);
}

void fourier_to_image(/*cpx* y*/){
	fft2d(tfourier,write_cpx,maxn,-1);
	for(int y = 0; y < *height; y++) {
		png_bytep row = write_rp[y];
		for(int x = 0; x < *width; x++) {
			png_bytep px = &(row[x * size_pixel]);
			px[0]=px[1]=px[2]=max(0,int(write_cpx[y*maxn+x].real()/**pow(-1,x+y)*/));
			px[0]=px[1]=px[2]=min(255,int(px[0]));
		}
	}
}

void print_histogram(std::string file){
	int F[3][255];
	for (int i=0;i<256;i++){
		F[0][i]=F[1][i]=F[2][i]=0;
	}
	for(int y = 0; y < *height; y++) {
		png_bytep row = row_pointers[y];
		for(int x = 0; x < *width; x++) {
			png_bytep px = &(row[x * size_pixel]);
			F[0][int(px[0])]++;
			F[1][int(px[1])]++;
			F[2][int(px[2])]++;
		}
	}
	ofstream myfile;
	myfile.open(file.c_str());
	myfile << "I;FRed;FGreen;FBlue\n";
	for (int i=0;i<256;i++){
		myfile << i << "; ";
		myfile << F[0][i] << "; ";
		myfile << F[1][i] << "; ";
		myfile << F[2][i] << "\n";
	}
	myfile.close();
}

void print_hsi(std::string file){
	ofstream myfile;
	myfile.open(file.c_str());
	myfile << "H;S;I\n";
	for(int y = 0; y < *height; y++) {
		png_bytep row = row_pointers[y];
		for(int x = 0; x < *width; x++) {
			png_bytep px = &(row[x * size_pixel]);
			double r=int(px[0]);
			double g=int(px[1]);
			double b=int(px[2]);
			double H;
			if(((r-g)*(r-g)+(r-b)*(r-b))==0){
				H=0;
			} else {
				H = acos(((r-g)/2+(r-b))/((r-g)*(r-g)+(r-b)*(r-b)));
			}
			if(b<=r){
			} else {
				H=2.0*(acos(-1.0))-H;
			}
			r/=255;
			g/=255;
			b/=255;
			double I=(r+g+b)/3;
			double S=1-min(r,min(g,b))*(1.0/I);
			myfile << H << "; ";
			myfile << S << "; ";
			myfile << I << "\n";
		}
	}
	myfile.close();
}
