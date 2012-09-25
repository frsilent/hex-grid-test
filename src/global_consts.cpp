#include "includes.h"

PyObject* get_py_attr(const char* attr_str) {
	PyRun_SimpleString("import os, sys"); 
	PyRun_SimpleString("sys.path.append(os.getcwd())");
	PyRun_SimpleString("sys.dont_write_bytecode = True");  
	PyObject *py_name = PyString_FromString("src.global_consts");
	PyObject *py_module = PyImport_Import(py_name);

	Py_XDECREF(py_name);

	if(PyErr_Occurred()) {
		PyErr_Print();
	}

	if(!py_module) {
		return NULL;
	}

	PyObject *py_global_consts = PyObject_GetAttrString(py_module, "GlobalConsts");
	Py_XDECREF(py_module);

	PyObject *py_attr  = PyObject_GetAttrString(py_global_consts, attr_str);
	Py_XDECREF(py_global_consts);

	return py_attr;
}

long long_global_const_attr(const char* attr_str) {
	bool py_needed_init = false;

	if(!Py_IsInitialized() ) {
		Py_Initialize();
		py_needed_init = true;
	}

	PyObject *py_attr = get_py_attr(attr_str);

	if(!py_attr) {
		return NULL;
	}

	long output = PyInt_AsLong(py_attr);
	Py_XDECREF(py_attr);

	if(py_needed_init) {
		Py_Finalize();
	}

	return output;
}


char char_global_const_attr(const char* attr_str) {
	bool py_needed_init = false;

	if(!Py_IsInitialized() ) {
		Py_Initialize();
		py_needed_init = true;
	}

	PyObject *py_attr = get_py_attr(attr_str);

	if(!py_attr) {
		return NULL;
	}

	char output = PyString_AsString(py_attr)[0];
	Py_XDECREF(py_attr);

	if(py_needed_init) {
		Py_Finalize();
	}

	return output;
}


double double_global_const_attr(const char* attr_str) {
	bool py_needed_init = false;

	if(!Py_IsInitialized() ) {
		Py_Initialize();
		py_needed_init = true;
	}

	PyObject *py_attr = get_py_attr(attr_str);

	if(!py_attr) {
		return NULL;
	}

	double output = PyFloat_AsDouble(py_attr);
	Py_XDECREF(py_attr);

	if(py_needed_init) {
		Py_Finalize();
	}

	return output;
}

std::map< const char*, std::vector< double >*, cmp_str> get_render_tray_coords() {
	std::map< const char*, std::vector< double >*, cmp_str> output; 

	output["N"] = new std::vector< double >();
	output["N"]->push_back( 0);
	output["N"]->push_back( 1.0 * GlobalConsts::SIN_60);

	output["NE"] = new std::vector< double >();
	output["NE"]->push_back( 1.5 * GlobalConsts::COS_60);
	output["NE"]->push_back( 0.5 * GlobalConsts::SIN_60);

	output["SE"] = new std::vector< double >();
	output["SE"]->push_back( 1.5 * GlobalConsts::COS_60);
	output["SE"]->push_back(-0.5 * GlobalConsts::SIN_60);

	output["S"] = new std::vector< double >();
	output["S"]->push_back( 0);
	output["S"]->push_back(-1.0 * GlobalConsts::SIN_60);

	output["SW"] = new std::vector< double >();
	output["SW"]->push_back(-1.5 * GlobalConsts::COS_60);
	output["SW"]->push_back(-0.5 * GlobalConsts::SIN_60);

	output["NW"] = new std::vector< double >();
	output["NW"]->push_back(-1.5 * GlobalConsts::COS_60);
	output["NW"]->push_back( 0.5 * GlobalConsts::SIN_60);

	return output;
}


const double GlobalConsts::COS_60 = std::cos(60.0 / 360.0 * 2.0 * M_PI);
const double GlobalConsts::SIN_60 = std::sin(60.0 / 360.0 * 2.0 * M_PI);

const char GlobalConsts::LEFT  = char_global_const_attr("LEFT");
const char GlobalConsts::RIGHT = char_global_const_attr("RIGHT");
const char GlobalConsts::UP    = char_global_const_attr("UP");
const char GlobalConsts::DOWN  = char_global_const_attr("DOWN");

const int GlobalConsts::MOUSE_LEFT 	  = long_global_const_attr("MOUSE_LEFT");
const int GlobalConsts::MOUSE_MIDDLE 	  = long_global_const_attr("MOUSE_MIDDLE");
const int GlobalConsts::MOUSE_RIGHT 	  = long_global_const_attr("MOUSE_RIGHT");
const int GlobalConsts::MOUSE_SCROLL_UP   = long_global_const_attr("MOUSE_SCROLL_UP");
const int GlobalConsts::MOUSE_SCROLL_DOWN = long_global_const_attr("MOUSE_SCROLL_DOWN");

const int GlobalConsts::RENDER_LINES     = long_global_const_attr("RENDER_LINES");
const int GlobalConsts::RENDER_TRIANGLES = long_global_const_attr("RENDER_TRIANGLES");

const long GlobalConsts::BOARD_WIDTH  = long_global_const_attr("BOARD_WIDTH");
const long GlobalConsts::BOARD_HEIGHT = long_global_const_attr("BOARD_HEIGHT");

const long GlobalConsts::BOARD_CHUNK_SIZE = long_global_const_attr("BOARD_CHUNK_SIZE");

const double GlobalConsts::MIN_ZOOM = double_global_const_attr("MIN_ZOOM");
const double GlobalConsts::MAX_ZOOM = double_global_const_attr("MAX_ZOOM");
const double GlobalConsts::START_ZOOM = double_global_const_attr("START_ZOOM");

const double GlobalConsts::START_ROTATION = double_global_const_attr("START_ROTATION");

const double GlobalConsts::START_VIEW_RANGE = double_global_const_attr("START_VIEW_RANGE");

const double GlobalConsts::MAX_PATHABLE_SLOPE = double_global_const_attr("MAX_PATHABLE_SLOPE");

std::map< const char*, std::vector< double >*, cmp_str>  GlobalConsts::RENDER_TRAY_COORDS = get_render_tray_coords();


