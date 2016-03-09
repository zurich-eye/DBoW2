/**
 * File: FeatureVector.cpp
 * Date: November 2011
 * Author: Dorian Galvez-Lopez
 * Description: feature vector
 * License: see the LICENSE.txt file
 *
 */

#include <map>
#include <vector>
#include <iostream>

#include <glog/logging.h>

#include "DBoW2/FeatureVector.h"

namespace DBoW2 {

// ---------------------------------------------------------------------------

FeatureVector::FeatureVector(void)
{
}

// ---------------------------------------------------------------------------

FeatureVector::~FeatureVector(void)
{
}

// ---------------------------------------------------------------------------

void FeatureVector::addFeature(NodeId id, unsigned int i_feature)
{
  FeatureVector::iterator vit = this->lower_bound(id);
  
  if(vit != this->end() && vit->first == id)
  {
    vit->second.push_back(i_feature);
  }
  else
  {
    vit = this->insert(vit, FeatureVector::value_type(id, 
      std::vector<unsigned int>() ));
    vit->second.push_back(i_feature);
  }
}

// ---------------------------------------------------------------------------

std::ostream& operator<<(std::ostream &out, 
  const FeatureVector &v)
{
  if(!v.empty())
  {
    FeatureVector::const_iterator vit = v.begin();
    
    const std::vector<unsigned int>* f = &vit->second;

    out << "<" << vit->first << ": [";
    if(!f->empty()) out << (*f)[0];
    for(unsigned int i = 1; i < f->size(); ++i)
    {
      out << ", " << (*f)[i];
    }
    out << "]>";
    
    for(++vit; vit != v.end(); ++vit)
    {
      f = &vit->second;
      
      out << ", <" << vit->first << ": [";
      if(!f->empty()) out << (*f)[0];
      for(unsigned int i = 1; i < f->size(); ++i)
      {
        out << ", " << (*f)[i];
      }
      out << "]>";
    }
  }
  
  return out;  
}

void FeatureVector::filter(const std::vector<unsigned int>& remaining_indices)
{
  typedef std::map<unsigned long, NodeId> InverseMap;
  InverseMap inverse_map;
  for (const value_type& node_indices : *this)
  {
    for (const unsigned int index : node_indices.second)
    {
      CHECK(inverse_map.emplace(index, node_indices.first).second);
    }
  }

  FeatureVector filtered;
  for (const unsigned int index : remaining_indices)
  {
    InverseMap::iterator found = inverse_map.find(index);
    CHECK(found != inverse_map.end());
    filtered.addFeature(found->second, found->first);
  }

  swap(filtered);
}

// ---------------------------------------------------------------------------

} // namespace DBoW2
