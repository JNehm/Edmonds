#include "graph.hpp" // always include corresponding header first

#include <ostream>
#include <stdexcept>
#include <fstream>
#include <string>
#include <iostream>


namespace ED
{
/////////////////////////////////////////////
//! \c Node definitions
/////////////////////////////////////////////

void Node::add_neighbor(NodeId const id)
{
   _neighbors.push_back(id);
}

void Node::delete_neighbor(NodeId const id)
{
	_neighbors.erase(std::remove(_neighbors.begin(), _neighbors.end(), id), _neighbors.end());
} 

/////////////////////////////////////////////
//! \c Graph definitions
/////////////////////////////////////////////

Graph::Graph(NodeId const num_nodes)
   :
   _nodes(num_nodes),
   _num_edges(0)
{}

void Graph::add_edge(NodeId node1_id, NodeId node2_id)
{
   if (node1_id == node2_id)
   {
      throw std::runtime_error("ED::Graph class does not support loops!");
   }

   // minimum redundancy :-), maybe a bit overkill...
   auto impl = [this](NodeId a, NodeId b)
   {
      Node & node = _nodes.at(a);
      node.add_neighbor(b);
   };

   impl(node1_id, node2_id);
   impl(node2_id, node1_id);

   ++_num_edges;
}

void Graph::delete_edge(NodeId node1_id, NodeId node2_id)
{
   Node & node1 = _nodes.at(node1_id);
   Node & node2 = _nodes.at(node2_id);
   size_type former_degree1 = node1.degree(); 
   
   node1.delete_neighbor(node2_id);
   node2.delete_neighbor(node1_id);
   
   _num_edges -= former_degree1 - node1.degree();
}

std::ostream & operator<<(std::ostream & str, Graph const & graph)
{
   str << "c This encodes a graph in DIMACS format\n"
       << "p edge " << graph.num_nodes() << " " << graph.num_edges() << "\n";

   for (NodeId node_id = 0; node_id < graph.num_nodes(); ++node_id)
   {
      auto const & node = graph.node(node_id);

      for (auto const & neighbor_id : node.neighbors())
      {
         // output each edge only once
         if (node_id < neighbor_id)
         {
            str << "e " << to_dimacs_id(node_id) << " " << to_dimacs_id(neighbor_id) << "\n";
         }
      }
   }

   str << std::flush;
   return str;
}


/////////////////////////////////////////////
//! global functions
/////////////////////////////////////////////

NodeId from_dimacs_id(DimacsId const dimacs_id)
{
   if (dimacs_id == 0)
   {
      throw std::runtime_error("Invalid (0) DIMACS id.");
   }

   return static_cast<NodeId>(dimacs_id - 1);
}

DimacsId to_dimacs_id(NodeId const node_id)
{
   if (node_id == std::numeric_limits<NodeId>::max())
   {
      throw std::runtime_error("Invalid (inf) node id.");
   }

   return static_cast<DimacsId>(node_id + 1);
}


Graph read_input(std::string filename)
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


} // namespace ED
