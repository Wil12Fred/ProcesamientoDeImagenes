#define PI acos(-1)

void filter_media(int tam=3, double ponderado=1){
	for (int y=0;y<*height-tam+1;y++){
		png_bytep row = write_rp[y+tam/2];
		for (int x=0;x<*width-tam+1;x++){
			png_bytep px = &(row[(x+tam/2) * size_pixel]);
			double value=0;
			for (int yy=0;yy<tam;yy++){
				for (int xx=0;xx<tam;xx++){
					if(yy==tam/2 && xx==tam/2){
						value+=ponderado*int(row_pointers[y+yy][(x+xx)*size_pixel]);
					} else {
						value+=int(row_pointers[y+yy][(x+xx)*size_pixel]);
					}
				}
			}
			int new_value=int(value/(tam*tam-1+ponderado));
			if(new_value<0){
				new_value=0;
			} else if(new_value>255){
				new_value=255;
			}
			px[0]=px[1]=px[2]=new_value;
		}
	}
}

void filter_mediana(int tam=3){
	int V[tam*tam];
	for (int y=0;y<*height-tam+1;y++){
		png_bytep row = write_rp[y+tam/2];
		for (int x=0;x<*width-tam+1;x++){
			png_bytep px = &(row[(x+tam/2) * size_pixel]);
			for (int yy=0;yy<tam;yy++){
				for (int xx=0;xx<tam;xx++){
					V[xx*tam+yy]=row_pointers[y+yy][(x+xx)*size_pixel];
				}
			}
			sort(V,V+(tam*tam));
			px[0]=px[1]=px[2]=V[(tam*tam)/2];
		}
	}
}

void filter_sobel(){
	int Sx[3][3]={1,0,-1,
			2,0,-2,
			1,0,-1};
	int Sy[3][3]={-1,-2,-1,
			0,0,0,
			1,2,1};
	int tam=3;
	double max=0;
	for (int y=0;y<*height-tam+1;y++){
		png_bytep row = write_rp[y+tam/2];
		for (int x=0;x<*width-tam+1;x++){
			png_bytep px = &(row[(x+tam/2) * size_pixel]);
			double valuex=0,valuey=0;
			for (int yy=0;yy<tam;yy++){
				for (int xx=0;xx<tam;xx++){
					valuex+=Sx[yy][xx]*int(row_pointers[y+yy][(x+xx)*size_pixel]);
					valuey+=Sy[yy][xx]*int(row_pointers[y+yy][(x+xx)*size_pixel]);
				}
			}
			double value=sqrt(valuex*valuex+valuey*valuey)/4;
			if(value>max){
				max=value;
			}
			double angle;
			if(valuex!=0 || valuey!=0){
				angle=atan2(valuey,valuex)*180/PI;
			} else {
				angle=0;
			}
			if((angle>-22.5 && angle<=22.5) || (angle>157.5 && angle<=-157.5)){
			}
			int new_value=sqrt(valuex*valuex+valuey*valuey);
			if(new_value<0){
				new_value=0;
			} else if(new_value>255){
				new_value=255;
			}
			px[0]=px[1]=px[2]=new_value;
		}
	}
}

void filter_roberts(){
	int Sx[3][3]={0,0,0,
			0,1,0,
			0,0,-1};
	int Sy[3][3]={0,0,0,
			0,0,1,
			0,-1,0};
	int tam=3;
	for (int y=0;y<*height-tam+1;y++){
		png_bytep row = write_rp[y+tam/2];
		for (int x=0;x<*width-tam+1;x++){
			png_bytep px = &(row[(x+tam/2) * size_pixel]);
			double valuex=0,valuey=0;
			for (int yy=0;yy<tam;yy++){
				for (int xx=0;xx<tam;xx++){
					valuex+=Sx[yy][xx]*int(row_pointers[y+yy][(x+xx)*size_pixel]);
					valuey+=Sy[yy][xx]*int(row_pointers[y+yy][(x+xx)*size_pixel]);
				}
			}
			int new_value=sqrt(valuex*valuex+valuey*valuey);
			if(new_value<0){
				new_value=0;
			} else if(new_value>255){
				new_value=255;
			}
			px[0]=px[1]=px[2]=new_value;
		}
	}
}

void filter_laplace(){
	int Sx[3][3]={-1,-1,-1,
			-1,8,-1,
			-1,-1,-1};
	int tam=3;
	for (int y=0;y<*height-tam+1;y++){
		png_bytep row = write_rp[y+tam/2];
		for (int x=0;x<*width-tam+1;x++){
			png_bytep px = &(row[(x+tam/2) * size_pixel]);
			double value=0;
			for (int yy=0;yy<tam;yy++){
				for (int xx=0;xx<tam;xx++){
					value+=Sx[yy][xx]*int(row_pointers[y+yy][(x+xx)*size_pixel]);
				}
			}
			int new_value=value;
			if(new_value<0){
				new_value=0;
			} else if(new_value>255){
				new_value=255;
			}
			px[0]=px[1]=px[2]=new_value;
		}
	}
}

void filter_gaussian(){
	int Sx[5][5]={1,4,7,4,1,
			4,16,26,16,4,
			7,26,41,26,7,
			4,16,26,16,4,
			1,4,7,4,1};
	int tam=5;
	for (int y=0;y<*height-tam+1;y++){
		png_bytep row = write_rp[y+tam/2];
		for (int x=0;x<*width-tam+1;x++){
			png_bytep px = &(row[(x+tam/2) * size_pixel]);
			double value=0;
			for (int yy=0;yy<tam;yy++){
				for (int xx=0;xx<tam;xx++){
					value+=(double(Sx[yy][xx])/273)*int(row_pointers[y+yy][(x+xx)*size_pixel]);
				}
			}
			int new_value=value;
			if(new_value<0){
				new_value=0;
			} else if(new_value>255){
				new_value=255;
			}
			px[0]=px[1]=px[2]=new_value;
		}
	}
}

void nonMaxSupperssion(){
	float pixel_1 = 0;
	float pixel_2 = 0;
	float pixel;
	for (int x=1; x<*height-1; x++){
		for (int y=1; y<*width-1; y++){
			
		}
	}
}

void filter_canny(){
	filter_gaussian();
	for(int y = 0; y < *height; y++) {
		png_bytep row = row_pointers[y];
		png_bytep row2 = write_rp[y];
		for(int x = 0; x < *width; x++) {
			png_bytep px = &(row[x * size_pixel]);
			png_bytep px2 = &(row2[x * size_pixel]);
			px[0]=px[1]=px[2]=px2[0];
		}
	}
	filter_sobel();
}


void filter_horizontal(){
	int Sx[3][3]={-1,-1,-1,
			2,2,2,
			-1,-1,-1};
	int tam=3;
	for (int y=0;y<*height-tam+1;y++){
		png_bytep row = write_rp[y+tam/2];
		for (int x=0;x<*width-tam+1;x++){
			png_bytep px = &(row[(x+tam/2) * size_pixel]);
			double value=0;
			for (int yy=0;yy<tam;yy++){
				for (int xx=0;xx<tam;xx++){
					value+=Sx[yy][xx]*int(row_pointers[y+yy][(x+xx)*size_pixel]);
				}
			}
			int new_value=value;
			if(new_value<0){
				new_value=0;
			} else if(new_value>255){
				new_value=255;
			}
			px[0]=px[1]=px[2]=new_value;
		}
	}
}

void filter_45p(){
	int Sx[3][3]={-1,-1,2,
			-1,2,-1,
			2,-1,-1};
	int tam=3;
	for (int y=0;y<*height-tam+1;y++){
		png_bytep row = write_rp[y+tam/2];
		for (int x=0;x<*width-tam+1;x++){
			png_bytep px = &(row[(x+tam/2) * size_pixel]);
			double value=0;
			for (int yy=0;yy<tam;yy++){
				for (int xx=0;xx<tam;xx++){
					value+=Sx[yy][xx]*int(row_pointers[y+yy][(x+xx)*size_pixel]);
				}
			}
			int new_value=value;
			if(new_value<0){
				new_value=0;
			} else if(new_value>255){
				new_value=255;
			}
			px[0]=px[1]=px[2]=new_value;
		}
	}
}

void filter_vertical(){
	int Sx[3][3]={-1,2,-1,
			-1,2,-1,
			-1,2,-1};
	int tam=3;
	for (int y=0;y<*height-tam+1;y++){
		png_bytep row = write_rp[y+tam/2];
		for (int x=0;x<*width-tam+1;x++){
			png_bytep px = &(row[(x+tam/2) * size_pixel]);
			double value=0;
			for (int yy=0;yy<tam;yy++){
				for (int xx=0;xx<tam;xx++){
					value+=Sx[yy][xx]*int(row_pointers[y+yy][(x+xx)*size_pixel]);
				}
			}
			int new_value=value;
			if(new_value<0){
				new_value=0;
			} else if(new_value>255){
				new_value=255;
			}
			px[0]=px[1]=px[2]=new_value;
		}
	}
}

void filter_45n(){
	int Sx[3][3]={2,-1,-1,
			-1,2,-1,
			-1,-1,2};
	int tam=3;
	for (int y=0;y<*height-tam+1;y++){
		png_bytep row = write_rp[y+tam/2];
		for (int x=0;x<*width-tam+1;x++){
			png_bytep px = &(row[(x+tam/2) * size_pixel]);
			double value=0;
			for (int yy=0;yy<tam;yy++){
				for (int xx=0;xx<tam;xx++){
					value+=Sx[yy][xx]*int(row_pointers[y+yy][(x+xx)*size_pixel]);
				}
			}
			int new_value=value;
			if(new_value<0){
				new_value=0;
			} else if(new_value>255){
				new_value=255;
			}
			px[0]=px[1]=px[2]=new_value;
		}
	}
}

void filter_fourier(){
	/*****************************
	*       CONVOLUCION          *
	******************************/
	/*double g[3][3]={0,1,0,
				0,1,0,
				0,1,0};*/
	double g[3][3]={1.0/16,2.0/16,1.0/16,
				2.0/16,4.0/16,2.0/16,
				1.0/16,2.0/16,1.0/16};
	/*double g[3][3]={1,2,1,
				0,0,0,
				-1,-1,-1};*/
	/*double g[3][3]={-1,-1,-1,
			-1,8,-1,
			-1,-1,-1};*/
	/*double g[3][3]={0.35,0.5,0.35,
			0.5,1,0.5,
			0.35,0.5,0.35};*/
	/*double g[3][3]={1,1,1,
			1,1,1,
			1,1,1};*/
	/*double g[3][3]={1.0/2,1.0/2,1.0/2,
			1.0/2,1.0/2,1.0/2,
			1.0/2,1.0/2,1.0/2};*/
	int tam=maxn;
	cpx *G,*TG,*tf;
	G = new cpx[tam*tam];
	for (int i=0;i<tam;i++){
		for (int j=0;j<tam;j++){
			if(i<3 && j<3){
				G[i*tam+j]=cpx(g[i][j],0);
			} else {
				G[i*tam+j]=cpx(0,0);
			}
			//G[i*tam+j]=g[i/(tam/3)][j/(tam/3)];
		}
	}
	TG = new cpx[tam*tam];
	fft2d(G,TG,tam,1);
	/***********************
	*         FILTRO       *
	************************/
	//maxn-tam-1+(tam-1)=maxn-2
	//tf = new cpx[maxn*maxn];
	for (int i=0;i<tam;i++){
		for (int j=0;j<tam;j++){
			tfourier[i*tam+j]=(tfourier[i*tam+j])*(TG[i*tam+j]);
		}
	}
}

void multiply(cpx *F, cpx *S, cpx* R){//R
	for (int i=0; i<maxn; i++){
		for (int j=0; j<maxn; j++){
			R[i*maxn+j]=0;
			for (int k=0; k<maxn; k++){
				R[i*maxn+j]+=F[i*maxn+k]*S[k*maxn+j];
			}
		}
	}
}

void pow2(cpx *B, cpx *R){//R
	multiply(B, B, R);
}

void add(cpx *M, double y){//R
	for (int i=0; i<maxn; i++){
		for (int j=0; j<maxn; j++){
			M[i*maxn+j]+=cpx(y,0);
		}
	}
}

void conj(cpx *M, cpx *CM){//R
	for (int i=0; i<maxn; i++){
		for (int j=0; j<maxn; j++){
			CM[i*maxn+j]=cpx(M[i*maxn+j].real(), - M[i*maxn+j].imag());
		}
	}
}

void abs(cpx *M, cpx *AM){//R
	for (int i=0; i<maxn; i++){
		for (int j=0; j < maxn; j++){
			AM[i*maxn+j] = abs(M[i*maxn+j].real());
		}
	}
}

const double EPS = 1e-7;

void inv(cpx *M){
	vector<int> irow(maxn), icol(maxn), ipiv(maxn);
	for (int i=0;i<maxn;i++){
		int pj=-1, pk=-1;
		for (int j=0; j<maxn; j++) if(!ipiv[j])
			for (int k=0; k<maxn; k++) if(!ipiv[k])
				if(pj == -1 || abs(M[j*maxn+k])>abs(M[pj*maxn+pk])){
					pj = j;
					pk = k;
				}
		if(abs(M[pj*maxn+pk]) < EPS){
			return;
		}
		ipiv[pk]++;
		for (int j=0;j<maxn;j++){
			swap(M[pj*maxn+j],M[pk*maxn+j]);
		}
		irow[i]=pj;
		icol[i]=pk;
		cpx c= cpx(1.0,0)/M[pk*maxn+pk];
		M[pk*maxn+pk]=cpx(1.0,0);
		for (int p=0; p< maxn; p++) M[pk*maxn+p] *=c;
		for (int p=0; p< maxn; p++) if(p!=pk){
			c=M[p*maxn+pk];
			M[p*maxn+pk]=0;
			for (int q=0; q<maxn; q++){
				M[p*maxn+q]-=M[pk*maxn+q] * c;
			}
		}
	}
	for (int p=maxn-1; p>=0; p--) if(irow[p]!=icol[p]){
		for (int k=0; k<maxn;k++){
			swap(M[k*maxn+irow[p]], M[k*maxn+icol[p]]);
		}
	}
}

void set(cpx* A, cpx* B){
	for (int i=0;i<maxn;i++){
		for (int j=0;j<maxn;j++){
			B[i*maxn+j]=A[i*maxn+j];
		}
	}
}

void WnrDeconvolutionFilter(cpx *H, cpx* W, double k){
	cpx * Hconj = new cpx[maxn * maxn];
	conj(H, Hconj);

	cpx * Ha = new cpx[maxn * maxn];
	abs(H, Ha);

	/*for (int i=0;i<maxn;i++){
		for (int j=0;j<maxn;j++){
			Hapow[i*maxn+j]=Ha[i*maxn+j] * Ha[i*maxn+j];
		}
	}*/
	cpx * Hconj2 = new cpx[maxn * maxn];
	conj(H, Hconj2);
	cpx * Hapow = new cpx[maxn * maxn];
	//pow2(Ha, Hapow);
	for (int i=0;i<maxn;i++){
		for (int j=0;j<maxn;j++){
			//Hapow[i*maxn+j]=Ha[i*maxn+j]*Ha[i*maxn+j];//Hconj2[i*maxn+j] * H[i*maxn+j];
			Hapow[i*maxn+j]=Ha[i*maxn+j] * Ha[i*maxn+j];
		}
	}
	add(Hapow, k);

	/*cpx * IHapow = new cpx[maxn * maxn];
	set(Hapow, IHapow);
	inv(IHapow);
	//
	multiply(Hconj, IHapow, W);*/
	for (int i=0;i<maxn;i++){
		for (int j=0;j<maxn;j++){
			W[i*maxn+j] = Hconj[i*maxn+j] / Hapow[i*maxn+j];
		}
	}
}

void filter_wiener(double K=0.1){
	/*****************************
	*       CONVOLUCION          *
	******************************/
	/*double g[3][3]={0,1,0,
				0,1,0,
				0,1,0};*/
	double g[3][3]={1.0/16,2.0/16,1.0/16,
				2.0/16,4.0/16,2.0/16,
				1.0/16,2.0/16,1.0/16};
	/*double g[3][3]={1.0/16,2.0/16,1.0/16,
				2.0/16,4.0/16,2.0/16,
				1.0/16,2.0/16,1.0/16}*/;
	/*double g[3][3]={1,2,1,
				0,0,0,
				-1,-1,-1};*/
	/*double g[3][3]={-1,-1,-1,
			-1,8,-1,
			-1,-1,-1};*/
	/*double g[3][3]={0.35,0.5,0.35,
			0.5,1,0.5,
			0.35,0.5,0.35};*/
	/*double g[3][3]={1,1,1,
			1,1,1,
			1,1,1};*/
	/*double g[3][3]={1.0/2,1.0/2,1.0/2,
			1.0/2,1.0/2,1.0/2,
			1.0/2,1.0/2,1.0/2};*/
	int tam=maxn;
	cpx *G,*TG,*tf;
	G = new cpx[tam*tam];
	for (int i=0;i<tam;i++){
		for (int j=0;j<tam;j++){
			if(i<3 && j<3){
				G[i*tam+j]=cpx(g[i][j],0);
			} else {
				G[i*tam+j]=cpx(0,0);
			}
			//G[i*tam+j]=g[i/(tam/3)][j/(tam/3)];
		}
	}
	/***********************
	*         FILTRO       *
	************************/
	TG = new cpx[tam*tam];
	fft2d(G,TG,tam,1);
	cpx *W = new cpx[tam*tam];
	WnrDeconvolutionFilter(TG, W, K);
	for (int i=0;i<tam;i++){
		for (int j=0;j<tam;j++){
			tfourier[i*tam+j]=tfourier[i*tam+j]*W[i*tam+j];
		}
	}
}
