/*
 * Asterisk -- An open source telephony toolkit.
 *
 * Copyright (C) 2012, Digium, Inc.
 *
 * Mark Michelson <mmmichelson@digium.com>
 *
 * See http://www.asterisk.org for more information about
 * the Asterisk project. Please do not directly contact
 * any of the maintainers of this project for assistance;
 * the project provides a web site, mailing lists and IRC
 * channels for your use.
 *
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 */


#ifndef _ASTERISK_THREADPOOL_H
#define _ASTERISK_THREADPOOL_H

struct ast_threadpool;
struct ast_taskprocessor;
struct ast_threadpool_listener;

struct ast_threadpool_listener_callbacks {
	/*!
	 * \brief Indicates that the state of threads in the pool has changed
	 *
	 * \param listener The threadpool listener
	 * \param active_threads The number of active threads in the pool
	 * \param idle_threads The number of idle threads in the pool
	 * \param zombie_threads The number of zombie threads in the pool
	 */
	void (*state_changed)(struct ast_threadpool_listener *listener,
			int active_threads,
			int idle_threads,
			int zombie_threads);
	/*!
	 * \brief Indicates that a task was pushed to the threadpool's taskprocessor
	 *
	 * \param listener The threadpool listener
	 * \param was_empty Indicates whether the taskprocessor was empty prior to adding the task
	 */
	void (*tps_task_pushed)(struct ast_threadpool_listener *listener,
			int was_empty);
	/*!
	 * \brief Indicates the threadpoo's taskprocessor has become empty
	 * 
	 * \param listener The threadpool's listener
	 */
	void (*emptied)(struct ast_threadpool_listener *listener);
};

/*!
 * \brief listener for a threadpool
 *
 * The listener is notified of changes in a threadpool. It can
 * react by doing things like increasing the number of threads
 * in the pool
 */
struct ast_threadpool_listener {
	/*! Callbacks called by the threadpool */
	struct ast_threadpool_listener_callbacks *callbacks;
	/*! Handle to the threadpool */
	struct ast_threadpool *threadpool;
	/*! User data for the listener */
	void *private_data;
};

/*!
 * \brief Create a new threadpool
 *
 * This function creates a threadpool and returns a taskprocessor. Tasks pushed
 * to this taskprocessor will be handled by the threadpool and will be reported
 * on the threadpool's listener.
 *
 * \param listener The listener the threadpool will notify of changes
 * \retval NULL Failed to create the threadpool
 * \retval non-NULL The associated taskprocessor
 */
struct ast_threadpool *ast_threadpool_create(struct ast_threadpool_listener *listener);

/*!
 * \brief Set the number of threads for the thread pool
 *
 * This number may be more or less than the current number of
 * threads in the threadpool.
 * 
 * \param threadpool The threadpool to adjust
 * \param size The new desired size of the threadpool
 */
void ast_threadpool_set_size(struct ast_threadpool *threadpool, unsigned int size);

#endif /* ASTERISK_THREADPOOL_H */
