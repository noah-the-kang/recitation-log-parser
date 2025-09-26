#include "solution.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::vector<Log> ReadLogs(const std::string& filename) {
  std::vector<Log> logs;
  std::ifstream ifs(filename);

  if (!ifs.is_open()) {
    std::cerr << "Error opening file!" << std::endl;
  }

  std::string header_line;
  if (std::getline(ifs, header_line)) {
    std::string date, time, message;
    int level = 0;
    while (ifs >> date >> time >> level >> message) {  // Read line by line
      if (ifs.fail()) break;
      std::string timestamp = date + " " + time;
      Log log;
      log.timestamp = timestamp;
      log.level = level;
      log.message = message;
      logs.push_back(log);
    }
  }
  ifs.close();  // Close the file
  return logs;
}

std::vector<Log> Filter(const std::vector<Log>& logs, int level) {
  std::vector<Log> filtered_logs;
  for (const auto& log : logs) {
    if (log.level == level) filtered_logs.push_back(log);
  }
  return filtered_logs;
}

std::vector<Log> Filter(const std::vector<Log>& logs,
                        DateTime start,
                        DateTime end) {
  std::vector<Log> filtered_logs;
  for (const auto& log : logs) {
    DateTime parsed_log = ParseTimestamp(log.timestamp);
    if (parsed_log <= end && parsed_log >= start) {
      filtered_logs.push_back(log);
    }
  }
  return filtered_logs;
}

void WriteMetrics(const std::vector<Log>& logs, const std::string& filename) {
  std::ofstream ofs(filename);

  if (!ofs.is_open()) {
    std::cerr << "Error: Unable to create file " << filename << std::endl;

    return;
  }

  unsigned long total_entries = logs.size();
  int count_404 = 0;
  int count_500 = 0;
  int count_other = 0;
  const int four_hundred = 404;
  const int five_hundred = 500;

  for (const Log& log : logs) {
    if (log.level == four_hundred) {
      count_404++;
    } else if (log.level == five_hundred) {
      count_500++;
    } else {
      count_other++;
    }
  }

  ofs << "Metric,Count" << std::endl;
  ofs << "Total Entries," << total_entries << std::endl;
  ofs << "404," << count_404 << std::endl;
  ofs << "500," << count_500 << std::endl;
  ofs << "Other," << count_other << std::endl;

  ofs.close();
}
