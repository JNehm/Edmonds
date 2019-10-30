#include "graph.hpp"
#include "subroutines.hpp"
#include "perfectmatching.hpp"
#include "subroutines.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>

namespace ED 
{
	Graph max_cardinality (std::string filename)
	{
		Graph graph_temp = read_input(filename);
		size_type num_nodes = graph_temp.num_nodes();
		Graph max_matching (num_nodes);
		
		//Use the perfect matching algorithm to find frustrated trees. For each, "remove" the frustrated tree from the copy of the graph, graph_temp, by
		//removing all edges incident to the nodes in the frustrated tree. Stop when all nodes have been considered (iff graph_temp has no edges left).
		while (graph_temp.num_edges() > 0)
		{
			Graph matching_temp = perfect_matching(graph_temp);
			
			//copy the edges in matching_temp to max_matching, delete edges incident to respective nodes from graph_temp
			for(NodeId id = 0; id < num_nodes; id++)
			{
				Node const temp_node = matching_temp.node(id);
				if (temp_node.degree() == 1)
				{				
					NodeId neighbor_id = (temp_node.neighbors()).at(0);
					max_matching.add_edge(id, neighbor_id);
					remove_all_incident_edges(graph_temp, id);
					remove_all_incident_edges(graph_temp, neighbor_id);
					remove_all_incident_edges (matching_temp, id);
				}
			}
		}
		return max_matching;
	}
	
} //namespace ED

int main(int argc, char** argv)
{	
	std::string filename;
	for(int i=0; i<argc; i++)
	{
		if(std::strcmp(argv[i], "--graph")==0)
		{
			if(i==argc-1) std::cout<<"The command '--graph' needs the name of a dmx-file as input"<<std::endl;
			if(i+1 < argc)
				filename = argv[i+1];
		}
	}
	
	
	std::cout << ED::max_cardinality(filename) << std::endl;
}