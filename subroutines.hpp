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
   void initialize_levels(std::vector<int> & levels, NodeId const root_id);
   
   /**
		Initializes the label-values for every new tree (--> unionfind, every node points to itself at first).
   **/
   void initialize_labels(std::vector<size_type> & labels, size_type num_nodes);
   
   // Find Matching-exposed vertex that is not isolated (return constexpr invalid_node_id if only isolated nodes left).
   NodeId find_exposed_vertex(Graph & matching, Graph const & graph);
   
   /**
		Check if while condition in perfect matching algorithm is fulfilled (if there exists an even-degree node in the tree with an odd-degree neighbor) and if yes, returns the pair.
   **/
   std::vector<NodeId> find_suitable_even_node (Graph const & graph, Graph & tree, std::vector<int> & levels);
   
   /** 
	   Execution of Step 2 in the perfect matching algorithm: 
	   Case: x in Even(tree), adjacent to y, y is not in V(tree) but covered by the matching 
	   --> extend the tree by {x,y},{y,z} where {y,z} is the edge in the matching
   **/	
   void tree_extension(Graph & tree, Graph & matching, NodeId const nodex_id, NodeId const nodey_id, std::vector<int> & levels);

	/**
	   Execution of augmentation in Step 1 in the perfect matching algorithm:
	   Case: x in Even(tree), adjacent to y, y is not in V(tree) and y is exposed by the matching
	   --> augment the matching ("move matching by one")
   **/
   void matching_augmentation (Graph & tree, NodeId const root_id, Graph & matching, NodeId const nodex_id, NodeId const nodey_id, std::vector<int> & levels, std::vector<size_type> &labels);
   
    /**
	circuit includes all NodeId in the circuit C formed by the edge {node1_id,node2_id} and T,
	in the form that the first and last entry in circuit are the same and the edges in C are the edges between successive entries in circuit
	**/
	std::vector<ED::NodeId> find_circuit(ED::Graph const T, std::vector<int> const levels, ED::NodeId const node1_id, ED::NodeId const node2_id);

	void update_labels(std::vector<ED::NodeId> & labels, std::vector<ED::size_type> & label_sizes, std::vector<ED::NodeId> const circuit);

	void remove_all_incident_edges(Graph & graph, NodeId id);
	
	
} //namespace ED




#endif /* SUBROUTINES_HPP */
