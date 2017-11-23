#include <iostream>
#include <cstdlib>
#include <vector>

#include "graph.hpp"
#include "subroutines.hpp"

//! return graph consisting of path with \c num_nodes many vertices
static ED::Graph create_path(ED::NodeId num_nodes)
{
   ED::Graph result{num_nodes};

   for (ED::NodeId node_id = 0; node_id + 1 < num_nodes; ++node_id)
   {
      result.add_edge(node_id, node_id + 1);
   }
   // will be moved
   return result;
}


int main(int argc, char**)
{

   // create and output a path with argc many vertices
   
   ED::NodeId num_nodes = 7;
   
   ED::Graph graph = create_path(5*static_cast<ED::NodeId>(argc));
   ED::Graph test_graph = ED::Graph(num_nodes);

   std::vector<int> levels = ED::initialize_levels(num_nodes,0);
   
   for (ED::NodeId node_id = 0; node_id + 2 < num_nodes; ++node_id)
   {
	   test_graph.add_edge(node_id, node_id + 1);
   }
   test_graph.add_edge(2, num_nodes - 1);
   
   ED::Graph test_tree = ED::Graph(num_nodes);
   ED::Graph test_matching = ED::Graph(num_nodes);
 
   test_matching.add_edge(1,2);
   test_matching.add_edge(3,4);
 
   std::cout << "test_graph:" << test_graph << "\n";
   std::cout << "test_tree:" << test_tree << "\n";
   
   ED::tree_extension(test_tree,test_matching, 0,1, levels);   
   ED::tree_extension(test_tree,test_matching, 2,3, levels); 
   
   std::cout << "test_matching:" << test_matching << "\n";
   std::cout << "Modified test_tree after extension:" << test_tree << "\n";
	
   
	ED::matching_augmentation(test_tree, 0, test_matching, 4,5, levels);
	
   // std::cout << "Modified test_matching after edge augmentation:" << test_matching << "\n";
   // std::cout << "Modified test_tree after edge augmentation:" << test_tree << "\n";
   
   
   return EXIT_SUCCESS;
}
