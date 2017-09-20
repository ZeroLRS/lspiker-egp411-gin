#pragma once

#include <string>
#include <wtypes.h>
#include "Trackable.h"

class Player:public Trackable
{
public:
	Player(const std::string& name, UINT playerNum );
	virtual ~Player(){};

	inline const std::string& getName() const { return mName; };
	inline UINT getPlayerNum() const { return mPlayerNum; };
	inline UINT getTotalScore() const { return mTotalScore; };
	inline void setTotalScore(UINT score) { mTotalScore = score; };

	virtual void update() = 0;
	virtual void setActive( bool isActive ) = 0;
	virtual bool isActive() const = 0;
protected:
	std::string mName;
	UINT mPlayerNum;
	UINT mTotalScore;
};

