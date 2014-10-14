#ifndef SWITCH_H
#define SWITCH_H

#include "utils.h"
#include "actuator.h"

#define TOGGLE_DOWN		0
#define TOGGLE_MID		1
#define TOGGLE_HIGH		2

class Button: public Actuator{ // A switch witch does not save state, it can emulate state saver switch
public:
	int 			minimum = 0;
	int 			maximum = 1;
	bool			default_state; // which value the button holds when is not pressed.



	Button(char* name, uint8_t id, bool default_state): Actuator(name, id, 1, 2, 1, VISUAL_NONE), default_state(default_state){

		Mode *mode = supports("toggle"); // saves state
		// mode->expects(PROPERTY_INTEGER, false);
		// mode->expects(PROPERTY_LOGARITHM, false);
		mode->expects(PROPERTY_TOGGLE, true);
		// mode->expects(PROPERTY_TRIGGER, false);
		// mode->expects(PROPERTY_SCALE_POINTS, false);
		// mode->expects(PROPERTY_ENUMERATION, false);
		// mode->expects(PROPERTY_TAP_TEMPO, false);
		// mode->expects(PROPERTY_BYPASS, false);

		Mode *mode2 = supports("trigger"); //does not save state
		// mode2->expects(PROPERTY_INTEGER, false);
		// mode2->expects(PROPERTY_LOGARITHM, false);
		mode2->expects(PROPERTY_TOGGLE, true);
		mode2->expects(PROPERTY_TRIGGER, true);
		// mode2->expects(PROPERTY_SCALE_POINTS, false);
		// mode2->expects(PROPERTY_ENUMERATION, false);
		// mode2->expects(PROPERTY_TAP_TEMPO, false);
		// mode2->expects(PROPERTY_BYPASS, false);

		addStep(2);

	}

 	void calculateValue(){

 		static bool trigger = false;
 		static uint8_t toggle_state = false;
 		static uint8_t last_toggle_state = false;

		bool sensor = (bool) this->getValue();
		
		float scaleMin, scaleMax;

		scaleMin = this->addrs[0]->minimum.f;
	    scaleMax = this->addrs[0]->maximum.f;
	    
	    if ((this->addrs[0]->port_properties & modes[0]->relevant_properties) == modes[0]->property_values) { // toggle
	    	switch(toggle_state){
	    		case TOGGLE_DOWN:
	    			if(!(sensor && default_state)){
	    				last_toggle_state = toggle_state;
	    				toggle_state = TOGGLE_MID;
	    			}
	    		break;

	    		case TOGGLE_MID:
	    			if(sensor && default_state){
	    				if(last_toggle_state == TOGGLE_HIGH){
		    				last_toggle_state = toggle_state;
		    				toggle_state = TOGGLE_DOWN;
		    				this->value = scaleMin;
	    				}
    					else if(last_toggle_state == TOGGLE_DOWN){
		    				last_toggle_state = toggle_state;
		    				toggle_state = TOGGLE_HIGH;
		    				this->value = scaleMax;
    					}
	    			}
	    		break;

	    		case TOGGLE_HIGH:
	    			if(!(sensor && default_state)){
	    				last_toggle_state = toggle_state;
	    				toggle_state = TOGGLE_MID;
	    			}
	    		break;
	    	}
	    }
	    //TODO implementar e testar modo trigger
		// if ((this->addressing->port_properties & modes[1]->relevant_properties) == modes[1]->property_values) { // trigger

	 //    	if(trigger && (default_state && sensor)){
	 //    		trigger = false;
	 //    		this->value = scaleMin;
	 //    	}
	 //    	else if(!trigger && !(default_state && sensor)){
	 //    		trigger = true;
	 //    		this->value = scaleMax;
	 //    	}
	 //    	else{
	 //    		this->value = scaleMin;
	 //    	}
	 //    }
 	}

 	void postMessageChanges(){
		// if ((this->addressing->port_properties & modes[1]->relevant_properties) == modes[1]->property_values) { // trigger

	 //    	this->value = 0;
	 //    }
 	}

	void getUpdates(Update* update){
		update->updates->setup(this->addrs[0]->id, this->value);
	}

	virtual float getValue()=0;

};

#endif