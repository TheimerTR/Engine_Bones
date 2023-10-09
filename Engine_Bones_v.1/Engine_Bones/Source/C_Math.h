#pragma once

#ifndef _C_MATH_H_
#define _C_MATH_H_

#include<stdlib.h>
#include<time.h>
#include<vector>
#include<string>

namespace C_Math
{
	void VectorPushBack(std::vector<float>& vector, float value) {
		if (vector.size() == vector.capacity())
		{
			//If the Vectir is full capacity, deletes the most old element of the list, and move all the rest elementes one pos back.

			for (size_t i = 0; i < vector.size(); i++)
			{
				if (i + 1 < vector.size())
				{
					vector[i] = vector[i + 1];
				}
			}

			//Put the last element at the back
			vector[vector.capacity() - 1] = value;
		}
		else
		{
			//If vec is not full so put new elements
			vector.push_back(value);
		}
	}

	template <class T>
	void ClearVec(std::vector<T>& vector)
	{
		if (vector.size() > 0)
		{
			for (int i = 0; i < vector.size() - 1; i++)
			{
				vector.pop_back();
			}
		}

		vector.clear();
	}
}

#endif // _C_MATH_H_
