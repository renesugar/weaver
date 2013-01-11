/*
 * =====================================================================================
 *
 *       Filename:  threadpool.h
 *
 *    Description:  Thread pool for central server.
 *
 *        Version:  1.0
 *        Created:  01/09/2013 04:17:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ayush Dubey (), dubey@cs.cornell.edu
 *   Organization:  Cornell University
 *
 * =====================================================================================
 */

#ifndef __CS_THREADPOOL__
#define __CS_THREADPOOL__

//C++
#include <vector>
#include <deque>
#include <thread>

//po6
#include <po6/threads/mutex.h>
#include <po6/threads/cond.h>

//Weaver
#include "../central.h"
#include "../../message/message.h"

namespace central_coordinator
{
namespace thread
{
    class pool;
    void thread_loop (pool *tpool);

    class unstarted_thread
    {
        public:
            unstarted_thread (
                void (*f) (central_coordinator::central*,
                           std::shared_ptr<message::message>, 
                           enum message::msg_type),
                central_coordinator::central *s,
                std::shared_ptr<message::message> m,
                enum message::msg_type mtype);

        public:
            void (*func) (central_coordinator::central*,
                          std::shared_ptr<message::message>, 
                          enum message::msg_type);
            central_coordinator::central *server;
            std::shared_ptr<message::message> msg;
            enum message::msg_type m_type;
    };

    inline
    unstarted_thread :: unstarted_thread ( 
            void (*f) (central_coordinator::central*, 
                       std::shared_ptr<message::message>,
                       enum message::msg_type),
            central_coordinator::central *s,
            std::shared_ptr<message::message> m,
            enum message::msg_type mtype)
        : func (f)
        , server (s)
        , msg (m)
        , m_type (mtype)
    {
    }

    class pool
    {
        public:
            pool (int n_threads);

        public:
            int num_threads;
            std::deque<std::unique_ptr<unstarted_thread>> queue;
            std::vector<std::thread> threads;
            po6::threads::mutex queue_mutex;
            po6::threads::cond empty_queue_cond;
        
        public:
            void add_request (std::unique_ptr<unstarted_thread> t);
    };

    inline
    pool :: pool (int n_threads)
        : num_threads (n_threads)
        , empty_queue_cond (&queue_mutex)
    {
        int i;
        std::unique_ptr<std::thread> t;
        for (i = 0; i < num_threads; i++)
        {
            t.reset (new std::thread (thread_loop, this));
            t->detach();
        }
    }

    inline void
    pool :: add_request (std::unique_ptr<unstarted_thread> t)
    {
        queue_mutex.lock();
        if (queue.empty())
        {
            empty_queue_cond.signal();
        }
        queue.push_back (std::move(t));
        queue_mutex.unlock();
    }

    void
    thread_loop (pool *tpool)
    {
        std::unique_ptr<unstarted_thread> thr;
        while (true)
        {
            tpool->queue_mutex.lock();
            while (tpool->queue.empty())
            {
                tpool->empty_queue_cond.wait();
            }
            thr = std::move(tpool->queue.front());
            tpool->queue.pop_front();
            if (!tpool->queue.empty())
            {
                tpool->empty_queue_cond.signal();
            }
            tpool->queue_mutex.unlock();
            (*(thr->func)) (thr->server, thr->msg, thr->m_type);
        }
    }
} //namespace thread
} //namespace db

#endif //__CS_THREADPOOL__
