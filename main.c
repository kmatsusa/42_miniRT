#include "minirt.h"

double InnerProduct( double a[3], double b[3])
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]; 
}
int		create_trgb(int t, int r, int g, int b)
{
	return(t << 24 | r << 16 | g << 8 | b);
}

// typedef struct s_data
// {
//     int width; //ウィンドウのサイズ
//     int height;
//     double Ie;
//     double rgba[3];
//     double rgbl[3];
//     double rgbsp[3];
//     double Pv[3];
//     double Pc[3];
//     double Pl[3];
// };


int main()
{
    void *mlx;//スクリーン接続識別子
    void *mlx_win;//ウィンドウ識別子
    int fd;
    int			flag;
	t_type		type_flag;
    t_data      data;

    fd = open("./minirt.rt", 0);
    get_line_info(&data, &flag, fd, &type_flag);
    // int i;
    // int j;
    int width = data.width; //ウィンドウのサイズ
    int height = data.height;

    mlx = mlx_init();//fileopenの時のfd的な？
    mlx_win = mlx_new_window(mlx, width, height,"test");
	// mlx_win = mlx_new_window(スクリーン識別子, width, height,"test");
	//座標は左上が原点、矢印はx軸が右、y軸が下向きになる
	//でも与えられる図形の座標は一般的な向きの座標のためあとで座標変換が必要となる
    // i= 0;
    double Pv[3] = {data.Pv.x, data.Pv.y, data.Pv.z}; //カメラ
    // double Pv[3] = {0.0, 0.0, 2000.0}; //カメラ
    double Pc[3] = {data.Pc.x, data.Pc.y, data.Pc.z};//球
    double R = data.R / 2; //大きさ
    
    double v[3];
    int i, j;

    for(i = 0; i <= width; i++){
        for(j = 0;j <= height; j++){
            v[0] = i + 0.5 - Pv[0];
            v[1] = j + 0.5 - Pv[1];
            v[2] = 0 - Pv[2];

            double A, B2, C, D;
            double Pvc[3];
            int k;
            for( k=0 ; k<3 ; k++ ) Pvc[k] = Pv[k]-Pc[k];
            A = InnerProduct( v, v );
            B2 = InnerProduct( Pvc, v );
            C = InnerProduct( Pvc, Pvc )-R*R;
            D = B2*B2-A*C;

            double P[3];
            double t0;
            t0 = (-B2-sqrt(D))/A;
            for( k=0 ; k<3 ; k++ ) P[k] = Pv[k]+v[k]*t0;

            double PL[3] = {data.Pl.x, data.Pl.y, data.Pl.z};//light
            double N[3], vL[3];
            double cosA;
            /* P は視線と球の交点 */
            for( k=0 ; k<3 ; k++ ) N[k] = P[k]-Pc[k];
            for( k=0 ; k<3 ; k++ ) vL[k] = P[k]-PL[k];
            cosA = -InnerProduct( vL, N );
            cosA /= sqrt( InnerProduct( vL, vL )*InnerProduct( N, N ) );
            double vR[3];
            int col;
            double w, cosG;
            /* P は視線と球の交点 */
            w = -InnerProduct( vL, N )/InnerProduct( N, N );
            for( k=0 ; k<3 ; k++ ) vR[k] = 2*w*N[k]+vL[k];
            cosG = -InnerProduct(vR,v)/
            sqrt(InnerProduct(vR, vR)*InnerProduct(v,v));
            if( cosG < 0 ) cosG = 0;
            cosG = pow(cosG,10);
            double Kd = 1.0, Ks = data.Ks, Ie = data.Ie, I = 255.0;
            double rgbsp[3] = {data.rgbsp.r, data.rgbsp.g, data.rgbsp.b}, rgbl[3] = {data.rgbl.r, data.rgbl.g, data.rgbl.b}, color[3];
            if( D >= 0 ){
            for( k=0 ; k<3 ; k++ ){
            color[k] = rgbsp[k]*Kd*cosA;
            if( color[k] < I*Ks*cosG ) color[k] = rgbl[k]*Ks*cosG;
            if( color[k] < rgbsp[k]*Ie ) color[k] = rgbsp[k]*Ie;
            }
            col = create_trgb(0, color[0], color[1], color[2]);
            mlx_pixel_put(mlx, mlx_win, i, j, col);
            }else{
            mlx_pixel_put(mlx, mlx_win, i, j, 127);
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
    close(fd);
    mlx_loop(mlx);//無限ループを作り、その間ウィンドウを表示させ続ける
    //最初のうちはcntl+Cで強制終了すればおk
    //クリックした時に終了させるなどのプログラムはゆくゆく書く必要がある
    return (1);
}