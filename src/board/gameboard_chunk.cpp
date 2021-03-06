#include "../includes.h"

GameboardChunk::GameboardChunk(Hexagon* base_hex) {
	this->base_hex = base_hex;

	this->board_terrain_data = NULL;
	this->board_select_data = NULL;
	//this->board_object_data = NULL;

	this->regenerate_terrain = true;
	this->regenerate_select = true;
	//this->regenerate_object = true;
}


GameboardChunk::~GameboardChunk() {
	std::cout << "deleting: " << this << std::endl;

	if(this->board_terrain_data) {
		delete this->board_terrain_data;
	}

	if(this->board_select_data) {
		delete this->board_select_data;
	}
}


void GameboardChunk::clear_terrain() {
	if(this->board_terrain_data) {
		delete this->board_terrain_data;
	}

	this->board_terrain_data = new UniqueDataVector< GLdouble >();
	this->regenerate_terrain = true;
}


void GameboardChunk::clear_select() {
	if(this->board_select_data) {
		delete this->board_select_data;
	}

	this->board_select_data = new UniqueDataVector< GLdouble >();
	this->regenerate_select = true;
}

/*void GameboardChunk::clear_object() {
	if(this->board_object_data) {
		delete this->board_object_data;
	}

	this->board_object_data = new UniqueDataVector< GLdouble >();
	this->regenerate_object = true;
}*/

void GameboardChunk::generate_render_data(Hexagon* curr_hex, GLdouble x, GLdouble y) {
	// TODO: I don't like this logic being here.. this should be moved out...
	if(this->regenerate_terrain) {
		curr_hex->generate_vertex_data(x, y, this->board_terrain_data);
	}

	if(this->regenerate_select) {
		curr_hex->generate_select_data(x, y, this->board_select_data);
	}

	/*if(this->regenerate_object) {
		curr_hex->generate_object_data(x, y, this->board_object_data);
	}*/

	curr_hex->parent_chunk = this;
}


void GameboardChunk::generate_chunk_data() {
	GLdouble base_x = 0;
	GLdouble base_y = 0;

	//std::cout << "Gameboard::generating: " << this->base_hex << std::endl;

	GLdouble x = base_x;
	GLdouble y = base_y;

	const char* dir_ary[] = {"SE", "NE"};

	Hexagon* curr_hex = this->base_hex;

	for(int i = 0; i < GlobalConsts::BOARD_CHUNK_SIZE; i++) {
		const char* direction = dir_ary[i%2];
		std::vector< GLdouble >* x_y_diff = GlobalConsts::RENDER_TRAY_COORDS[direction];
		Hexagon* temp_hex = curr_hex;
		GLdouble temp_x = x;
		GLdouble temp_y = y;

		for(int j = 0; j < GlobalConsts::BOARD_CHUNK_SIZE; j++) {
			this->generate_render_data(temp_hex, temp_x, temp_y);

			temp_hex = temp_hex->get_neighbor("N");
			std::vector< GLdouble >* temp_x_y_diff = GlobalConsts::RENDER_TRAY_COORDS["N"];
			temp_x += temp_x_y_diff->at(0);
			temp_y += temp_x_y_diff->at(1);
		}

		curr_hex = curr_hex->get_neighbor(direction);
		x += x_y_diff->at(0);
		y += x_y_diff->at(1);
	}

	if(this->regenerate_terrain) {
		this->board_terrain_data->reverse_indicies();
	}

	if(this->regenerate_select) {
		this->board_select_data->reverse_indicies();
	}

	/*if(this->regenerate_object) {
		this->board_object_data->reverse_indicies();
	}*/
}


void GameboardChunk::verify_render_data() {
	if(this->regenerate_terrain || this->regenerate_select) {// || this->regenerate_object) {
		if(this->regenerate_terrain) {
			this->clear_terrain();
		}

		if(this->regenerate_select) {
			this->clear_select();
		}

		/*if(this->regenerate_object) {
			this->clear_object();
		}*/

		this->generate_chunk_data();
		this->write_VBO_data();

		this->regenerate_terrain = false;
		this->regenerate_select = false;
		//this->regenerate_object = false;
	}
}

void GameboardChunk::write_VBO_data() {
	this->board_terrain_data->write_VBO_data(GL_STATIC_DRAW);
	this->board_select_data->write_VBO_data(GL_STATIC_DRAW);
	//this->board_object_data->write_VBO_data(GL_STATIC_DRAW);
}

void GameboardChunk::render() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	this->board_terrain_data->bind_for_draw(GL_DOUBLE);

	// front facing polys
	glDrawElements(GL_TRIANGLES, this->board_terrain_data->indicies_size(), GL_UNSIGNED_INT, 0);

	// turn off color array so that we can draw black lines
	glDisableClientState(GL_COLOR_ARRAY);

	// draw back facing black lines
	glCullFace(GL_FRONT);
	glColor3f(0, 0, 0);
	glDrawElements(GL_TRIANGLES, this->board_terrain_data->indicies_size(), GL_UNSIGNED_INT, 0);
	glCullFace(GL_BACK);

	glDisableClientState(GL_VERTEX_ARRAY);

	//----------------------------------------

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	this->board_select_data->bind_for_draw(GL_DOUBLE);

	glDrawElements(GL_TRIANGLES, this->board_select_data->indicies_size(), GL_UNSIGNED_INT, 0);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	//----------------------------------------

	/*glDisable(GL_CULL_FACE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	this->board_object_data->bind_for_draw(GL_DOUBLE);

	glDrawElements(GL_TRIANGLES, this->board_object_data->indicies_size(), GL_UNSIGNED_INT, 0);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_CULL_FACE);*/

	//----------------------------------------

	// bind with 0, so, switch back to normal pointer operation
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


