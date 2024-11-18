/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcaratti <vcaratti@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:30:46 by vcaratti          #+#    #+#             */
/*   Updated: 2024/11/16 17:15:43 by vcaratti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	d_status(t_params *params)
{
	int	ret;

	pthread_mutex_lock(&(params->death_status_mutex));
	ret = params->died;
	pthread_mutex_unlock(&(params->death_status_mutex));
	if (ret)
		return (ret);
	pthread_mutex_lock(&(params->all_ate_mutex));
	ret = params->all_ate;
	pthread_mutex_unlock(&(params->all_ate_mutex));
	if (ret)
		return (2);
	return (0);
}

void	free_all(pthread_t *tid, pthread_mutex_t *forks, t_philo_d *pdata)
{
	free(tid);
	free(forks);
	free(pdata);
}

long long int	current_time_ll(void)
{
	struct timeval	time_s;

	gettimeofday(&time_s, NULL);
	return (time_s.tv_sec * 1000LL + time_s.tv_usec / 1000);
}

long long int	timestamp(long long int t0)
{
	return (current_time_ll() - t0);
}

void	close_threads_mutex(t_params *params, t_data *data)
{
	int	i;

	i = 0;
	while (i < params->nb_philos)
		pthread_join(data->tid[i++], NULL);
	i = 0;
	while (i < params->nb_philos)
		pthread_mutex_destroy(&(data->forks[i++]));
	pthread_mutex_destroy(&(params->write_mutex));
	pthread_mutex_destroy(&(params->death_mutex));
}
