#include "graph.hpp"
#include "subroutines.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

namespace ED
{
	
Graph perfect_matching(Graph const graph)
{
	NodeId num_nodes = graph.num_nodes();
	Graph matching (num_nodes);
	Graph tree (num_nodes);
	NodeId root_id = 0;	
	
	std::vector<NodeId> labels (num_nodes);
	std::vector<NodeId> label_sizes(num_nodes);
	std::vector<int> levels (num_nodes);
	initialize_levels(levels, root_id);
	initialize_labels(labels, num_nodes);

	std::vector<NodeId> pair(2);
	
	
	
	while (find_suitable_even_node(graph, tree, levels).size() != 0) 
	{
		pair = find_suitable_even_node(graph, tree, levels);
		NodeId nodex_id = pair.at(0);
		NodeId nodey_id = pair.at(1);
		
		int level_y = levels.at(nodey_id);
		bool yinmatching = (matching.node(nodey_id)).degree();
		
		//Case 1: Augment matching 
		if ((level_y == -1) && (yinmatching == false))
		{
		
	//TO DO: COMPLETE AUGMENTATION STEP/ CASE 1
			//augment matching
			matching_augmentation (tree, root_id, matching, nodex_id, nodey_id, levels, labels);
						
			//Case 1.a: perfect matching
			if(matching.num_edges()== (num_nodes/2))
			{
				std::cout << "Found perfect matching:" << std::endl;
				std::cout << matching << std::endl;
				return matching;
			}
			//Case 1.b: grow a new tree
			else
			{	
				//matching = unshrunk_matching;
				tree = Graph(num_nodes);
				root_id = find_exposed_vertex (matching, graph);
				if (root_id == invalid_node_id)
				{
					std::cout << "Only isolated nodes left! Matching is maximal. " << std::endl;
					return matching;
				} 
				initialize_levels(levels, root_id);
			}
		}
		
		
		//Case 2: Extend tree 
		else if (level_y == -1 && yinmatching == true)
		{
			tree_extension (tree, matching, nodex_id, nodey_id, levels);
		}
		
		//Case 3: "Shrink circuit" (update labels)
		else
		{
			std::vector<NodeId> const circuit = find_circuit(tree, levels, nodex_id, nodey_id);
			update_labels(labels, label_sizes, circuit);
		}
	}
	std::cout << "Found a frustrated tree! G has no perfect matching." << std::endl;
	
	return matching;
}	
	
} //namespace ED

