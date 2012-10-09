#include "includes.h"

GameboardChunk::GameboardChunk(Hexagon* base_hex) {
	this->base_hex = base_hex;

	this->board_vertex_data = NULL;
	this->board_select_data = NULL;
	
	this->vbo_hex_vert = 0;
	this->vbo_hex_indicie = 0;

	this->vbo_sel_vert = 0;
	this->vbo_sel_indicie = 0;

	glGenBuffers(1, &(this->vbo_hex_vert));
	glGenBuffers(1, &(this->vbo_hex_indicie));

	glGenBuffers(1, &(this->vbo_sel_vert));
	glGenBuffers(1, &(this->vbo_sel_indicie));

	this->regenerate_vertex = true;
	this->regenerate_select = true;
}


GameboardChunk::~GameboardChunk() {
	std::cout << "deleting: " << this << std::endl;

	if(this->board_vertex_data) {
		delete this->board_vertex_data;
	}

	if(this->board_select_data) {
		delete this->board_select_data;
	}

	glDeleteBuffers(1, &(this->vbo_hex_vert));
	glDeleteBuffers(1, &(this->vbo_hex_indicie));

	glDeleteBuffers(1, &(this->vbo_sel_vert));
	glDeleteBuffers(1, &(this->vbo_sel_indicie));
}


void GameboardChunk::clear_vertex() {
	if(this->board_vertex_data) {
		delete this->board_vertex_data;
	}

	this->board_vertex_data = new UniqueDataVector< GLfloat >();
	this->regenerate_vertex = true;
}


void GameboardChunk::clear_select() {
	if(this->board_select_data) {
		delete this->board_select_data;
	}

	this->board_select_data = new UniqueDataVector< GLfloat >();
	this->regenerate_select = true;
}

void GameboardChunk::generate_render_data(Hexagon* curr_hex, double x, double y) {
	// TODO: I don't like this logic being here.. this should be moved out...
	if(this->regenerate_vertex) {
		curr_hex->generate_vertex_data(x, y, this->board_vertex_data);
	} 

	if(this->regenerate_select) {
		curr_hex->generate_select_data(x, y, this->board_select_data);
	} 

	curr_hex->parent_chunk = this;
}


void GameboardChunk::generate_chunk_data() {
	double base_x = 0;
	double base_y = 0;

	std::cout << "Gameboard::generating: " << this->base_hex << std::endl;

	double x = base_x;
	double y = base_y;

	const char* dir_ary[] = {"SE", "NE"}; 

	Hexagon* curr_hex = this->base_hex;

	for(int i = 0; i < GlobalConsts::BOARD_CHUNK_SIZE; i++) {
		const char* direction = dir_ary[i%2];
		std::vector< double >* x_y_diff = GlobalConsts::RENDER_TRAY_COORDS[direction];
		Hexagon* temp_hex = curr_hex;
		double temp_x = x;
		double temp_y = y;

		for(int j = 0; j < GlobalConsts::BOARD_CHUNK_SIZE; j++) {
			this->generate_render_data(temp_hex, temp_x, temp_y);

			temp_hex = temp_hex->get_neighbor("N");
			std::vector< double >* temp_x_y_diff = GlobalConsts::RENDER_TRAY_COORDS["N"];
			temp_x += temp_x_y_diff->at(0);
			temp_y += temp_x_y_diff->at(1);
		}

		curr_hex = curr_hex->get_neighbor(direction);
		x += x_y_diff->at(0);
		y += x_y_diff->at(1);
	}

	if(this->regenerate_vertex) {
		this->board_vertex_data->reverse_indicies();
	}

	if(this->regenerate_select) {
		this->board_select_data->reverse_indicies();
	}
}


void GameboardChunk::verify_render_data() {
	if(this->regenerate_vertex || this->regenerate_select) {
		if(this->regenerate_vertex) {
			this->clear_vertex();
		}

		if(this->regenerate_select) {
			this->clear_select();
		}

		this->generate_chunk_data();
		this->write_VBO_data();

		this->regenerate_vertex = false;
		this->regenerate_select = false;
	}
}

void GameboardChunk::write_VBO_data() {
	/*std::cout << "VBO Start" << std::endl;
	std::cout << this->board_vertex_data->vector_size() << " | " << this->board_vertex_data->indicies_size() << " - " ;
	std::cout << this->board_select_data->vector_size() << " | " << this->board_select_data->indicies_size() << std::endl;*/

	GLsizeiptr hex_vert_size = sizeof(GLfloat) * this->board_vertex_data->vector_size();

	//std::cout << "A: " << this->vbo_hex_vert << " | " << hex_vert_size << " | " << this->board_vertex_data->vector_data() << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_hex_vert);
	glBufferData(GL_ARRAY_BUFFER, hex_vert_size, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, hex_vert_size, this->board_vertex_data->vector_data());

	GLsizeiptr hex_indicie_size = sizeof(GLuint) * this->board_vertex_data->indicies_size();

	//std::cout << "C: " << this->vbo_hex_indicie << " | " << hex_indicie_size << std::endl;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_hex_indicie);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, hex_indicie_size, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, hex_indicie_size, this->board_vertex_data->indicies_data());

	//-------------------------------------

	GLsizeiptr sel_vert_size = sizeof(GLfloat) * this->board_select_data->vector_size();

	//std::cout << "D: " << this->vbo_sel_vert << " | " << sel_vert_size << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_sel_vert);
	glBufferData(GL_ARRAY_BUFFER, sel_vert_size, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sel_vert_size, this->board_select_data->vector_data());

	GLsizeiptr sel_indicie_size = sizeof(GLuint) * this->board_select_data->indicies_size();

	//std::cout << "F: " << this->vbo_sel_indicie << " | " << sel_indicie_size << std::endl;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_sel_indicie);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sel_indicie_size, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sel_indicie_size, this->board_select_data->indicies_data());

	//std::cout << "VBO End" << " | " << glGetError() << std::endl;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//std::cout << "VBO Stop: " << glGetError() << " | " << GL_NO_ERROR << std::endl << std::endl;

}


