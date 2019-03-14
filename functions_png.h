#include <complex>

using namespace std;

int *width=new int, *height=new int;
int image_rowbytes;
int maxn;
int bit_depth, color_type;
png_byte ** row_pointers;
png_byte ** write_rp;
complex<double>* write_cpx;
complex<double>* tfourier;
int size_pixel;

#include "io_png.h"
#include "basic_png.h"
#include "filter_png.h"
#include "morpho_png.h"
