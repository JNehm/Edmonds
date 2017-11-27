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
	NodeId root_id = find_exposed_vertex(matching, graph);	
	
	if(root_id == invalid_node_id) return matching;
	
	std::vector<std::vector<NodeId>> candidate_edges; //L in script
	add_outgoing_candidate_edges (graph, candidate_edges, root_id);
	std::vector<NodeId> labels (num_nodes);
	std::vector<NodeId> label_sizes(num_nodes);
	std::vector<int> levels (num_nodes);
	initialize_levels(levels, root_id);
	initialize_labels(labels, num_nodes);


	while (candidate_edges.size() > 0) 
	{
		std::vector<NodeId> pair = candidate_edges.at(0);
		NodeId nodex_id = pair.at(0);
		NodeId nodey_id = pair.at(1);
		
		//continue, if both nodes lie in the same circuit
		if (labels.at(nodex_id) == labels.at(nodey_id))
		{
			candidate_edges.erase(candidate_edges.begin());
			continue;
		}
		
		int level_y = levels.at(nodey_id);
		bool yinmatching = (matching.node(nodey_id)).degree();
		
		//Case 1: Augment matching 
		if ((level_y == -1) && (yinmatching == false))
		{
		
	//TO DO: COMPLETE AUGMENTATION STEP/ CASE 1
			//augment matching
			matching_augmentation (tree, root_id, matching, nodex_id, nodey_id, levels, labels);
						
			//Case 1.a: perfect matching
			if(matching.num_edges()== (num_nodes/2) && (num_nodes % 2 == 0))
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
					
				// check if all remaining nodes are isolated or only adjacent to matched nodes
				if (root_id == invalid_node_id)
				{
					std::cout << "Matching is maximal. " << std::endl;
					return matching;
				} 
				initialize_levels(levels, root_id);
				initialize_labels(labels, num_nodes);
				candidate_edges.clear();
				add_outgoing_candidate_edges (graph, candidate_edges, root_id);
			}
		}
		
		
		//Case 2: Extend tree 
		else if (level_y == -1 && yinmatching == true)
		{
			tree_extension (tree, graph, matching, nodex_id, nodey_id, levels, candidate_edges);
			candidate_edges.erase(candidate_edges.begin());
		}
		
		//Case 3: "Shrink circuit" (update labels)
		else if (level_y % 2 == 0 || labels.at(nodey_id) != nodey_id) //if both nodes are in Even(tree) or nodey is already in a circuit
		{
			std::vector<NodeId> const circuit = find_circuit(tree, levels, nodex_id, nodey_id);
			update_labels(labels, label_sizes, circuit, graph, candidate_edges, levels);
			candidate_edges.erase(candidate_edges.begin());
		}
	}
	std::cout << "Found a frustrated tree! G has no perfect matching." << std::endl;
	
	return matching;
}	
	
} //namespace ED

