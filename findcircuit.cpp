#include "graph.hpp"
#include <iostream>
#include <algorithm>

//circuit includes all NodeId in the circuit C formed by the edge {node1_id,node2_id} and T,
	//in the form that the first and last entry in circuit are the same and the edges in C are the edges between successive entries in circuit
std::vector<ED::NodeId> find_circuit(ED::Graph const T, std::vector<ED::size_type> const levels, ED::NodeId const node1_id, ED::NodeId const node2_id)
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
		ED::size_type level_1=levels.at(prev_1);
		ED::size_type level_2=levels.at(prev_2);
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
// receives in "labels" the R-values for all nodes, in "label_sizes" the logarithmic size of the "pseudo-nodes", and the circuit for which the labels have to be upgraded
// assignes for all "old roots" whose partition classes are included in the circuit the new root and updates the sizes if necessary
void update_labels(std::vector<ED::NodeId> & labels, std::vector<ED::size_type> & label_sizes, std::vector<ED::NodeId> const circuit)
{
	std::vector<ED::NodeId> roots; // will save for all nodes in the circuit their current roots
	std::vector<ED::size_type> root_sizes; // will save for all the roots in roots the current size of their unions
	for(unsigned int i =0; i<circuit.size()-1;i++)
	{
		ED::NodeId candidate = circuit.at(i);
		//finds the root of the partition class the vertex at circuit.at(i) is currently in
		while(candidate!=labels.at(candidate)) 
			candidate=labels.at(candidate);
		roots.push_back(candidate);
		root_sizes.push_back(label_sizes.at(candidate));
	}
	std::vector<ED::size_type>::iterator it = max_element(root_sizes.begin(), root_sizes.end());
	ED::NodeId new_root = roots.at(*it);
	bool same_size = false;
	for(unsigned int i = 0; i<roots.size();i++)
	{
		labels.at(roots.at(i))= new_root;
		
		
		if(roots.at(i)!=new_root && root_sizes.at(i)==root_sizes.at(*it))
		{
			same_size=true;
		}
	}
	if(same_size==true)
		label_sizes.at(new_root)++;	
}


int main()
{
	ED::Graph T{5};
	std::vector<ED::NodeId> labels;
	labels.push_back(0);
	labels.push_back(1);
	labels.push_back(2);
	labels.push_back(3);
	labels.push_back(4);
	std::vector<ED::size_type> sizes;
	sizes.push_back(0);
	sizes.push_back(0);
	sizes.push_back(0);
	sizes.push_back(0);
	sizes.push_back(0);
	T.add_edge(0,1);
	T.add_edge(0,2);
	T.add_edge(1,2);
	T.add_edge(0,3);
	T.add_edge(0,4);
	T.add_edge(3,4);
	std::vector<ED::size_type> levels;
	levels.push_back(0);
	levels.push_back(1);
	levels.push_back(1);
	levels.push_back(1);
	levels.push_back(1);
	
	std::vector<ED::NodeId> circuit1 = find_circuit(T, levels,1,2);
	std::vector<ED::NodeId> circuit2 = find_circuit(T, levels,3,4);
	update_labels(labels, sizes, circuit1);
	update_labels(labels, sizes, circuit2);
	std::cout << "circuit1:" << std::endl;
	for(unsigned int i =0; i<circuit1.size();i++)
		std::cout << circuit1.at(i)<<"\n";
	std::cout << "circuit2:" << std::endl;
	for(unsigned int i =0; i<circuit2.size();i++)
		std::cout<< circuit2.at(i)<<"\n";
	std::cout << "label_sizes:" << std::endl;
	for(unsigned int i = 0; i<sizes.size();i++)
		std::cout<<i<<" - "<<sizes.at(i)<<"\n";
    std::cout << "labels:" << std::endl;
	for(unsigned int i = 0; i<labels.size();i++)
		std::cout<<i<<" - "<<labels.at(i)<<"\n";	
	return 0;
	
}
