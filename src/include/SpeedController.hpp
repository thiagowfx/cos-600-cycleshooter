#ifndef _SPEEDCONTROLLER_HPP_
#define _SPEEDCONTROLLER_HPP_

namespace Cycleshooter {
class SpeedController{
	protected:
		double speed;

	public:
		SpeedController();
		~SpeedController();
		
		void increaseSpeed();
		void decreaseSpeed();
};

class KeyboardController : public SpeedController {
	public:
		KeyboardController();
		~KeyboardController();
		
		void setSpeed(char velocity);

};

class BicycleController : public SpeedController{
	public:
		BicycleController();
		~BicycleController();

		void setSpeed(double velocity);
};	
}

#endif