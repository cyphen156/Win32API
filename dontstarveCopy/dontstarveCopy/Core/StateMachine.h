#pragma once

template<typename TState>
class StateMachine
{
public:
	StateMachine(TState initialState) : state(initialState)
	{
	}

	TState GetState() { return state; }

	bool TryChangeState(TState next)
	{
		if (state == next)
		{
			return false;
		}

		state = next;
		return true;
	}

	bool ForceChangeState(TState next)
	{
		state = next;
		return true;
	}

private:
	TState state;
};