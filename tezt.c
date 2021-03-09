#include <stdio.h>
#include "mlx.h"
#include <math.h>

int		create_trgb(int t, int r, int g, int b)
{
	return(t << 24 | r << 16 | g << 8 | b);
}

double InnerProduct( double a[3], double b[3])
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]; 
}

int main()
{
    void *mlx;//スクリーン接続識別子
    void *mlx_win;//ウィンドウ識別子
    // int i;
    // int j;
    int width = 1920; //ウィンドウのサイズ
    int height = 1080;
    // int width = 500; //ウィンドウのサイズ
    // int height = 500;

    mlx = mlx_init();//fileopenの時のfd的な？
    mlx_win = mlx_new_window(mlx, width, height,"test");
	// mlx_win = mlx_new_window(スクリーン識別子, width, height,"test");
	//座標は左上が原点、矢印はx軸が右、y軸が下向きになる
	//でも与えられる図形の座標は一般的な向きの座標のためあとで座標変換が必要となる
    // i= 0;
    double Pv[3] = {0.0, 0.0, 2000.0}; 
    double Pc[3] = {250.0, 250.0, -1000.0};//球
    double R = 250.0; //大きさ
    
    double v[3];
    int i, j;


    for(i = 0; i <= 400; i++){
        for(j = 0;j <= 400; j++){
            v[0] = i + 0.5 - Pv[0];
            v[1] = j + 0.5 - Pv[1];
            v[2] = 0 - Pv[2];

            double A, B2, C, D;
            double P[3];
            int k;
            double t0;
            t0 = (-B2-sqrt(D))/A;
            for( k=0 ; k<3 ; k++ ) P[k] = Pv[k]+v[k]*t0;
            double PL[3] = {-20000, 20000, 20000};
            double N[3], vL[3];

            double cosA;
            /* P は視線と球の交点 */
            for( k=0 ; k<3 ; k++ ) N[k] = P[k]-Pc[k];
            for( k=0 ; k<3 ; k++ ) vL[k] = P[k]-PL[k];
            cosA = -InnerProduct( vL, N );
            cosA /= sqrt( InnerProduct( vL, vL )*InnerProduct( N, N ) );
            
            
            double Pvc[3];
            int col;
            for( k=0 ; k<3 ; k++ ) Pvc[k] = Pv[k]-Pc[k];
            A = InnerProduct( v, v );
            B2 = InnerProduct( Pvc, v );
            C = InnerProduct( Pvc, Pvc )-R*R;
            D = B2*B2-A*C ;
            
            if( D >= 0 ){
/* 座標(i,j)に明るさcosAで点を描く */
            if( cosA >= 0 ){
                col = create_trgb(0, 0, 0, 255*cosA);
            mlx_pixel_put( mlx, mlx_win, i, j, col);
            }else{
            mlx_pixel_put( mlx, mlx_win, i, j, 0);
            }
            }else{
            mlx_pixel_put( mlx, mlx_win, i, j, 127);
            }
        }
    }
	// while (i < width)
	// {
	// 		j = 0;
	// 		while (j < height)
	// 		{
	// 				mlx_pixel_put(mlx, mlx_win, i, j, 127);
	// 				j++;
	// 		}
	// 		i++;
	// }
    mlx_loop(mlx);//無限ループを作り、その間ウィンドウを表示させ続ける
    //最初のうちはcntl+Cで強制終了すればおk
    //クリックした時に終了させるなどのプログラムはゆくゆく書く必要がある
    return (1);
}