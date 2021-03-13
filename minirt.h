#ifndef ARTMINIRT_H
# define ARTMINIRT_H
# include "mlx.h"
# include <math.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include "mlx.h"

# include <math.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>

# define MY_DESTROYNOTIFY 17
# define MY_EXPOSE 12

# define FALSE 0

# define ADJUSTMENT_SCREEN 100
# define HEADER_SIZE 54
# define INFO_HEADER_SIZE 40
# define FILE_PERMISSION 0644
# define SPECULAR_GLOSS 15
# define MAX_OB 100

# define BUFFER_SIZE 1000
# define SUCCESS 0
# define ERROR 1
# define ERROR_GET_LINE -1
# define ERROR_OPEN -2
# define ERROR_MALLOC -3
# define ERROR_TYPE -4
# define ERROR_DUPLICATE -5
# define ERROR_MISSING -6
# define ERROR_ARGC -7
# define ERROR_SAVEFLAG -8
# define ERROR_RT_FILE -9
# define ERROR_RESOLUTION -10
# define ERROR_AMBIENT_LIGHT -11
# define ERROR_CAMERA -12
# define ERROR_LIGHT -13
# define ERROR_SPHERE -14
# define ERROR_TRIANGLE -15
# define ERROR_PLANE -16
# define ERROR_SQUARE -17
# define ERROR_CYLINDER -18
# define ERROR_SCREEN_SIZE -20
# define ERROR_CREATE_FILE -21
# define ERROR_WRITE_FILE -22
# define ERROR_OVER -23
# define ERROR_OVER_OB -24
# define ERROR_MLX -25

# define POINT_NOT_FREE 0
# define POINT_DATA_FREE 1
# define POINT_MLX_FREE 2
# define POINT_WIN_FREE 3
# define POINT_DATAS_FREE 4
# define POINT_IMAGE_FREE 5

# define TYPE_ERROR 'e'
# define TYPE_LIGHT 'l'
# define TYPE_SPHERE 's'
# define TYPE_TRIANGLE 't'
# define TYPE_PLANE 'p'
# define TYPE_SQUARE 'q'
# define TYPE_CYLINDER 'c'
# define TYPE_TUBE 'u'
# define TYPE_CIRCLE 'i'

# define KEY_ESC 65307
# define KEY_ZERO 48
# define KEY_LEFT 65361
# define KEY_UP 65362
# define KEY_RIGHT 65363
# define KEY_DOWN 65364



typedef struct	s_vec {
	double	x;
	double	y;
	double	z;
}				t_vec;

typedef struct	s_col {
	double	r;
	double	g;
	double	b;
}				t_col;

typedef struct	s_type {
	int		first;
	int		second;
	int		resolution;
	int		ambient;
}				t_type;

typedef struct	s_object {
	t_vec	vec;
	t_vec	no_vec;
	t_vec	*points;
	t_col rgb;
	double	height;
	double	r;
	double	size;
	int		h_fov;
	double	ratio;
	char	type;
}				t_object;

typedef struct s_data
{
    int width; //ウィンドウのサイズ
    int height;
    double Ie;
	double Ks;
    t_col rgba;
    t_object	*camera_arr;
	int			n_camera;
	t_object	*object_arr;
	int         n_obj;
    t_object    *light_arr;
    int         n_light;
    t_vec		screen;
	int			front_ob_i;
	t_vec		front_ob_point;
	int			save_flag;
}           t_data;

typedef struct	s_vars {
	void	*mlx;
	void	*win;
	t_data	img;
	t_data	*img_array;
}				t_vars;
double InnerProduct( double a[3], double b[3]);
int		create_trgb(int t, int r, int g, int b);
void	check_line_info(t_data *img, char *s, int *flag, t_type *type_flag);
void	get_line_info(t_data *data, int fd);
void	ft_free(char *memo, char **line);
int		ft_put_line(char **line, char **memo, char *buf);
int		get_next_line(int fd, char **line);
size_t	ft_strchr(char *s, char c);
ssize_t	ft_strlen(const char *s);
char	*ft_strchr_p(const char *s, int c);
void	ft_strcpy(char *s1, char *s2, size_t n);
char	*ft_strdup(char *s1);
char	*ft_strjoin(char *s1, char *s2, ssize_t n1, ssize_t n2);
#endif