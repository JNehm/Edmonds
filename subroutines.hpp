#ifndef SUBROUTINES_HPP
#define SUBROUTINES_HPP

/**
   @file subroutines.hpp

   @brief This file provides subroutines needed for the perfect matching algorithm.
**/

//#include <vector>

namespace ED
{
   using size_type = std::size_t;
   using NodeId = size_type;
   using DimacsId = size_type;
	
   /**
      ONLY WORKS FOR MATCHING-GRAPHS. 
	  Returns whether a node is in the matching by checking if the node has positive degree. 
   **/
   bool isinMatching(Graph & matching, NodeId id);
	
	
	/**
		Initializes the level-values for every new tree. Assign 0 for node, -1 for all vertices that are not in the tree.
	**/
   std::vector<int> initialize_levels(NodeId const num_nodes, NodeId const root_id);
	
   /** 
	   Execution of Step 2 in the perfect matching algorithm: 
	   Case: x in Even(tree), adjacent to y, y is not in V(tree) but covered by the matching 
	   --> extend the tree by {x,y},{y,z} where {y,z} is the edge in the matching
   **/	
   void tree_extension(Graph & tree, Graph & matching, NodeId const nodex_id, NodeId const nodey_id, std::vector<int> const levels);

	/**
	   Execution of augmentation in Step 1 in the perfect matching algorithm:
	   Case: x in Even(tree), adjacent to y, y is not in V(tree) and y is exposed by the matching
	   --> augment the matching ("move matching by one")
   **/
   void matching_augmentation (Graph & tree, NodeId const root_id, Graph & matching, NodeId const nodex_id, NodeId const nodey_id, std::vector<int> levels);
   
} //namespace ED




#endif /* SUBROUTINES_HPP */
