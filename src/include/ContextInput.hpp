#ifndef _CONTEXTINPUT_HPP_
#define _CONTEXTINPUT_HPP_

namespace Cycleshooter {
class ContextInput{
	public:
		enum InputMode {
			CONTEXT_LAB
			CONTEXT_DEBUG
			CONTEXT_NONE
		};

	private:
		InputMode inputMode;
		KeyboardController *keyboardController;
		BicycleController  *bicycleController;

	public:
		ContextInput();
		virtual ~ContextInput();

		void setupDebugInput();
		void setupLabInput();
};	
}

#endif
