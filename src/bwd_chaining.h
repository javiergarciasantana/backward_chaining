#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>

#ifndef BWD_CHAINING_H
#define BWD_CHAINING_H

class BwdChaining {
 public:
  BwdChaining(std::ifstream& input, std::string text_file_name) {
    step_ = 0;
    input.open(text_file_name, std::ios_base::in);
    
    if (input.is_open()) {
      std::string line, aux;
      while(getline(input, line)) {

        if (line == "1) Rules") {
          while (getline(input, line) && line != "") {
            for (int i = line.length() - 1; i >= 2; i = i - 2) {
              aux += line[i];
            }
            rules_.push_back(std::make_pair(std::string(aux.rbegin(), aux.rend()), line[0]));
            aux.clear();
          }
        }
        if (line == "2) Facts") {
          while (getline(input, line) && line != "") {
            for (int i = 0; i < line.length(); ++i) {
              if (line[i] != ' ') {
                initial_facts_.push_back(line[i]);
              }
            }
          }
        }
        if (line == "3) Goal") {
          while (getline(input, line) && line != "") {
            goal_ = line[0];
          }
        }
      }
      
      if (IsAnInitialFact(goal_)) {
        if (output_file.is_open()) {
          DataPrint(output_file);
          output_file << "\nPART 3. Results" << std::endl 
          << "  Goal " << goal_ << " among facts. Empty path.";
        }
      } else {
        // Initialize a set to keep track of visited facts during the backward chaining process
        std::set<char> visitedFacts;
        for (int i = 0; i < initial_facts_.size(); ++i) {
          inferred_facts_.insert(initial_facts_[i]);
        }
        // Call the recursive function
        if (output_file.is_open()) {
          DataPrint(output_file);
          output_file << "\nPART 2. Trace" << std::endl;
          PrintResult(output_file, FindSolution(goal_, visitedFacts));
        }
      }
    }
  }

  void DataPrint(std::ostream& out);
  void PrintLHS(std::ostream& out, const std::string kRuleLhs);
  void PrintInitialFacts(std::ostream& out);
  void PrintInferredFacts(std::ostream& out);
  void PrintResult(std::ostream& out, bool goal_reached);
  void PrintDepthMarkers(int depth);
  void EraseSons(char goal);

  bool IsAnInitialFact(const char kGoal);
  bool Inferred(const char kGoal);
  
  bool FindSolution(char goal, std::set<char>& visitedFacts, int depth = 0);

 private:
  std::ofstream output_file{"output.txt"};
  std::vector<std::pair<std::string, char> > rules_;
  std::set<char> inferred_facts_;
  std::vector<char> initial_facts_;
  std::vector<int> path_;
  char goal_;
  int step_;
};

#endif