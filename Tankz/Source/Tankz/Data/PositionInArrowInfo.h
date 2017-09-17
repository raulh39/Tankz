#pragma once

enum class Side
{
	Left,
	Right
};

enum class Facing
{
	Front,
	Right,
	Back,
	Left
};

inline bool isParallel(Facing facing)
{
	return facing == Facing::Back || facing == Facing::Front;
}

class PositionInArrowInfo
{
public:
	PositionInArrowInfo();
	PositionInArrowInfo(const PositionInArrowInfo&) = default;

	void ChangeRotation(bool increment);
	void ChangeDeltaX(float value);
	void FlipSides();

	float getDeltaX() const { return currentDeltaX; }
	float getDeltaY() const { return currentDeltaY; }

private:
	struct BoundsInfo {
		float maxDeltaX; //Max offset of tank along arrow's tail
		float minDeltaX; //Min offset of tank along arrow's tail
		float deltaY;    //Desplacement of tank in Y to be touching arrow
	};
	BoundsInfo parallelBounds;
	BoundsInfo perpendicularBounds;
	
	Facing currentRotation;
	Side currentSide;
	float currentDeltaX;
	float currentDeltaY;

	float getMaxDeltaX() {
		if(isParallel(currentRotation)) return parallelBounds.maxDeltaX;
		return perpendicularBounds.maxDeltaX;
	}
	float getMinDeltaX() {
		if(isParallel(currentRotation)) return parallelBounds.minDeltaX;
		return perpendicularBounds.minDeltaX;
	}
	void recalculateDeltaY() {
		if(isParallel(currentRotation)) currentDeltaY = parallelBounds.deltaY;
		else currentDeltaY = perpendicularBounds.deltaY;
		if(currentSide == Side::Left) currentDeltaY *= -1;
	}
	void clampDeltaX() {
		if(currentDeltaX > getMaxDeltaX()) currentDeltaX=getMaxDeltaX();
		if(currentDeltaX < getMinDeltaX()) currentDeltaX=getMinDeltaX();
	}
};

inline void PositionInArrowInfo::ChangeDeltaX(float value)
{
	currentDeltaX += value;
	clampDeltaX();
}

inline void PositionInArrowInfo::FlipSides()
{
	currentDeltaY *= -1;
	if(currentSide == Side::Left) currentSide = Side::Right;
	else currentSide = Side::Left;
}

inline void PositionInArrowInfo::ChangeRotation(bool increment)
{
	switch(currentRotation)
	{
	case Facing::Front:
		if(increment) currentRotation = Facing::Right;
		else currentRotation = Facing::Left; 
		break;
	case Facing::Right:
		if(increment) currentRotation = Facing::Back;
		else currentRotation = Facing::Front;
		break;
	case Facing::Back:
		if(increment) currentRotation = Facing::Left;
		else currentRotation = Facing::Right;
		break;
	case Facing::Left:
		if(increment) currentRotation = Facing::Front;
		else currentRotation = Facing::Back;
		break;
	}
	clampDeltaX();
	recalculateDeltaY();
}
