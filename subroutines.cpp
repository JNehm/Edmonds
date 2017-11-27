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
			NodeId degree = (graph.node(i)).degree();
			bool all_neighbors_matched = true;
			
			//check if all neighbor nodes are matched (if yes, return invalid_node_id)
			for (NodeId neighbor = 0; neighbor < degree; neighbor++)
			{
				if ((matching.node(neighbor)).degree() == 0) all_neighbors_matched = false; 
			}
			if (all_neighbors_matched == false) exposed_node_id = i;
			break;
		}
	}

	return exposed_node_id;
}


void tree_extension(Graph & tree, Graph const & graph, Graph & matching, NodeId const nodex_id, NodeId const nodey_id, std::vector<int> & levels, std::vector<std::vector<NodeId>> & candidate_edges)
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
   
   add_outgoing_candidate_edges (graph, candidate_edges, nodez_id);
}

   
   //circuit includes all NodeId in the circuit C formed by the edge {node1_id,node2_id} and T,
	//in the form that the first and last entry in circuit are the same and the edges in C are the edges between successive entries in circuit
std::vector<ED::NodeId> find_circuit(ED::Graph const T, std::vector<int> const levels, ED::NodeId const node1_id, ED::NodeId const node2_id)
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
	
	return circuit;
}

void add_outgoing_candidate_edges (Graph const & graph, std::vector<std::vector<NodeId>> & candidate_edges, NodeId nodex_id)
{
	Node const node = graph.node(nodex_id);
	size_type degree = node.degree();
	
	for (NodeId i = 0; i < degree; i++)
	{
		std::vector<NodeId> pair;
		pair.push_back(nodex_id);
		pair.push_back((node.neighbors()).at(i));
		candidate_edges.push_back(pair);
	}
}
	

void update_labels(std::vector<ED::NodeId> & labels, std::vector<ED::size_type> & label_sizes, std::vector<ED::NodeId> const circuit, Graph consthelp & graph, std::vector<std::vector<NodeId>> & candidate_edges, std::vector<int> const levels)
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
	
	//add all outgoing edges of odd-degree nodes in the circuit to candidate_edges
	for (NodeId i = 0; i < circuit.size()-1; i++)
	{
		if (levels.at(circuit.at(i)) % 2 == 1) add_outgoing_candidate_edges(graph, candidate_edges, circuit.at(i));
	}
}

void remove_all_incident_edges(Graph & graph, NodeId id)
{
	Node const & node = graph.node(id);
	NodeId neighbor_number = node.degree();
	
	for (size_type i = 0; i < neighbor_number; i++)
	{
		graph.delete_edge(id, (node.neighbors()).at(0));
	}		
}
   
} //namespace ED
