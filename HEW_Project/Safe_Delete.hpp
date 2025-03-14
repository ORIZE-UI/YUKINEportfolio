#ifndef __SAFE_DELETE_HPP__
#define __SAFE_DELETE_HPP__

#include <iostream>

/**
 * @brief メモリの解放
 * @tparam T  解放する型 (省略可)
 * @param p 解放するポインタ
 */
template <typename T>
inline void SafeDelete(T*& p)
{
	if (p != nullptr)
	{
		delete p;
		p = nullptr;
	}
}

/**
 * @brief  配列のメモリ解放
 * @tparam T 解放する型 (省略可)
 * @param p 解放するポインタ配列
 */
template <typename T>
inline void SafeDelete_Array(T*& p)
{
	if (p != nullptr)
	{
		delete[] p;
		p = nullptr;
	}
}

#endif // !__SAFE_DELETE_HPP__
