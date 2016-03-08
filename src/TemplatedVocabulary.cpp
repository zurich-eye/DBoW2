#include "DBoW2/TemplatedVocabulary.h"

#include "DBoW2/FBrief.h"
#include "DBoW2/FSurf64.h"

namespace DBoW2 {

template<>
void TemplatedVocabulary<DBoW2::FBrief::TDescriptor, DBoW2::FBrief>::loadProto(
    const std::string& file_name)
{
  std::ifstream file(file_name.c_str());
  CHECK(file.is_open()) << "Couldn't open " << file_name;

  google::protobuf::io::IstreamInputStream raw_in(&file);
  google::protobuf::io::CodedInputStream coded_in(&raw_in);
  coded_in.SetTotalBytesLimit(std::numeric_limits<int>::max(), -1);

  proto::Vocabulary vocabulary_proto;
  CHECK(vocabulary_proto.ParseFromCodedStream(&coded_in));

  m_k = vocabulary_proto.k();
  m_L = vocabulary_proto.l();
  m_scoring = static_cast<ScoringType>(vocabulary_proto.scoring_type());
  m_weighting = static_cast<WeightingType>(vocabulary_proto.weighting_type());

  createScoringObject();

  // nodes
  m_nodes.resize(vocabulary_proto.nodes_size() + 1); // +1 to include root
  m_nodes[0].id = 0;

  for(const proto::Node& node : vocabulary_proto.nodes())
  {
    const NodeId node_id = node.node_id();
    const NodeId parent_id = node.parent_id();

    m_nodes[node_id].id = node.node_id();
    m_nodes[node_id].parent = node.parent_id();
    m_nodes[node_id].weight = node.weight();

    m_nodes[parent_id].children.push_back(node_id);

    // For now only works with BRIEF.
    std::vector<unsigned long> descriptor_blocks;
    descriptor_blocks.reserve(node.node_descriptor_size());
    for (const uint64_t block : node.node_descriptor())
    {
      descriptor_blocks.emplace_back(block);
    }

    m_nodes[node_id].descriptor.append(
        descriptor_blocks.begin(), descriptor_blocks.end());
  }

  m_words.resize(vocabulary_proto.words_size());

  for(const proto::Word& word : vocabulary_proto.words())
  {
    const WordId word_id = word.word_id();
    const NodeId node_id = word.node_id();

    m_nodes[node_id].word_id = word_id;
    m_words[word_id] = &m_nodes[node_id];
  }
}

template<>
void TemplatedVocabulary<
DBoW2::FSurf64::TDescriptor, DBoW2::FSurf64>::loadProto(
    const std::string& file_name)
{
  LOG(FATAL) << "Not implemented yet!";
}

}  // namespace DBoW2
