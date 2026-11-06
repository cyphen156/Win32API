#pragma once

template<typename TOwner>
class Controller
{
public:
	Controller(TOwner* ownerObject) : owner(ownerObject)
	{
	}

	virtual ~Controller()
	{
	}

	virtual void update()
	{
	}

protected:
	TOwner* const owner;
};