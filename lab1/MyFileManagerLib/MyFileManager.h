#pragma once
#ifndef MYLIBRARY_H
#define MYLIBRARY_H

#include <string>
/**
 * @brief Сравнивает два пути и проверяет, указывают ли они на одну и ту же директорию
 *
 * @param path1 Путь к первой директории
 * @param path2 Путь ко второй директории
 * @return true Пути указывают на одну и ту же директорию
 * @return false Пути не совпадают
 */
bool is_same_CD(const char* path1, const char* path2);

/**
 * @brief Проверяет, является ли путь абсолютным
 *
 * @param path Путь для проверки
 * @return true Путь абсолютный
 * @return false Путь не абсолютный
 */
bool is_absolute(const char* path);

/**
 * @brief Проверяет, является ли путь нормальным
 *
 * @param path Путь для проверки
 * @return true Путь нормальный
 * @return false Путь содержит недопустимые символы или неверные разделители
 */
bool is_path_normal(const char* path);

/**
 * @brief Объединяет базовый путь с относительным
 *
 * @param base Базовый путь
 * @param relative Относительный путь
 * @return Указатель на новый объединенный путь
 * @throws std::invalid_argument Один из аргументов равен nullptr или пути некорректные
 */
char* join(const char* base, const char* relative);

/**
 * @brief Преобразует относительный путь в абсолютный
 *
 * @param path Относительный путь
 * @return Указатель на новый абсолютный путь
 * @throws std::invalid_argument Путь равен nullptr или некорректный
 * @throws std::runtime_error Возникает ошибка при получении текущего рабочего каталога
 */
char* absolute(const char* path);

/**
 * @brief Преобразует абсолютный путь в относительный
 *
 * @param path Абсолютный путь
 * @return Указатель на новый относительный путь
 * @throws std::invalid_argument Путь равен nullptr или некорректный
 * @throws std::runtime_error Возникает ошибка при получении текущего рабочего каталога
 */
char* relative(const char* path);

/**
 * @brief Преобразует целевой путь в относительный путь относительно базового
 *
 * @param base_path Базовый путь
 * @param target_path Целевой путь
 * @return Указатель на новый относительный путь
 * @throws std::invalid_argument Один из аргументов равен nullptr, пути некорректные или находятся на разных дисках
 */
char* relativize(const char* base_path, const char* target_path);

/**
 * @brief Проверяет, является ли путь абсолютным (для std::string)
 *
 * @param path Путь для проверки
 * @return true Путь абсолютный
 * @return false Путь не абсолютный
 */
bool is_absolute(const std::string& path);

/**
 * @brief Проверяет, является ли путь нормальным (для std::string)
 *
 * @param path Путь для проверки
 * @return true Путь нормальный
 * @return false Путь содержит недопустимые символы или неверные разделители
 */
bool is_path_normal(const std::string& path);

/**
 * @brief Объединяет базовый путь с относительным (для std::string)
 *
 * @param base Базовый путь
 * @param relative Относительный путь
 * @return Новый объединенный путь
 * @throws std::invalid_argument Один из аргументов пуст или пути некорректные
 */
std::string join(const std::string& base, const std::string& relative);

/**
 * @brief Преобразует относительный путь в абсолютный (для std::string)
 *
 * @param path Относительный путь
 * @return Новый абсолютный путь
 * @throws std::invalid_argument Путь пуст или некорректный
 */
std::string absolute(const std::string& path);

/**
 * @brief Преобразует абсолютный путь в относительный (для std::string)
 *
 * @param path Абсолютный путь
 * @return Новый относительный путь
 * @throws std::invalid_argument Путь пуст
 */
std::string relative(const std::string& path);

/**
 * @brief Преобразует целевой путь в относительный путь относительно базового (для std::string)
 *
 * @param base_path Базовый путь
 * @param target_path Целевой путь
 * @return Новый относительный путь
 * @throws std::invalid_argument Один из аргументов пуст или пути некорректные
 */
std::string relativize(const std::string& base_path, const std::string& target_path);


#endif