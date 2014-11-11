#include "update.h"

AssignUpdate::AssignUpdate():assignment_id(0), value(0){}

void AssignUpdate::setup(uint8_t assignment_id, float value){
	this->assignment_id = assignment_id;
	this->value = value;
}

Update::Update():update_list_size(0){}

// sends complete Update description.
int Update::getDescriptor(uint8_t *buffer){

	int i=0, buf_counter=0;
	uint8_t* float_ptr;

	for (; i < update_list_size; ++i){

		buffer[buf_counter++] = this->updates[i].assignment_id;

		float_ptr = (uint8_t*) &(this->updates[i].value);

		buffer[buf_counter++] = *float_ptr++;
		buffer[buf_counter++] = *float_ptr++;
		buffer[buf_counter++] = *float_ptr++;
		buffer[buf_counter++] = *float_ptr;

	}

	return buf_counter;
}

void Update::addAssignUpdate(uint8_t assignment_id, float value){
	if(update_list_size < NUMBER_OF_UPDATES){
		this->updates[update_list_size].setup(assignment_id, value);
		update_list_size++;
	}
}

void Update::reset(){
	update_list_size = 0;
}