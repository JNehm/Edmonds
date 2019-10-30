#ifndef PERFECTMATCHINGALG_HPP
#define PERFECTMATCHINGALG_HPP

/**
   @file perfectmatchingalg.hpp

   @brief This file provides the modified perfect matching algorithm needed for every iteration in the maximum cardinality algorithm.
**/

//#include <vector>

namespace ED
{
   using size_type = std::size_t; 
   using NodeId = size_type;
   using DimacsId = size_type;	

   Graph perfect_matching(Graph const graph);
	
} //namespace ED




#endif /* PERFECTMATCHINGALG_HPP */