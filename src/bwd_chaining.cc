#include "bwd_chaining.h"

void BwdChaining::DataPrint(std::ostream& out) {
  out << "PART 1. Data";
  out << "\n   1) Rules:\n";
  for(int i = 0; i < rules_.size(); ++i) {
    out << "      R" << i + 1 << ": ";
    PrintLHS(out, rules_[i].first); 
    out << " -> " << rules_[i].second << std::endl;
  }
  out << "   2) Facts ";
  PrintInitialFacts(out);
  out << std::endl << "   3) Goal " << goal_ << std::endl;
}

void BwdChaining::PrintLHS(std::ostream& out, const std::string kRuleLhs) {
  for (int i = 0; i < kRuleLhs.length(); ++i) {
    out << kRuleLhs[i];
    if (i != kRuleLhs.length() - 1) {
      out << ", ";
    } 
  }
}

void BwdChaining::PrintInitialFacts(std::ostream& out) {
  for (int i = 0; i < initial_facts_.size(); ++i) {
    out << initial_facts_[i];
    if (i != initial_facts_.size() - 1) {
      out << ", ";
    } 
  }
}

void BwdChaining::PrintInferredFacts(std::ostream& out) {
  for  (auto& it : inferred_facts_) {
    out << it;
    if (&it != &(*inferred_facts_.rbegin())) {
      out << ", ";
    } 
  }
}

bool BwdChaining::IsAnInitialFact(const char kGoal) {
  bool flag = false;
  for (int j = 0; j < initial_facts_.size(); ++j) {
    if (initial_facts_[j] == kGoal) {
      flag = true;
    }
  }
  return flag;
}

bool BwdChaining::Inferred(const char kGoal){
  return inferred_facts_.find(kGoal) != inferred_facts_.end();
}

// Helper function to print depth markers
void BwdChaining::PrintDepthMarkers(int depth) {
  for (int i = 0; i < depth; ++i) {
    output_file << "-";
  }
}

void BwdChaining::EraseSons(char goal) {
  for (int i = 0; i < rules_.size(); ++i) {
    if (goal == rules_[i].second) {
      for (int k = 0; k < rules_[i].first.size(); ++k) {
        if (!IsAnInitialFact(rules_[i].first[k]) && Inferred(rules_[i].first[k])) {
          if (path_.size() >= 1) {
            path_.erase(path_.end() - 1);
          }
          inferred_facts_.erase(rules_[i].first[k]);
          EraseSons(rules_[i].first[k]);
        }
      }
    } 
  }
  return;
}


bool BwdChaining::FindSolution(char goal, std::set<char>& visitedFacts, int depth) {
  bool flag = false;

  if (Inferred(goal)) {
    if (IsAnInitialFact(goal)) {
    output_file << "  " << ++step_ << ") ";
    PrintDepthMarkers(depth);
    output_file << "Goal " << goal << ". Fact (initial), as facts are ";
    PrintInferredFacts(output_file);
    output_file << ". Back, OK" << std::endl;
    } else {
      output_file << "  " << ++step_ << ") ";
      PrintDepthMarkers(depth);
      output_file << "Goal " << goal << ". Fact";
      output_file << "(earlier inferred), as facts ";
      PrintInferredFacts(output_file);
      output_file << ". Back, OK." << std::endl;
    }
    return true;
  }

  // If the goal has already been visited, return false to avoid infinite loops
  if (visitedFacts.find(goal) != visitedFacts.end()) {
    output_file << "  " << ++step_ << ") ";
    PrintDepthMarkers(depth);
    output_file << "Goal " << goal << ". Cycle. Back, FAIL. " << std::endl;
    return false;
  }

  // Mark the current fact as visited
  visitedFacts.insert(goal);
  
  // Find rules for the current goal
  for (int i = 0; i < rules_.size(); ++i) {
    bool flag_2 = true;
    if (goal == rules_[i].second) {
      flag = true;
      output_file << "  " << ++step_ << ") ";
      PrintDepthMarkers(depth);

      output_file << "Goal " << goal << ". Find R" << i + 1 << ":";
      PrintLHS(output_file, rules_[i].first);
      output_file << "->" << rules_[i].second << ". New Goals ";
      PrintLHS(output_file, rules_[i].first);
      output_file << std::endl;
  
      for (int j = 0; j < rules_[i].first.size(); ++j) {
        if (!FindSolution(rules_[i].first[j], visitedFacts, depth + 1)) {
          flag_2 = false;
          if (path_.size() > 1) {
            path_.erase(path_.end() - 1);
          }
          EraseSons(goal);        
          break;
        }
      }
      if (flag_2) {

        inferred_facts_.insert(goal);
        path_.push_back(i+1);
        output_file << "  " << ++step_ << ") ";
        PrintDepthMarkers(depth);
        output_file << "Goal " << goal << ". Fact";
        output_file << "(presently inferred). Facts ";
        PrintInferredFacts(output_file);
        output_file << ". Back, OK." << std::endl;

        return true;
      }
    }
  }

  // If no rule satisfied, return false
  if (!flag) {
    output_file << "  " << ++step_ << ") ";
    PrintDepthMarkers(depth);
    output_file << "Goal " << goal << ". No rules. Back, FAIL." << std::endl;
    visitedFacts.erase(goal);
    //inferred_facts_.erase(goal);


    return false;
  }
  // If no more rules satisfied, return false
  output_file << "  " << ++step_ << ") ";
  PrintDepthMarkers(depth);
  output_file << "Goal " << goal << ". No more rules. Back, FAIL." << std::endl;
  visitedFacts.erase(goal);
  //inferred_facts_.erase(goal);


  return false;
}


void BwdChaining::PrintResult(std::ostream& out, bool goal_reached) {
  out << "\nPART 3: Results" << std::endl;

  if (goal_reached == true) {
    out << "   1) Goal " << goal_ << " achieved." << std::endl;
    out << "   2) Path";
    for (auto& it : path_) {
      if (&it != &(*path_.rbegin())) {
        out << " R" << it << ",";
      } else {
        out << " R" << it << ".";
      }
    }
    out << std::endl;
  } else {
    out << "   No path" << std::endl;
  }
}