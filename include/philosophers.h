/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcaratti <vcaratti@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:15:00 by vcaratti          #+#    #+#             */
/*   Updated: 2024/11/16 16:51:15 by vcaratti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include "../libft/libft.h"
# include <sys/time.h>

typedef struct s_params
{
	int					nb_philos;
	int					ttd;
	int					tte;
	int					tts;
	int					nb_tte;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		write_mutex;
	pthread_mutex_t		death_status_mutex;
	pthread_mutex_t		all_ate_mutex;
	long long int		t0;
	int					died;
	int					all_ate;
}	t_params;

typedef struct s_philo_d
{
	pthread_mutex_t		*myforks[2];
	int					pid;
	long long int		last_ate;
	int					nb_ate;
	int					me_dead;
	t_params			*params;
}	t_philo_d;

typedef struct s_data
{
	pthread_t			*tid;
	pthread_mutex_t		*forks;
	t_philo_d			*pdata;
}	t_data;

//====	philosophers.c	====//

void			*live(void *arg);
int				start_philos(t_params *params, t_data *data);
int				grim_reaper(t_params *params, t_data *data);
void			supervisor(t_params *params, t_data *data);

//====		tools.c		====//

int				d_status(t_params *params);
void			free_all(pthread_t *tid, pthread_mutex_t *forks,
					t_philo_d *pdata);
long long int	current_time_ll(void);
long long int	timestamp(long long int t0);
void			close_threads_mutex(t_params *params, t_data *data);

//====	  p_actions.c	====//

void			philo_print(char *str, t_philo_d *pdata);
void			philo_wait(t_philo_d *pdata, int ttw);
void			philo_eat(t_philo_d *pdata);
void			philo_think(t_philo_d *pdata);
void			philo_sleep(t_philo_d *pdata);

//====		init.c		====//

int				treat_params(int argc, char **argv, t_params *params);
void			init_philo_data(t_params *params, t_data *data);
int				allocate_mem(t_params *params, t_data *data);

#endif
