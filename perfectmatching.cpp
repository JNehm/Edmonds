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
	std::vector<std::vector<NodeId>> all_circuits;


	while (candidate_edges.size() > 0) 
	{
		std::vector<NodeId> pair = candidate_edges.at(0);
		NodeId nodex_id = pair.at(0);
		NodeId nodey_id = pair.at(1);
		NodeId rootx_id = nodex_id;
		NodeId rooty_id = nodey_id;
		while(labels.at(rootx_id)!=rootx_id)
			rootx_id = labels.at(rootx_id);
		while(labels.at(rooty_id)!=rooty_id)
			rooty_id = labels.at(rooty_id);
		//continue, if both nodes lie in the same circuit
		if (rootx_id == rooty_id)
		{
			candidate_edges.erase(candidate_edges.begin());
			continue;
		}
		//continue, if the edge is already in the tree
		std::vector<NodeId> x_neighbors = tree.node(nodex_id).neighbors();
		if(find(x_neighbors.begin(),x_neighbors.end(),nodey_id)!=x_neighbors.end())
		{
			candidate_edges.erase(candidate_edges.begin());
			continue;			
		}
		
		int level_y = levels.at(nodey_id);
		bool yinmatching = (matching.node(nodey_id)).degree();
		
		//Case 1: Augment matching 
		if ((level_y == -1) && (yinmatching == false))
		{
		
			//augment matching
			matching_augmentation (tree, root_id, matching, nodex_id, nodey_id, levels, labels);			
			unshrink_circuits(all_circuits, matching);			
			all_circuits.clear();
			//Case 1.a: perfect matching
			if(2*matching.num_edges()== num_nodes)
			{
				std::cout << "Found perfect matching:" << std::endl;
				return matching;
			}
			//Case 1.b: grow a new tree
			else
			{	
				tree = Graph(num_nodes);
				root_id = find_exposed_vertex (matching, graph);
					
				// check if all remaining nodes are isolated or only adjacent to matched nodes
				if (root_id == invalid_node_id)
				{
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
			std::vector<NodeId> const circuit = find_circuit(tree, levels, nodex_id, nodey_id, all_circuits, labels);
			update_labels(labels, label_sizes, circuit, matching, graph, candidate_edges, levels);
			
			candidate_edges.erase(candidate_edges.begin());
		}
		else if (level_y % 2 == 1 && labels.at(nodey_id) == nodey_id)
		{
			candidate_edges.erase(candidate_edges.begin());
		}
	}
	
	unshrink_circuits(all_circuits, matching);		
	
	all_circuits.clear();
	
	return matching;
}	
	
} //namespace ED

