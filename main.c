#include "altminirt.h"

double InnerProduct( double a[3], double b[3])
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]; 
}
int		create_trgb(int t, int r, int g, int b)
{
	return(t << 24 | r << 16 | g << 8 | b);
}

void initialize_img(t_data *img)
{
	img->object_arr = NULL;
	img->n_camera = 0;
	img->light_arr = NULL;
	img->n_light = 0;
	img->object_arr = NULL;
	img->n_obj = 0;
    img->front_ob_i = 0;
}

int main()
{
    void *mlx;//スクリーン接続識別子
    void *mlx_win;//ウィンドウ識別子
    int fd;
    int			flag;
	t_type		type_flag;
    t_vars      var;

    initialize_img(&(var.img));
    fd = open("./minirt.rt", 0);
    get_line_info(&(var.img),fd);
    mlx = mlx_init();//fileopenの時のfd的な？
    mlx_win = mlx_new_window(mlx, var.img.width, var.img.height,"test");
    double Pv[3] = {var.img.camera_arr->vec.x, var.img.camera_arr->vec.y, var.img.camera_arr->vec.z}; //カメラ
    double Pc[50][3], RGB[50][3];
    double R[50];
    double v[3];
    int i, j;
    
    for (int i = 0; i < var.img.n_obj ; i++)
    {
        Pc[i][0] = var.img.object_arr[i].vec.x;
        Pc[i][1] = var.img.object_arr[i].vec.y;
        Pc[i][2] = var.img.object_arr[i].vec.z;
        R[i] = var.img.object_arr[i].r / 2;
        RGB[i][0] = var.img.object_arr[i].rgb.r;
        RGB[i][1] = var.img.object_arr[i].rgb.g; 
        RGB[i][2] = var.img.object_arr[i].rgb.b; 
    }

    for(i = 0; i <= var.img.width; i++){
        for(j = 0;j <= var.img.height; j++){
            v[0] = i + 0.5 - Pv[0];
            v[1] = j + 0.5 - Pv[1];
            v[2] = 0 - Pv[2];

            double A, B2, C, D0, D;
            double Pvc[3];
            int k;
            int s0 = -1,s;
            double t0,t;

            for (s=0;s<var.img.n_obj;s++)
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
            double PL[10][3];
            double N[3], vL[3];
            double cosA;
            double vR[3];
            int col;
            double w, cosG;
            double v1[3], Pvc1[3];
            double Kd = 1.0, Ks = 0.7, Ie = 0.1, I = 255.0;
            double color[3],color_alt[3]= {0,0,0};
            int li;

            for (int i = 0; i < var.img.n_light ; i++)
            {
                PL[i][0] = var.img.light_arr[i].vec.x;
                PL[i][1] = var.img.light_arr[i].vec.y;
                PL[i][2] = var.img.light_arr[i].vec.z;
            }

            if (s0 >= 0){
                for ( li = 0; li < var.img.n_light; li++)
                {
                    for( k=0 ; k<3 ; k++ ) P[k] = Pv[k]+v[k]*t0;
                    /* P は視線と球の交点 */
                    for( k=0 ; k<3 ; k++ ) N[k] = P[k]-Pc[s0][k];
                    for( k=0 ; k<3 ; k++ ) vL[k] = P[k]-PL[li][k];

                    /* P は視線と球の交点 */
                    for ( s = 0; s < var.img.n_obj; s++)
                    {
                        if( s == s0 ) continue;
                        for( k=0 ; k<3 ; k++ ) v1[k] = PL[li][k]-P[k];
                        for( k=0 ; k<3 ; k++ ) Pvc1[k] = P[k]-Pc[s][k];
                        A = InnerProduct( v1, v1 );
                        B2 = InnerProduct( Pvc1, v1 );
                        C = InnerProduct( Pvc1, Pvc1 )-R[s]*R[s];
                        D = B2*B2-A*C ;
                        if( D >= 0 && (-B2-sqrt(D))/A > 0 ) break;
                    }
                    if(s<var.img.n_obj)
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
                    if( D0 >= 0 ){
                        for( k=0 ; k<3 ; k++ ){
                            color[k] = RGB[s0][k]*Kd*cosA;
                            if( color[k] < I*Ks*cosG ) color[k] = I*Ks*cosG;
                            if( color[k] < RGB[s0][k]*Ie ) color[k] = RGB[s0][k]*Ie;
                            color_alt[k] += color[k];
                            if (color_alt[k] > 255)
                                color_alt[k] = 255;
                        }
                    }
                col = create_trgb(0, color_alt[0], color_alt[1], color_alt[2]);
                mlx_pixel_put(mlx, mlx_win, i, j, col);
                }
            }
            else{
                col = create_trgb(0, 0, 0, 0);
                mlx_pixel_put(mlx, mlx_win, i, j, col);
            }
        }
    }
    
    
    close(fd);
    mlx_loop(mlx);//無限ループを作り、その間ウィンドウを表示させ続ける
    //最初のうちはcntl+Cで強制終了すればおk
    //クリックした時に終了させるなどのプログラムはゆくゆく書く必要がある
    return (1);
}






