#include "graph.hpp"
#include "subroutines.hpp"
#include <iostream>

namespace ED
{
	
	bool isinMatching(Graph & matching, NodeId id)
	{
	   return ((matching.node(id)).degree() > 0);
	}

	std::vector<int> initialize_levels(NodeId const num_nodes, NodeId const root_id)
	{
		std::vector<int> levels(num_nodes, -1);
		levels.at(root_id) = 0;
		return levels;
	}
	
   void tree_extension(Graph & tree, Graph & matching, NodeId const nodex_id, NodeId const nodey_id, std::vector<int> levels)
   {
	   if (levels.at(nodey_id) != -1)
	   {
		   std::cout << "Error: Node" << to_dimacs_id(nodey_id) << "is already in the matching!" << std::endl;
		   return;
	   }
	   NodeId const nodez_id = (matching.node(nodey_id)).neighbors()[0]; //in matching, every node has exactly one neighbor
	   
	   tree.add_edge(nodex_id, nodey_id);
	   tree.add_edge(nodey_id, nodez_id);
	
       levels.at(nodey_id) = levels.at(nodex_id) + 1;
	   levels.at(nodez_id) = levels.at(nodex_id) + 2;
	   
   }
   
   void matching_augmentation (Graph & tree, NodeId const root_id, Graph & matching, NodeId const nodex_id, NodeId const nodey_id, std::vector<int> levels)  // add edge {node_add_id, node_neighbor_id}, delete edge{node_neighbor_id, node_del_id}
   {
	   NodeId node_add_id = root_id;
	
   	   //arbitrary initialization
	   NodeId node_neighbor_id = root_id;
	   NodeId node_del_id = root_id;
	   
	   if (isinMatching(matching, root_id)) 
	   {
		   std::cout << "Error: Tree can not be augmented, the root is already in the matching! (i.e., this was not an M-alternating tree to begin with)";
		   return;
	   }
	  
	   while(node_add_id != nodex_id)
	   {	   
		   int i = 0;
		   node_neighbor_id = (tree.node(node_add_id)).neighbors()[0];
		   
    	   //choose the right neighbor node in the tree (i.e. which is farther away from root and in the matching)
		   while ((levels.at(node_neighbor_id) < levels.at(node_add_id)) or (isinMatching(matching, node_neighbor_id) == false))
		   {
			   ++i;
			   node_neighbor_id = (tree.node(node_add_id)).neighbors()[i];   
		   }
		   node_del_id = (matching.node(node_neighbor_id)).neighbors()[0];
		   
		   matching.add_edge(node_add_id, node_neighbor_id);
		   matching.delete_edge(node_neighbor_id, node_del_id);
		   
		   node_add_id = node_del_id;
	   }
	   matching.add_edge(nodex_id, nodey_id);		   
   }   
   
} //namespace ED
