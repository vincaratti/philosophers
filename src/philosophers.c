/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcaratti <vcaratti@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:36:52 by vcaratti          #+#    #+#             */
/*   Updated: 2024/11/16 17:16:01 by vcaratti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*live(void *arg)
{
	t_philo_d	*pdata;
	t_params	*params;

	pdata = (t_philo_d *)arg;
	params = pdata->params;
	while (!d_status(params) && !pdata->me_dead)
	{
		philo_eat(pdata);
		if (d_status(params) || pdata->me_dead)
			break ;
		philo_sleep(pdata);
		if (d_status(params) || pdata->me_dead)
			break ;
		philo_think(pdata);
		if (d_status(params) || pdata->me_dead)
			break ;
	}
	return (NULL);
}

int	start_philos(t_params *params, t_data *data)
{
	int	i;

	i = 0;
	while (i < params->nb_philos)
	{
		data->pdata[i].params->t0 = current_time_ll();
		if (pthread_create(data->tid + i, 0, &live,
				(void *)(&(data->pdata[i]))))
			return (1);
		i += 2;
	}
	usleep(1000);
	i = 1;
	data->pdata->params->t0 = current_time_ll();
	while (i < params->nb_philos)
	{
		if (pthread_create(data->tid + i, 0, &live,
				(void *)(&(data->pdata[i]))))
			return (1);
		i += 2;
	}
	return (0);
}

int	grim_reaper(t_params *params, t_data *data)
{
	int	i;

	i = 0;
	while (i < params->nb_philos && !d_status(params))
	{
		pthread_mutex_lock(&(params->death_mutex));
		if (timestamp(params->t0) - data->pdata->last_ate >= params->ttd)
		{
			philo_print("died", data->pdata + i);
			pthread_mutex_lock(&(params->death_status_mutex));
			params->died = 1;
			pthread_mutex_unlock(&(params->death_status_mutex));
		}
		pthread_mutex_unlock(&(params->death_mutex));
		if (d_status(params))
			return (1);
		usleep(100);
		i++;
	}
	return (0);
}

void	supervisor(t_params *params, t_data *data)
{
	int	i;

	while (params->all_ate == 0)
	{
		if (grim_reaper(params, data))
			return ;
		if (params->nb_tte > 0)
		{
			i = 0;
			while (i < params->nb_philos
				&& data->pdata[i].nb_ate >= params->nb_tte)
				i++;
			if (i == params->nb_philos)
			{
				pthread_mutex_lock(&(params->all_ate_mutex));
				params->all_ate = 1;
				pthread_mutex_unlock(&(params->all_ate_mutex));
			}
		}
	}
}

int	main(int argc, char **argv)
{
	t_params	params;
	t_data		data;

	if (treat_params(argc, argv, &params))
		return (1);
	if (allocate_mem(&params, &data))
		return (1);
	if (start_philos(&params, &data))
		return (free_all(data.tid, data.forks, data.pdata),
			close_threads_mutex(&params, &data), 1);
	supervisor(&params, &data);
	usleep(10000);
	close_threads_mutex(&params, &data);
	return (free_all(data.tid, data.forks, data.pdata), 0);
}
