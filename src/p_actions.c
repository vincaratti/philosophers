/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_actions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcaratti <vcaratti@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:41:36 by vcaratti          #+#    #+#             */
/*   Updated: 2024/11/16 17:15:54 by vcaratti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	philo_print(char *str, t_philo_d *pdata)
{
	if (d_status(pdata->params) != 1)
	{
		pthread_mutex_lock(&(pdata->params->write_mutex));
		printf("%lli\t%d %s\n", timestamp(pdata->params->t0),
			pdata->pid + 1, str);
		pthread_mutex_unlock(&(pdata->params->write_mutex));
	}
}

void	philo_wait(t_philo_d *pdata, int ttw)
{
	long long int	t;

	t = timestamp(pdata->params->t0);
	while (!d_status(pdata->params))
	{
		if (timestamp(pdata->params->t0) - t >= ttw)
			return ;
		usleep(60);
	}
}

void	philo_eat(t_philo_d *pdata)
{
	pthread_mutex_lock(pdata->myforks[0]);
	philo_print("has taken fork", pdata);
	if (pdata->params->nb_philos == 1)
	{
		pthread_mutex_unlock(pdata->myforks[0]);
		pdata->me_dead = 1;
		return ;
	}
	pthread_mutex_lock(pdata->myforks[1]);
	philo_print("has taken fork", pdata);
	philo_print("is eating", pdata);
	philo_wait(pdata, pdata->params->tte);
	pthread_mutex_lock(&(pdata->params->death_mutex));
	pdata->last_ate = timestamp(pdata->params->t0);
	pthread_mutex_unlock(&(pdata->params->death_mutex));
	pdata->nb_ate++;
	pthread_mutex_unlock(pdata->myforks[0]);
	pthread_mutex_unlock(pdata->myforks[1]);
}

void	philo_think(t_philo_d *pdata)
{
	philo_print("is thinking", pdata);
}

void	philo_sleep(t_philo_d *pdata)
{
	philo_print("is sleeping", pdata);
	philo_wait(pdata, pdata->params->tts);
}
