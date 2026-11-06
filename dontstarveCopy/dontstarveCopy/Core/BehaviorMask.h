#pragma once
#include <initializer_list>
#include <cassert>

template<typename TState, typename TBehavior>
class BehaviorMask
{
public:
	BehaviorMask(initializer_list<initializer_list<bool>> rows)
	{
		assert(rows.size() == static_cast<int>(TState::END));

		int stateIndex = 0;
		for (const initializer_list<bool>& row : rows)
		{
			assert(row.size() == static_cast<int>(TBehavior::END));

			int behaviorIndex = 0;
			for (bool isAllowed : row)
			{
				allowed[stateIndex][behaviorIndex] = isAllowed;
				++behaviorIndex;
			}
			++stateIndex;
		}
	}

	bool Allows(TState state, TBehavior behavior) const
	{
		return allowed[static_cast<int>(state)][static_cast<int>(behavior)];
	}

private:
	bool allowed[static_cast<int>(TState::END)][static_cast<int>(TBehavior::END)] = {};
};