#include "graph.hpp"
#include "subroutines.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

namespace ED
{
	
bool isinMatching(Graph & matching, NodeId id)
{
   return ((matching.node(id)).degree() > 0);
}

void initialize_levels(std::vector<int> & levels, NodeId const root_id)
{
	
	std::fill(levels.begin(), levels.end(), -1);
	levels.at(root_id) = 0;
	
}

void initialize_labels(std::vector<size_type> & labels, size_type num_nodes)
{
	for (size_type i = 0; i < num_nodes; i++) labels.at(i) = i;
}


NodeId find_exposed_vertex(Graph & matching, Graph const & graph)
{
	size_type num_nodes = matching.num_nodes();
	NodeId exposed_node_id = invalid_node_id;
	
	for (NodeId i = 0; i < num_nodes; i++)
	{
		if (((matching.node(i)).degree() == 0) && ((graph.node(i)).degree() != 0))
		{
			exposed_node_id = i;
		}
	}

	return exposed_node_id;
}

std::vector<NodeId> find_suitable_even_node (Graph const & graph, Graph & tree, std::vector<int> & levels)
{
	size_type num_nodes = tree.num_nodes();
	std::vector<NodeId> pair;
	
	for (NodeId candidate_id = 0; candidate_id < num_nodes; candidate_id ++)
	{
		// check if candidate node in Even(tree)
		if ((levels.at(candidate_id) % 2 == 1) || (levels.at(candidate_id) == -1)) continue;
		
		Node const & candidate_node = graph.node(candidate_id);
	
		for (size_type i = 0; i < (candidate_node.neighbors()).size(); i++)
		{
			NodeId candidate_neighbor_id = (candidate_node.neighbors()).at(i);
			if(levels.at(candidate_neighbor_id) % 2 == 0 || levels.at(candidate_neighbor_id) == -1)
			{
				pair.push_back(candidate_id);
				pair.push_back(candidate_neighbor_id);
				return pair;
			}	
		}	 
	}
	return pair; 
}

void tree_extension(Graph & tree, Graph & matching, NodeId const nodex_id, NodeId const nodey_id, std::vector<int> & levels)
{
   if (levels.at(nodey_id) != -1)
   {
	   std::cout << "Error: Node" << to_dimacs_id(nodey_id) << "is already in the tree!" << std::endl;
	   return;
   }
   NodeId const nodez_id = (matching.node(nodey_id)).neighbors()[0]; //in matching, every node has exactly one neighbor
   
   tree.add_edge(nodex_id, nodey_id);
   tree.add_edge(nodey_id, nodez_id);

   levels.at(nodey_id) = levels.at(nodex_id) + 1;
   levels.at(nodez_id) = levels.at(nodex_id) + 2;
   
}

void matching_augmentation (Graph & tree, NodeId const root_id, Graph & matching, NodeId const nodex_id, NodeId const nodey_id, std::vector<int> & levels, std::vector<size_type> &labels)  // add edge {node_add_id, node_neighbor_id}, delete edge{node_neighbor_id, node_del_id}
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
	   while ((levels.at(node_neighbor_id) < levels.at(node_add_id)) || (isinMatching(matching, node_neighbor_id) == false))
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

   
   //circuit includes all NodeId in the circuit C formed by the edge {node1_id,node2_id} and T,
	//in the form that the first and last entry in circuit are the same and the edges in C are the edges between successive entries in circuit
std::vector<ED::NodeId> find_circuit(ED::Graph const T, std::vector<int> const levels, ED::NodeId const node1_id, ED::NodeId const node2_id, std::vector<std::vector<ED::NodeId>> & all_circuits)
{
	std::vector<ED::NodeId> circuit;
	circuit.push_back(node1_id);
	circuit.push_back(node2_id);
	
	ED::NodeId prev_1=node1_id;
	ED::NodeId prev_2=node2_id;
	
	//to find the circuit, we move towards the root from node1_id and node2_id (these paths are unique in a tree) 
	//until we find a common node in both paths (note: this common node can also be node1_id or node2_id)
	while(prev_1!=prev_2)
	{
		int level_1=levels.at(prev_1);
		int level_2=levels.at(prev_2);
		ED::Node prev_node_1 = T.node(prev_1);
		ED::Node prev_node_2 = T.node(prev_2);
		bool found = false;
		for(unsigned int i=0;i<prev_node_1.degree() && found==false && level_1>=level_2;i++)
		{
			
			ED::NodeId neighbor1_id = prev_node_1.neighbors().at(i); 
			if(levels.at(neighbor1_id)==level_1-1)
			{
				found = true;
				prev_1 = neighbor1_id;
				std::vector<ED::NodeId>::iterator it = circuit.begin();
				circuit.insert(it,prev_1);
			}
		}
		found=false;
		for(unsigned int i=0;i<prev_node_2.degree() && found==false && level_2>=level_1; i++)
		{
			ED::NodeId neighbor2_id = prev_node_2.neighbors().at(i); 
			if(levels.at(neighbor2_id)==level_2-1)
			{
				found = true;
				prev_2 = neighbor2_id;
				circuit.push_back(prev_2);
			}
		}
	}
	all_circuits.push_back(circuit);
	return circuit;
}

void update_labels(std::vector<ED::NodeId> & labels, std::vector<ED::size_type> & label_sizes, std::vector<ED::NodeId> const circuit, ED::Graph & current_matching)
{
	std::vector<ED::NodeId> roots;
	std::vector<ED::size_type> root_sizes;

	for(unsigned int i =0; i<circuit.size()-1;i++)
	{
		ED::NodeId candidate = circuit.at(i);
		while(candidate!=labels.at(candidate))
			candidate=labels.at(candidate);
				
		roots.push_back(candidate);
		root_sizes.push_back(label_sizes.at(candidate));
	}
	unsigned int own_iterator = 0;
 	for(unsigned int i=1; i<root_sizes.size();i++)
 		if(root_sizes.at(i)>root_sizes.at(own_iterator))
 			own_iterator=i;
 	ED::NodeId new_root = roots.at(own_iterator);

	bool same_size = false;
	for(unsigned int i = 0; i<roots.size();i++)
	{
		labels.at(roots.at(i))= new_root;
				
		if(roots.at(i)!=new_root && root_sizes.at(i)==root_sizes.at(own_iterator))
  		{
			same_size=true;
		}
	}
	if(same_size==true)
		label_sizes.at(new_root)++;	
	
	for(unsigned int i=0; i<circuit.size()-1;i++)
		if(current_matching.node(circuit.at(i)).degree() !=0 && current_matching.node(circuit.at(i)).neighbors().at(0)==circuit.at(i+1))
			current_matching.delete_edge(circuit.at(i),circuit.at(i+1));
	
}

void remove_all_incident_edges(Graph & graph, NodeId id)
{
	Node const & node = graph.node(id);
	NodeId neighbor_number = node.degree();
	
	for (size_type i = 0; i < neighbor_number; i++)
	{
		graph.delete_edge(id, (node.neighbors()).at(i));
	}		
}
   
} //namespace ED
