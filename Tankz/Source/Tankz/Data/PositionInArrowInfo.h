#pragma once

#include <Math/Vector.h>

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
	void ToSide(Side newSide);

	FVector getArrowOffset() { return FVector(currentDeltaX, currentDeltaY, 0); }
	FRotator getRotation();
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

inline PositionInArrowInfo::PositionInArrowInfo():
	currentRotation(Facing::Front),
	currentSide(Side::Right)
{
	//TODO: This values should be calculated from TankTypeData
	constexpr float arrowTailXSize = 96.f;
	constexpr float arrowTailYSize = 6.f;
	constexpr float tankXSize = 50.f;
	constexpr float tankYSize = 28.f;

	parallelBounds.minDeltaX = tankXSize/2.f;
	parallelBounds.maxDeltaX = arrowTailXSize - tankXSize/2.f;
	parallelBounds.deltaY = arrowTailYSize + tankYSize/2.f;

	perpendicularBounds.minDeltaX = tankYSize/2.f;
	perpendicularBounds.maxDeltaX = arrowTailXSize - tankYSize/2.f;
	perpendicularBounds.deltaY = arrowTailYSize + tankXSize/2.f;

	currentDeltaX=parallelBounds.maxDeltaX;
	recalculateDeltaY();
}

inline void PositionInArrowInfo::ChangeDeltaX(float value)
{
	currentDeltaX += value;
	clampDeltaX();
}

inline void PositionInArrowInfo::ToSide(Side newSide)
{
	if(currentSide == newSide) return;
	currentDeltaY *= -1;
	currentSide = newSide;
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

inline FRotator PositionInArrowInfo::getRotation()
{
	switch(currentRotation) {
		case Facing::Front:
			return FRotator(0.f, 0.f, 0.f);
		case Facing::Right:
			return FRotator(0.f, 90.f, 0.f);
		case Facing::Back:
			return FRotator(0.f, 180.f, 0.f);
		case Facing::Left:
			return FRotator(0.f, 270.f, 0.f);
	}
	return FRotator(0.f, 0.f, 0.f); //Make the compiler happy
}
