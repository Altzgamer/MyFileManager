#include "MyFileManager.h"
#include <string>
#include <filesystem>
#include <stdexcept>
#include <cstring>
#include <algorithm> 

using std::string;

#ifdef _WIN32
    #include <direct.h>
    const char PATH_SEPARATOR[] = "\\";
    #define getcwd _getcwd
    #define strdup _strdup

    void unified_strcpy_s(char* dst, size_t total_len, const char* source) {
        std::copy(source, source + std::strlen(source) + 1, dst);
    }
    void unified_strcat_s(char* dst, size_t total_len, const char* source) {
        std::copy(source, source + std::strlen(source) + 1, dst + std::strlen(dst)); 
    }
#else
    #include <unistd.h>
    const char PATH_SEPARATOR[] = "/";
    void unified_strcpy_s(char* dst, size_t total_len, const char* source) {
        std::copy(source, source + std::strlen(source) + 1, dst); 
    }

    void unified_strcat_s(char* dst, size_t total_len, const char* source) {
        std::copy(source, source + std::strlen(source) + 1, dst + std::strlen(dst));
    }
#endif

bool is_same_CD(const char* path1, const char* path2) {
    if ((strlen(path1) > 2 && path1[1] == ':' && path1[2] == '\\' && (std::isalpha(path1[0]))) &&
        (strlen(path2) > 2 && path2[1] == ':' && path2[2] == '\\' && (std::isalpha(path2[0])))
        && (toupper(path1[0]) == toupper(path2[0]))) return true;
    return false;
}

bool is_absolute(const char* path) {
    if (path == nullptr || std::strlen(path) == 0) {
        return false;
    }
    #ifdef _WIN32
        return (std::strlen(path) >= 2 && path[1] == ':') || (path[0] == '\\');
    #else
        return path[0] == '/';
    #endif
}

bool is_path_normal(const char* path) {
    const char invalid_chars[] = { '#','%', '&', '{', '}', '<', '>', '*', '?', ' ', '$', '!', '"', '@', '+', '`', '|', '=' };

    if (std::find_first_of(path, path + strlen(path), invalid_chars, invalid_chars + sizeof(invalid_chars)) != path + strlen(path)) {
        return false;
    }

    if (std::any_of(path, path + strlen(path), [](char c) {
        return (c == '/' || c == '\\') && c != PATH_SEPARATOR[0];
        })) {
        return false;
    }

    return true;
}

char* join(const char* base, const char* relative) {
    if (base == nullptr || relative == nullptr) throw std::invalid_argument("Null argument");
    if (!is_path_normal(base) || !is_path_normal(relative)) throw std::invalid_argument("Wrong separators or symbols");
    if (is_absolute(relative)) throw std::invalid_argument("Second path is not relative");

    size_t base_len = std::strlen(base);
    size_t relative_len = std::strlen(relative);
    size_t total_len = base_len + relative_len + 2; 

    char* result = new char[total_len];
    
    unified_strcpy_s(result, total_len, base);
    if(result[strlen(result) - 1] != PATH_SEPARATOR[0]) unified_strcat_s(result, total_len, PATH_SEPARATOR);
    unified_strcat_s(result, total_len, relative);

    return result;
}

char* absolute(const char* path) {
    if (path == nullptr) throw std::invalid_argument("Null Argument");
    if (!is_path_normal(path)) throw std::invalid_argument("Wrong separators or symbols");

    char* cwd = getcwd(NULL, 0);
    if (cwd == nullptr) throw std::runtime_error("Cwd error");

    if (is_absolute(path)) {
        if (std::strncmp(path, cwd, std::strlen(cwd)) == 0) free(cwd);
        return strdup(path);
    }
    else {
        char* result = join(cwd, path);
        free(cwd);
        return result;
    }
}

char* relative(const char* path) {
    if (path == nullptr) throw std::invalid_argument("Null argument");

    if (!is_absolute(path)) return strdup(path);

    char* cwd = getcwd(NULL, 0);
    if (cwd == nullptr) throw std::runtime_error("Cwd error");

    #ifdef _WIN32
        if (!is_same_CD(cwd, path)) {free(cwd); throw std::invalid_argument("Different CDs");}
    #endif

    size_t cwd_len = std::strlen(cwd);
    size_t path_len = std::strlen(path);

    if (path_len < cwd_len || std::strncmp(path, cwd, cwd_len) != 0) {

        size_t up_steps = std::count(cwd, cwd + cwd_len, PATH_SEPARATOR[0]);

        const char* relative_part = path;

        if (strlen(path) > 3 && path[1] == ':' && path[2] == '\\' && ((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z'))) {
            relative_part = path + 3;
        }
        else if (path[0] == '/' || path[0] == '\\') relative_part = path + 1;
     
        size_t relative_part_len = std::strlen(relative_part); 
        size_t total_len = up_steps * (std::strlen("..") + 1) + relative_part_len + 1; 

        char* result = new char[total_len];

        char* ptr = result;
        for (size_t i = 0; i < up_steps; ++i) {
            unified_strcpy_s(ptr, total_len - (ptr - result), "..");
            ptr += std::strlen("..");
            *ptr++ = PATH_SEPARATOR[0]; 
        }
        unified_strcpy_s(ptr, total_len - (ptr - result), relative_part);

        free(cwd);
        return result;
    }

    else {
        size_t relative_path_len = path_len - cwd_len;
        if (relative_path_len == 0) return strdup(".");

        size_t total_len = relative_path_len + 1; 

        char* result = new char[total_len];

        unified_strcpy_s(result, total_len, path + cwd_len);
        free(cwd);
        return result;
    }
}

char* relativize(const char* base_path, const char* target_path) {
    if (target_path == nullptr || base_path == nullptr) throw std::invalid_argument("Null argument");
    #ifdef _WIN32
        if (!is_same_CD(base_path, target_path)) throw std::invalid_argument("Different CDs");
    #endif
    if (!is_absolute(target_path) || (!is_absolute(base_path))) throw std::invalid_argument("Not absolute argument");


    size_t base_len = std::strlen(base_path);
    size_t path_len = std::strlen(target_path);

    if (path_len < base_len || std::strncmp(target_path, base_path, base_len) != 0) {

        size_t up_steps = std::count(base_path, base_path + base_len, PATH_SEPARATOR[0]);

        const char* relative_part = target_path;

        if (target_path[1] == ':' && target_path[2] == '\\' && ((target_path[0] >= 'A' && target_path[0] <= 'Z') || (target_path[0] >= 'a' && target_path[0] <= 'z'))) {
            relative_part = target_path + 3;
        }
        else if (target_path[0] == '/' || target_path[0] == '\\') relative_part = target_path + 1;

        size_t relative_part_len = std::strlen(relative_part);
        size_t total_len = up_steps * (std::strlen("..") + 1) + relative_part_len + 1;

        char* result = new char[total_len];

        char* ptr = result;
        for (size_t i = 0; i < up_steps; ++i) {
            unified_strcpy_s(ptr, total_len - (ptr - result), "..");
            ptr += std::strlen("..");
            *ptr++ = PATH_SEPARATOR[0];
        }

        unified_strcpy_s(ptr, total_len - (ptr - result), relative_part);

        return result;
    }

    else {
        size_t relative_path_len = path_len - base_len;
        if (relative_path_len == 0) return strdup(".");

        size_t total_len = relative_path_len + 1;

        char* result = new char[total_len];

        unified_strcpy_s(result, total_len, target_path + base_len);
        return result;
    }
}

bool is_absolute(const string& path) {
    if (path.empty()) return false;

    #ifdef _WIN32
        return (path.size() >= 2 && path[1] == ':' && path[2] == '\\') || (path[0] == '\\');
    #else
        return path[0] == '/';
    #endif
}

bool is_path_normal(const string& path) {
    const char invalid_chars[] = { '#','%', '&', '{', '}', '<', '>', '*', '?', ' ', '$', '!', '"', '@', '+', '`', '|', '=' };

    if (std::find_first_of(path.begin(), path.end(), std::begin(invalid_chars), std::end(invalid_chars)) != path.end()) {
        return false;
    }

    if (std::any_of(path.begin(), path.end(), [](char ch) {
        return (ch == '/' || ch == '\\') && ch != PATH_SEPARATOR[0];
        })) {
        return false;
    }

    return true;
}

string join(const string& base, const string& relative) {
    if (base.empty() || relative.empty()) throw std::invalid_argument("Null Argument");
    if (!is_path_normal(base) || !is_path_normal(relative)) throw std::invalid_argument("Wrong separators or symbols");
    if (is_absolute(relative)) throw std::invalid_argument("Second path is not relative.");

    string result = base;

    if (!result.empty() && result.back() != PATH_SEPARATOR[0]) {
        result += PATH_SEPARATOR[0];
    }

    result += relative;

    return result;
}

string absolute(const string& path) {
    if (path.empty()) throw std::invalid_argument("Null Argument");

    if (!is_path_normal(path)) throw std::invalid_argument("Wrong separators or symbols");

    char* cwd = getcwd(NULL, 0);
    if (cwd == nullptr) throw std::runtime_error("Cwd error");

    string current_directory(cwd);
    free(cwd);

    if (is_absolute(path)) return path; // ?

    return join(current_directory, path);
}

string relative(const string& path) {
    if (path.empty()) throw std::invalid_argument("Null argument");
    if (!is_absolute(path)) return path;

    char* cwd = getcwd(NULL, 0);
    if (cwd == nullptr) throw std::runtime_error("Cwd error");

    #ifdef _WIN32
        if (!is_same_CD(cwd, path.c_str())) { free(cwd); throw std::invalid_argument("Different CDs");}
    #endif 

    string current_directory(cwd);
    free(cwd);


    size_t cwd_len = current_directory.length();
    size_t path_len = path.length();

    if (path_len < cwd_len || path.substr(0, cwd_len) != current_directory) {
        size_t up_steps = std::count(current_directory.begin(), current_directory.end(), PATH_SEPARATOR[0]);

        string relative_part = path;
        if (path_len > 2 && path[1] == ':' && path[2] == '\\' && ((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z'))) {
            relative_part = path.substr(3);
        }
        else if (path[0] == '/' || path[0] == '\\') relative_part = path.substr(1);

        string result;
        for (size_t i = 0; i < up_steps; ++i) {
            result += "..";
            result += PATH_SEPARATOR[0];
        }
        result += relative_part;
        return result;
    }
    else {
        string relative_path = path.substr(cwd_len);
        if (relative_path.empty()) {
            return "."; 
        }

        return relative_path[0] == PATH_SEPARATOR[0] ? relative_path.substr(1) : relative_path;
    }
}

string relativize(const string& base_path, const string& target_path) {
    if (base_path.empty() || target_path.empty()) throw std::invalid_argument("Null argument");

    if (!is_absolute(base_path) || !is_absolute(target_path)) throw std::invalid_argument("Not absolute argument");

    #ifdef _WIN32
        if (!is_same_CD(base_path.c_str(), target_path.c_str())) throw std::invalid_argument("Different CDs"); 
    #endif 

    size_t base_len = base_path.length();
    size_t path_len = target_path.length();

    if (path_len < base_len || target_path.substr(0, base_len) != base_path) {
        size_t up_steps = std::count(base_path.begin(), base_path.end(), PATH_SEPARATOR[0]);;

        string relative_part = target_path;
        if (path_len > 2 && target_path[1] == ':' && target_path[2] == '\\' && ((target_path[0] >= 'A' && target_path[0] <= 'Z') || (target_path[0] >= 'a' && target_path[0] <= 'z'))) {
            relative_part = target_path.substr(3);
        }
        else if (target_path[0] == '/' || target_path[0] == '\\') relative_part = target_path.substr(1);

        string result;
        for (size_t i = 0; i < up_steps; ++i) {
            result += "..";
            result += PATH_SEPARATOR;
        }
        result += relative_part;
        return result;
    }
    else {
        string relative_path = target_path.substr(base_len);
        if (relative_path.empty()) return ".";

        return relative_path[0] == PATH_SEPARATOR[0] ? relative_path.substr(1) : relative_path;
    }
}
 