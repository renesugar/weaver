/*
 * ===============================================================
 *    Description:  Node program to get a BTC address and all
 *                  of its transactions
 *
 *         Author:  Ayush Dubey, dubey@cs.cornell.edu
 *
 * Copyright (C) 2014, Cornell University, see the LICENSE file
 *                     for licensing agreement
 * ===============================================================
 */

#ifndef weaver_node_prog_get_btc_addr_h_
#define weaver_node_prog_get_btc_addr_h_

#include <string>

#include "common/property_predicate.h"
#include "db/remote_node.h"
#include "node_prog/node.h"
#include "node_prog/edge.h"
#include "node_prog/base_classes.h"
#include "node_prog/cache_response.h"

namespace node_prog
{
    struct get_btc_addr_params: public virtual Node_Parameters_Base
    {
        node_handle_t addr_handle;
        cl::node addr_node;
        std::vector<cl::node> txs;

        bool returning;
        db::remote_node addr_rn;

        get_btc_addr_params();
        ~get_btc_addr_params() { }
        uint64_t size() const;
        void pack(e::packer&) const;
        void unpack(e::unpacker&);

        // no caching
        bool search_cache() { return false; }
        cache_key_t cache_key() { return cache_key_t(); }
    };

    struct get_btc_addr_state: public virtual Node_State_Base
    {
        uint32_t outstanding_count;
        std::vector<cl::node> txs;

        get_btc_addr_state();
        ~get_btc_addr_state() { }
        uint64_t size() const;
        void pack(e::packer&) const;
        void unpack(e::unpacker&);
    };

   std::pair<search_type, std::vector<std::pair<db::remote_node, get_btc_addr_params>>>
   get_btc_addr_node_program(node &n,
       db::remote_node &rn,
       get_btc_addr_params &params,
       std::function<get_btc_addr_state&()> state_getter,
       std::function<void(std::shared_ptr<Cache_Value_Base>, std::shared_ptr<std::vector<db::remote_node>>, cache_key_t)>&,
       cache_response<Cache_Value_Base>*);
}

#endif
