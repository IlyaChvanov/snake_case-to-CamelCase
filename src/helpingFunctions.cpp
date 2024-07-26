#include "helpingFunctions.h"

std::string HelpingFunctions::GetTextFromFile(const std::string& file_path) {
  std::ifstream file(file_path);
  std::string to_return;
  std::getline(file, to_return, '\0');
  return to_return;
}

std::string HelpingFunctions::GetExtension(const std::string& file) {
  return file.substr(file.find_last_of('.') + 1);
}

bool HelpingFunctions::IsProgrammingFile(const std::string& file) {
  std::string fe = GetExtension(file);
  return (fe == "c" || fe == "cpp" || fe == "h" || fe == "hpp");
}

Texts HelpingFunctions::MakeTextsFromFiles(
    const std::vector<std::string>& files) {
  Texts texts;
  texts.reserve(files.size());
  for (auto& file : files) {
    texts.emplace_back(GetTextFromFile(file));
  }
  texts.shrink_to_fit();
  return texts;
}

void HelpingFunctions::GetFiles(std::string& root,
                                std::vector<std::string>& files) {
  try {
    for (auto& file : std::filesystem::directory_iterator(root)) {
      if (std::filesystem::is_directory(file) && file.path().string() != "cmake-build-debug") {
        std::string fp = file.path().string();
        GetFiles(fp, files);
      } else if (HelpingFunctions::IsProgrammingFile(file.path().string())) {
        files.emplace_back(file.path().string());
      }
    }
  } catch (const std::filesystem::__cxx11::filesystem_error&) {
    std::cout << "incorrect path of project, try again" << '\n';
    UI::AskDirectoryPath();
    root = UI::ReadPath();
    GetFiles(root, files);
  }
}
void HelpingFunctions::LogForFindings(const std::sregex_iterator& it, int pos_of_pushing) {
  std::cout << "string: " << it->str(0) << " name: " << it->str(pos_of_pushing) << std::endl;
}
std::vector<std::string> HelpingFunctions::SplitWordsSnakeCase
    (std::string_view str) {
  std::string word;
  std::vector<std::string> words;
  for (auto c : str) {
    if (c != '_') {
      word += c;
    } else {
      words.push_back(word);
      word.clear();
    }
  }
  if (!word.empty()) {
    words.push_back(word);
  }
  return words;
}
std::vector<std::string> HelpingFunctions::SplitWordsPascalOrCamel
    (std::string_view str) {
  std::string word;
  std::vector<std::string> words;
  for (auto it = str.begin(); it != str.end(); it++) {
    if (*it != tolower(*it) && it != str.begin()) {
      words.push_back(word);
      word.clear();
      word += static_cast<char>(tolower(*it));
      continue;
    }
    word += static_cast<char>(tolower(*it));
  }
  if (!word.empty()) {
    words.push_back(word);
  }
  LogForWordsSpliting(words, str);
  words.shrink_to_fit();
  return words;
}
void HelpingFunctions::LogForWordsSpliting(const std::vector<std::string>& words,
                                           const std::string_view str) {
  std::cout << "Original word is: " << str;
  for (const auto& word : words) {
    std::cout << word << ' ';
  }
}
std::string HelpingFunctions::MakeNewName(const std::vector<std::string>& splited, Style style) {
  std::string to_return;
  size_t size_of_words = 0;
  for (const auto& word : splited) {
    size_of_words += word.size() + 1;
  }
  to_return.reserve(size_of_words);

  if (style == snake_case) {
    for (const auto& word : splited) {
      for (auto c : word) {
        to_return += c;
      }
      to_return += '_';
    }
    to_return.erase(to_return.end() - 1);
  } else if (style == camelCase) {
    bool is_first_word = true;
    for (const auto& word : splited) {
      bool is_first_letter = true;
      for (auto c : word) {
        if (is_first_letter && !is_first_word) {
          to_return += static_cast<char>(toupper(c));
          is_first_letter = false;
        } else {
          to_return += c;
        }
      }
      is_first_word = false;
    }
  } else if (style == PascalCase) {
    for (const auto& word : splited) {
      bool is_first_letter = true;
      for (auto c : word) {
        if (is_first_letter) {
          to_return += static_cast<char>(toupper(c));
          is_first_letter = false;
        } else {
          to_return += c;
        }
      }
    }
  }
  return to_return;
}


