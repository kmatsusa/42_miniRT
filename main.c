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

    data.n_obj = 0;
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


    double Pc[10][3];
    double R[10];
    
    // Pc[i][3]= {data.object_arr[i].Pc.x, data.object_arr[i].Pc.y, data.object_arr[i].Pc.z};//球
    // R[i]= set_R(data.object_arr[i]) //大きさ

    // printf("%f,%f,%f\n",data.object_arr[2].Pc.x, data.object_arr[2].Pc.y, data.object_arr[2].Pc.z);
    for (int i = 0; i < data.n_obj ; i++)
    {
        Pc[i][0] = data.object_arr[i].Pc.x;
        Pc[i][1] = data.object_arr[i].Pc.y;
        Pc[i][2] = data.object_arr[i].Pc.z;
        R[i] = data.object_arr[i].r/2;
    }
    
    // printf("%f,%f,%f\n",data.object_arr[0].Pc.x, data.object_arr[0].Pc.y, data.object_arr[0].Pc.z);
    // printf("%f,%f,%f\n",data.object_arr[1].Pc.x, data.object_arr[1].Pc.y, data.object_arr[1].Pc.z);
//*    
    double v[3];
    int i, j;

    for(i = 0; i <= 1000; i++){
        for(j = 0;j <= 1000; j++){
            v[0] = i + 0.5 - Pv[0];
            v[1] = j + 0.5 - Pv[1];
            v[2] = 0 - Pv[2];

            double A, B2, C, D0, D;
            double Pvc[3];
            int k;
            int s0 = -1,s;
            double t0,t;

            for (s=0;s<2;s++)
            {
                for( k=0 ; k<3 ; k++ ) Pvc[k] = Pv[k]-Pc[s][k];
                A = InnerProduct( v, v );
                B2 = InnerProduct( Pvc, v );
                C = InnerProduct( Pvc, Pvc )-R[s]*R[s];
                D = B2*B2-A*C;
                if( D >= 0 ){
                    t = (-B2-sqrt(D))/A;
                    if( s0 < 0 || t < t0 ){
                        s0 = s;
                        t0 = t;
                        D0 = D;
                    }
                }
            }

            double P[3];
            double PL[3] = {-20000, -20000, 20000};
            double N[3], vL[3];
            double cosA;
            double vR[3];
            int col;
            double w, cosG;
            double v1[3], Pvc1[3];

            if (s0 >= 0){
                for( k=0 ; k<3 ; k++ ) P[k] = Pv[k]+v[k]*t0;
                /* P は視線と球の交点 */
                for( k=0 ; k<3 ; k++ ) N[k] = P[k]-Pc[s0][k];
                for( k=0 ; k<3 ; k++ ) vL[k] = P[k]-PL[k];
                
                /* P は視線と球の交点 */
                for ( s = 0; s < 2; s++)
                {
                    if( s == s0 ) continue;
                    for( k=0 ; k<3 ; k++ ) v1[k] = PL[k]-P[k];
                    for( k=0 ; k<3 ; k++ ) Pvc1[k] = P[k]-Pc[s][k];
                    A = InnerProduct( v1, v1 );
                    B2 = InnerProduct( Pvc1, v1 );
                    C = InnerProduct( Pvc1, Pvc1 )-R[s]*R[s];
                    D = B2*B2-A*C ;
                    if( D >= 0 && (-B2-sqrt(D))/A > 0 ) break;
                }
                if(s<2)
                {
                    cosA = cosG = 0.0;
                }else {
                    cosA = -InnerProduct( vL, N );
                    cosA /= sqrt( InnerProduct( vL, vL )*InnerProduct( N, N ) );
                    w = -InnerProduct( vL, N )/InnerProduct( N, N );
                    for( k=0 ; k<3 ; k++ ) vR[k] = 2*w*N[k]+vL[k];
                    cosG = -InnerProduct(vR,v)/
                    sqrt(InnerProduct(vR, vR)*InnerProduct(v,v));
                    if( cosG < 0 ) cosG = 0;
                    cosG = pow(cosG,10);
                }
                double Kd = 1.0, Ks = 0.7, Ie = 0.1, I = 255.0;
                double RGB[2][3] = {{255, 0, 0},{0,255,0}}, color[3];
                if( D0 >= 0 ){
                    for( k=0 ; k<3 ; k++ ){
                        color[k] = RGB[s0][k]*Kd*cosA;
                        if( color[k] < I*Ks*cosG ) color[k] = I*Ks*cosG;
                        if( color[k] < RGB[s0][k]*Ie ) color[k] = RGB[s0][k]*Ie;
                    }
                }
                col = create_trgb(0, color[0], color[1], color[2]);
                mlx_pixel_put(mlx, mlx_win, i, j, col);
            }else{
                col = create_trgb(0, 55, 55, 55);
                mlx_pixel_put(mlx, mlx_win, i, j, col);
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