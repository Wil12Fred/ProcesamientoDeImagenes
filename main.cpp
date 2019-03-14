#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <algorithm>
//#include <QtWidgets>
#include <png.h>
//#include "addressbook.h"
#include "functions_png.h"

int main(int argc, char *argv[]) {
	load_file("camahorizontal.png");
	to_white_and_black();
	sum("camavertical.png");
	sum("cama45p.png");
	sum("cama45n.png");
	write_png_file("sumlena.png");
	/*string file_name;
	cin >> file_name;
	if (!load_file(file_name))
		return 0;
	load_file(file_name);
	//to_grayscale();
	//to_white_and_black();
	//int Mask[4];
	//erosion(Mask,4,4);
	//write_png_file("dilate_"+file_name);
	to_grayscale();//row_pointers->write_rp
	//TFourier
	to_fourier();//write_rp->write_cpx->tfourier
	complex_to_image(tfourier);
	write_png_file("fourier_"+file_name);
	//filter_fourier();
	//complex_to_image(tfourier);
	//write_png_file("fourier_"+file_name);
	filter_wiener(0.3);//tfourier->tfourier
	complex_to_image(tfourier);
	write_png_file("wiener_"+file_name);
	fourier_to_image();//tfourier->write_cpx->write_rp
	complex_to_real(write_cpx);//write_cpx->row_pointers
	//write_png_file("ifourier_"+file_name);
	filter_mediana(2);//row_pointers->write_rp
	write_png_file("mediana_"+file_name);//write_rp->png
	to_white_and_black(100);//write_rp->write_rp
	write_png_file("w+b_"+file_name);//write_rp->png
	int Mask[3];
	//erosion(Mask,5,5);
	//dilate(Mask,3,3);
	dilateMinusErosion(Mask,2,2);
	//write_png_file("erosion_"+file_name);//write_rp->write_rp
	//dilate(Mask,2,2);
	//write_png_file("dilate_"+file_name);//write_rp->write_rp
	write_png_file("paperImplementation_"+file_name);//write_rp->write_rp
	load_file(file_name);
	to_grayscale();
	filter_sobel();
	write_png_file("sobel_"+file_name);//write_rp->write_rp*/
	/*QApplication app(argc, argv);

	AddressBook addressBook;

	addressBook.show();

	return app.exec();*/
}
