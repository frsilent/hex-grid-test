#ifndef GAMEBOARD_CHUNK_H
#define GAMEBOARD_CHUNK_H



class GameboardChunk {
	private:
		Hexagon* base_hex;

	public:
		GLuint vbo_hex_vert;
		GLuint vbo_hex_indicie;

		GLuint vbo_sel_vert;
		GLuint vbo_sel_indicie;

		UniqueDataVector< GLfloat >* board_vertex_data;
		UniqueDataVector< GLfloat >* board_select_data;

		bool regenerate_vertex;
		bool regenerate_select;

		GameboardChunk(Hexagon*);
		~GameboardChunk();

		void clear_vertex();
		void clear_select();

		void generate_chunk_data();
		void generate_render_data(Hexagon*, double, double);

		void verify_render_data();

		void write_VBO_data();

};

#endif

