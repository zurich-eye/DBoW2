#include <glog/logging.h>

#include "DBoW2/DBoW2.h"

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  CHECK_EQ(argc, 2) << "Usage: " << argv[0] << " <classical-format vocab>";
  const std::string file_name(argv[1]);

  BriefVocabulary vocabulary(file_name);
  vocabulary.saveProto(file_name + ".proto");
}
