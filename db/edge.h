/*
 * ===============================================================
 *    Description:  Graph edge class 
 *
 *        Created:  Tuesday 16 October 2012 02:28:29  EDT
 *
 *         Author:  Ayush Dubey, dubey@cs.cornell.edu
 * 
 * Copyright (C) 2013, Cornell University, see the LICENSE file
 *                     for licensing agreement
 * ===============================================================
 */

#ifndef weaver_db_element_edge_h_
#define weaver_db_element_edge_h_

#include <stdint.h>
#include <vector>

#include "node_prog/edge.h"
#include "node_prog/prop_list.h"
#include "node_prog/property.h"
#include "db/remote_node.h"
#include "db/element.h"

namespace db
{
namespace element
{
    class edge : public node_prog::edge
    {
        public:
            edge();
            edge(uint64_t id, vc::vclock &vclk, uint64_t remote_loc, uint64_t remote_id);
            edge(uint64_t id, vc::vclock &vclk, remote_node &rn);
            ~edge() { }

        public:
            element base;
            remote_node nbr; // out-neighbor for this edge
            uint32_t msg_count; // number of messages sent on this link
            bool migr_edge; // true if this edge was migrated along with parent node
            void traverse(); // indicate that this edge was traversed; useful for migration statistics

            remote_node &get_neighbor();
            node_prog::prop_list get_properties();
            bool has_property(std::pair<std::string, std::string> &p);
            bool has_all_properties(std::vector<std::pair<std::string, std::string>> &props);
            uint64_t get_id() const { return base.get_id(); }
    };
}
}

#endif
