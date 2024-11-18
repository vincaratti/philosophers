/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcaratti <vcaratti@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:28:05 by vcaratti          #+#    #+#             */
/*   Updated: 2024/11/16 17:09:21 by vcaratti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	launcher_exit(t_params *params)
{
	pthread_mutex_lock(&(params->death_status_mutex));
	params->died = 1;
	pthread_mutex_unlock(&(params->death_status_mutex));
}

int	treat_params(int argc, char **argv, t_params *params)
{
	if (argc < 5 || argc > 6)
		return (1);
	params->nb_philos = ft_atoi(argv[1]);
	params->ttd = ft_atoi(argv[2]);
	params->tte = ft_atoi(argv[3]);
	params->tts = ft_atoi(argv[4]);
	params->died = 0;
	params->all_ate = 0;
	params->nb_tte = -1;
	if (argc == 6)
		params->nb_tte = ft_atoi(argv[5]);
	if (params->nb_philos < 1 || params->nb_philos > 250
		|| (params->nb_tte <= 0 && argc == 6)
		|| params->ttd < 0 || params->tte < 0 || params->tts < 0)
		return (1);
	if (pthread_mutex_init(&(params->death_mutex), NULL)
		|| pthread_mutex_init(&(params->write_mutex), NULL)
		|| pthread_mutex_init(&(params->death_status_mutex), NULL)
		|| pthread_mutex_init(&(params->all_ate_mutex), NULL))
		return (1);
	return (0);
}

void	init_philo_data(t_params *params, t_data *data)
{
	int	i;

	i = 0;
	while (i < params->nb_philos)
	{
		data->pdata[i].pid = i;
		data->pdata[i].nb_ate = 0;
		data->pdata[i].me_dead = 0;
		data->pdata[i].params = params;
		data->pdata[i].myforks[0] = &(data->forks[i]);
		if (i == params->nb_philos - 1)
			data->pdata[i].myforks[1] = &(data->forks[0]);
		else
			data->pdata[i].myforks[1] = &(data->forks[i + 1]);
		i++;
	}
}

int	allocate_mem(t_params *params, t_data *data)
{
	int	i;

	data->tid = malloc(sizeof(pthread_t) * (params->nb_philos));
	data->forks = malloc(sizeof(pthread_mutex_t) * (params->nb_philos));
	data->pdata = malloc(sizeof(t_philo_d) * (params->nb_philos));
	if (!data->tid || !data->forks || !data->tid)
		return (free_all(data->tid, data->forks, data->pdata), 1);
	i = -1;
	while (++i < params->nb_philos)
	{
		if (pthread_mutex_init(data->forks + i, NULL) != 0)
			return (free_all(data->tid, data->forks, data->pdata), 1);
	}
	init_philo_data(params, data);
	return (0);
}
