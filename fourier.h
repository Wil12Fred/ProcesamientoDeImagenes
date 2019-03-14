#include <cmath>
#include <complex>

namespace fourier{

using namespace std;

typedef complex<double> cpx;
const double dos_pi=2.0*(acos(-1.0));

void fft(cpx x[], cpx y[], /*int dx,*/ int N, int dir){
	dir*=-1;
	if (N > 1){
		cpx x1[N];
		cpx y1[N];
		for (int i=0;i<N/2;i++){
			x1[i]=x[i*2];
			x1[i+N/2]=x[i*2+1];
		}
		fft(x1, y1, /*2*dx,*/ N/2, dir*-1);//even
		fft(x1+/*dx*/N/2, y1+N/2, /*2*dx,*/ N/2, dir*-1);//odd
		for (int i=0; i<N/2; i++){
			cpx even = y1[i], 
				odd = y1[i+(N/2)],
				twiddle=exp(cpx(0, (dir*dos_pi*i)/N));//e^((2*pi*i)/N)
			y[i] = even+twiddle*odd;//R
			y[i+N/2] = even-twiddle*odd;//R
		}
	} else {
		y[0] = x[0];
	}
}

void fft(cpx *x, cpx *y, int dx, int N, int dir){
	dir*=-1;
	if (N > 1){
		fft(x, y, 2*dx, N/2, dir*-1);//even
		fft(x+dx, y+N/2, 2*dx, N/2, dir*-1);//odd
		for (int i=0; i<N/2; i++){
			cpx even = y[i], 
				odd = y[i+(N/2)],
				twiddle=exp(cpx(0, (dir*dos_pi*i)/N));//e^((2*pi*i)/N)
			y[i] = even+twiddle*odd;//R
			y[i+N/2] = even-twiddle*odd;//R
		}
	} else {
		y[0] = x[0];
	}
}

void fft2d(cpx* x, cpx* y, int N, int dir){
	dir*=-1;
	cpx* fxv = new cpx[N*N];
	cpx* fvx = new cpx[N*N];
	for (int i=0;i<N;i++){//para x
		fft(x+i*N, fxv+i*N, 1, N, dir*-1);
	}
	for (int i=0;i<N;i++){//v
		for (int j=0;j<N;j++){//x
			fvx[i*N+j]=fxv[i+j*N];
		}
	}
	cpx* y2 = new cpx[N*N];
	for (int i=0;i<N;i++){//para v
		fft(fvx+i*N,y2+i*N,1,N,dir*-1);
	}
	for (int i=0;i<N;i++){
		for (int j=0;j<N;j++){
			y[i*N+j]=y2[i+j*N];
			if(dir==1){//antitransformada
				y[i*N+j]/=cpx(N*N,0);
			}
		}
	}
}

}//namespace
