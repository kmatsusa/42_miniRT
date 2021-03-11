#include "minirt.h"


int		ft_isdigit(int c)
{
	return ('0' <= c && c <= '9');
}

int		ft_isspace(int c)
{
	return (c == ' ' || c == '\f' || c == '\n' ||\
			c == '\r' || c == '\t' || c == '\v');
}

void	ft_advance_isspace(char **s)
{
	while (ft_isspace(**s))
		(*s)++;
}

int		ft_strcmp(const char *s1, const char *s2)
{
	size_t i;

	i = 0;
	if (s1 == NULL && s2 == NULL)
		return (0);
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

ssize_t	ft_strlen(const char *s)
{
	ssize_t i;

	i = 0;
	if (s == NULL)
		return (0);
	while (*(s + i))
		i++;
	return (i);
}

int		ft_get_ndigits(int n)
{
	int i;

	i = 0;
	while (n != 0)
	{
		i++;
		n /= 10;
	}
	return (i);
}

char	*ft_strrchr(const char *s, int c)
{
	size_t n;
	size_t i;

	i = 0;
	n = 0;
	while (*(s + i))
	{
		if (*(s + i) == c)
			n = i;
		i++;
	}
	if (c == 0)
		return ((char *)(s + i));
	if (*s != c && n == 0)
		return (NULL);
	return ((char *)(s + n));
}

void	ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;

	d = (char *)dest;
	s = (const char *)src;
	while (n--)
		*d++ = *s++;
}

char	*ft_strchr_p(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s && *s != c)
		s++;
	if (*s == c)
		return ((char *)s);
	return (NULL);
}

int		ft_strchr_n(const char *s, int c)
{
	int	n;

	n = 0;
	if (c == 0)
		return (-1);
	while (*(s + n) && *(s + n) != c)
		n++;
	if (*(s + n) == c)
		return (n);
	return (-1);
}

char	*ft_strchr_ex(const char *s, int c, ssize_t *len, int *flag)
{
	const char *start;

	*len = 0;
	if (s == NULL)
		return (NULL);
	start = s;
	while (*s && *s != c)
		s++;
	*len = (ssize_t)(s - start);
	if (*s == c)
		*flag = 1;
	if (*s == c && *(s + 1))
		return ((char *)(s + 1));
	return (NULL);
}

int		set_int(char **s, int *flag)
{
	int num;
	int i;

	num = 0;
	i = 0;
	while (ft_isdigit(**s))
	{
		if (++i <= 8)
			num = num * 10 + (**s - '0');
		else
			*flag = ERROR_OVER;
		++(*s);
	}
	return (num);
}

double	set_double(char **s, int *flag)
{
	int		d;
	double	num;
	int		sign;
	char	*p;

	sign = 1;
	if (**s == '-')
	{
		sign = -1;
		(*s)++;
	}
	if (!ft_isdigit(**s))
		return (INFINITY);
	num = set_int(s, flag);
	if (**s == '.')
	{
		(*s)++;
		p = *s;
		while (**s == '0')
			(*s)++;
		d = set_int(s, flag);
		num += pow(10, -1 * (int)(*s - p)) * d;
	}
	return (num * sign);
}

void set_color(t_col *rgba, char **s,int *flag)
{
	rgba->r = set_int(s, flag);
	if (**s == ',')
		(*s)++;
	// else
	// 	*flag = error_num;
	rgba->g = set_int(s, flag);
	if (**s == ',')
		(*s)++;
	// else
	// 	*flag = error_num;
	rgba->b = set_int(s, flag);
	ft_advance_isspace(s);
}
void set_vec(t_vec *P, char **s,int *flag)
{
	P->x = set_double(s, flag);
	if (**s == ',')
		(*s)++;
	// ft_advance_isspace(s);
	P->y = set_double(s, flag);
	if (**s == ',')
		(*s)++;
	// ft_advance_isspace(s);
	P->z = set_double(s, flag);
	ft_advance_isspace(s);
}


void	set_resolution(t_data *data, char *s, int *flag, t_type *type_flag)
{
	// if (type_flag->resolution)
	// {
	// 	*flag = ERROR_DUPLICATE;
	// 	return ;
	// }
	type_flag->resolution = 1;
	ft_advance_isspace(&s);
	data->width = set_int(&s, flag);
	ft_advance_isspace(&s);
	data->height = set_int(&s, flag);
	ft_advance_isspace(&s);
	// if (*flag >= 0 && (img->x == 0 || img->y == 0 || *s != 0 || \
	// 							img->x > 10000 || img->y > 10000))
	// 	*flag = ERROR_RESOLUTION;
}

void	set_ambient_light(t_data *data, char *s, int *flag, t_type *type_flag)
{
	// if (type_flag->ambient)
	// {
	// 	*flag = ERROR_DUPLICATE;
	// 	return ;
	// }
	type_flag->ambient = !FALSE;
	ft_advance_isspace(&s);
	data->Ie = set_double(&s, flag);
	ft_advance_isspace(&s);
	set_color(&data->rgba, &s, flag);
	ft_advance_isspace(&s);
	// if (*flag >= 0 && (img->ambient_light == -1 || \
	// 	!ft_isin_double(0, img->ambient_light_ratio, 1) || \
	// 	*s != 0))
	// 	*flag = ERROR_AMBIENT_LIGHT;
}

void	set_camera(t_data *data, char *s, int *flag)
{
	set_vec(&data->Pv, &s, flag);
	ft_advance_isspace(&s);



	// t_object *ob;

	// set_object(&(img->camera), &(img->n_camera), flag);
	// if (*flag < 0)
	// 	return ;
	// ob = &(img->camera[img->n_camera - 1]);
	// ft_advance_isspace(&s);
	// set_vec(&(ob->vec), &s, flag, ERROR_CAMERA);
	// ft_advance_isspace(&s);
	// set_vec(&(ob->no_vec), &s, flag, ERROR_CAMERA);
	// ft_advance_isspace(&s);
	// if (!ft_isdigit(*s))
	// 	*flag = ERROR_CAMERA;
	// ob->h_fov = set_int(&s, flag);
	// ft_advance_isspace(&s);
// 	if (*flag >= 0 && (!ft_isin_double_vec(-1, &(ob->no_vec), 1) || \
// 		!ft_isin_int_nequal(0, ob->h_fov, 180) || *s != 0))
// 		*flag = ERROR_CAMERA;
// 	if (*flag >= 0)
// 		vec_product_scaler_inplace(&(ob->no_vec), M_PI);
}

void	set_light(t_data *data, char *s, int *flag)
{
	set_vec(&data->Pl, &s, flag);
	ft_advance_isspace(&s);
	data->Ks = set_double(&s, flag);
	ft_advance_isspace(&s);
	set_color(&data->rgbl, &s, flag);
	ft_advance_isspace(&s);
}

void set_object(t_object **object, int *n_obj)
{
	int i;
	t_object *alt;

	i = 0;
	if ((alt = (t_object *)malloc((1 + 1)*sizeof(t_object)))==NULL)
		return ;
	while (i < *n_obj)
	{
		alt[i] = (*object)[i];
		i++;
	}
	// free(*object);
	*object = alt;
	++(*n_obj);
}

void	set_sphere(t_data *data, char *s, int *flag)
{
	t_object *alt;

	set_object(&(data->object_arr), &(data->n_obj));
	alt = &(data->object_arr[data->n_obj - 1]);
	set_vec(&(alt->Pc), &s, flag);
	ft_advance_isspace(&s);
	alt->r = set_double(&s, flag);
	ft_advance_isspace(&s);
	set_color(&alt->rgbsp, &s, flag);
}

void	check_type(t_data *img, char *s, int *flag, t_type *type_flag)
{
	if (type_flag->first == 0 && type_flag->second == 0)
		set_resolution(img, ++s, flag, type_flag);
	else if (type_flag->first == 1 && type_flag->second == 0)
		set_ambient_light(img, ++s, flag, type_flag);
	else if (type_flag->first == 2 && type_flag->second == 0)
		set_camera(img, ++s, flag);
	else if (type_flag->first == 3 && type_flag->second == 0)
		set_light(img, ++s, flag);
	else if (type_flag->first == 4 && type_flag->second == 1)
		set_sphere(img, ++s, flag);

	// else if (type_flag->first == 5 && type_flag->second == 2)
	// 	set_plane(img, ++s, flag);
	// else if (type_flag->first == 4 && type_flag->second == 3)
	// 	set_square(img, ++s, flag);
	// else if (type_flag->first == 2 && type_flag->second == 4)
	// 	set_cylinder(img, ++s, flag);
	// else if (type_flag->first == 6 && type_flag->second == 5)
	// 	set_triangle(img, ++s, flag);
	// else
	// 	*flag = ERROR_TYPE;
}

void	check_line_info(t_data *data, char *s, int *flag, t_type *type_flag)
{
	if (*s == 0)
		return ;
	ft_advance_isspace(&s);
	if ((type_flag->first = ft_strchr_n("RAclspt", *s)) >= 0)
		if ((type_flag->second = \
			(ft_isspace(*(++s)) ? 0 : ft_strchr_n(" plqyr", *s))) >= 1)
			if (!ft_isspace(*(++s)))
				type_flag->first = -1;
	check_type(data, s, flag, type_flag);
}

void	get_line_info(t_data *data, int *flag, int fd, t_type *type_flag)
{
	char	*buff;

	while ((*flag = get_next_line(fd, &buff)) >= 0)
	{
		check_line_info(data, buff, flag, type_flag);
		free(buff);
		buff = NULL;
		// if (vars->img.n_object > MAX_OB || \
		// 		vars->img.n_light > MAX_OB || \
		// 		vars->img.n_camera > MAX_OB)
		// 	*flag = ERROR_OVER_OB;
		if (*flag <= 0)
		{
			get_next_line(-1, &buff);
			break ;
		}
	}
}