#include "graph.hpp"
#include <fstream>
#include <string>
#include <iostream>


ED::Graph read_input(std::string filename)
{
	
	std::string nextline;
	std::ifstream graph_file (filename);
	if(graph_file)
	{
		
		ED::size_type check_num_edges;
		bool initialized = false;
		ED::NodeId num_nodes;
		while(initialized==false && getline(graph_file, nextline))
		{
			
			std::string start = nextline.substr(0,6);
			if(start=="p edge"&&initialized==false)
			{
				unsigned int gap = nextline.find(' ',7);
				std::string str_num_nodes=nextline.substr(7, gap-7);
			
				num_nodes=std::stoul(str_num_nodes,nullptr,0);
				
				check_num_edges=std::stoul(nextline.substr(gap+1),nullptr,0);
				initialized=true;
				
			}
		}
		
		ED::Graph result{num_nodes};
		
		
		while(getline(graph_file, nextline))
		{	
			
			if(nextline[0] == 'e')
			{
				unsigned int gap = nextline.find(' ',2);
				ED::NodeId first_node_id = std::stoul(nextline.substr(2,gap-2),nullptr,0);
				ED::NodeId second_node_id = std::stoul(nextline.substr(gap+1),nullptr,0);
				
				
				result.add_edge(ED::from_dimacs_id(first_node_id),ED::from_dimacs_id(second_node_id));
				
				
			}
			
		}	
		
		if(!check_num_edges || result.num_edges()!=check_num_edges)
			throw std::runtime_error("The number of encoded edges does not match the initial number\n");
	
	
	return result;
	}
	throw std::runtime_error("No such file found");
}

int main()
{
	
	ED::Graph mygraph = read_input("queen16_16.dmx");
	
	std::cout<<mygraph;
	
	
	return 0;	
}